#include "progressive/http_client.hpp"
#include <cstring>
#include <sstream>

// Note: Full implementation requires JNI bridge for TLS socket.
// The C++ side handles HTTP protocol formatting and parsing.
// TLS connection is delegated to Android's javax.net.ssl.SSLSocket via JNI.
// See jni_bridge.cpp for the native TLS socket wrapper.

namespace progressive {

// ==== URL Parsing ====

ParsedUrl parseUrl(const std::string& url) {
    ParsedUrl result;

    // Find scheme
    auto schemeEnd = url.find("://");
    if (schemeEnd == std::string::npos) return result;
    result.scheme = url.substr(0, schemeEnd);
    result.port = (result.scheme == "https") ? 443 : 80;

    // Find host start (after ://)
    size_t hostStart = schemeEnd + 3;
    auto pathStart = url.find('/', hostStart);
    if (pathStart == std::string::npos) {
        result.host = url.substr(hostStart);
        result.path = "/";
    } else {
        result.host = url.substr(hostStart, pathStart - hostStart);
        result.path = url.substr(pathStart);
    }

    // Check for port in host
    auto portSep = result.host.find(':');
    if (portSep != std::string::npos) {
        result.port = std::stoi(result.host.substr(portSep + 1));
        result.host = result.host.substr(0, portSep);
    }

    result.valid = !result.host.empty();
    return result;
}

// ==== HTTP Protocol Formatting ====

static std::string buildHttpRequest(const HttpRequest& req) {
    auto parsed = parseUrl(req.url);
    if (!parsed.valid) return "";

    std::ostringstream os;
    // Request line
    os << req.method << " " << parsed.path << " HTTP/1.1\r\n";
    // Host header
    os << "Host: " << parsed.host;
    if (parsed.port != 443 && parsed.port != 80)
        os << ":" << parsed.port;
    os << "\r\n";
    // Custom headers
    for (const auto& [k, v] : req.headers) {
        os << k << ": " << v << "\r\n";
    }
    // Content length if body
    if (!req.body.empty()) {
        os << "Content-Length: " << req.body.size() << "\r\n";
    }
    // Connection
    os << "Connection: close\r\n";
    // End headers
    os << "\r\n";
    // Body
    if (!req.body.empty()) {
        os << req.body;
    }
    return os.str();
}

static HttpResponse parseHttpResponse(const std::string& raw) {
    HttpResponse resp;

    // Find header/body separator
    auto bodyStart = raw.find("\r\n\r\n");
    if (bodyStart == std::string::npos) {
        resp.errorMessage = "Invalid HTTP response";
        return resp;
    }

    std::string headerBlock = raw.substr(0, bodyStart);
    resp.body = raw.substr(bodyStart + 4);

    // Parse status line
    auto firstNl = headerBlock.find("\r\n");
    if (firstNl == std::string::npos) {
        resp.errorMessage = "No status line";
        return resp;
    }
    std::string statusLine = headerBlock.substr(0, firstNl);

    // "HTTP/1.1 200 OK"
    auto codeStart = statusLine.find(' ');
    if (codeStart != std::string::npos) {
        auto codeEnd = statusLine.find(' ', codeStart + 1);
        std::string codeStr = (codeEnd != std::string::npos)
            ? statusLine.substr(codeStart + 1, codeEnd - codeStart - 1)
            : statusLine.substr(codeStart + 1);
        resp.statusCode = std::stoi(codeStr);
    }

    // Parse headers
    size_t pos = firstNl + 2;
    while (pos < headerBlock.size()) {
        auto nl = headerBlock.find("\r\n", pos);
        if (nl == std::string::npos) break;
        std::string line = headerBlock.substr(pos, nl - pos);
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            // Trim leading space from value
            if (!value.empty() && value[0] == ' ') value = value.substr(1);
            resp.headers[key] = value;
        }
        pos = nl + 2;
    }

    resp.success = true;
    return resp;
}

// ==== Main Execute ====
//
// JNI bridge: the actual socket connection + TLS is done in Java via
// javax.net.ssl.SSLSocket. This C++ function formats the HTTP request
// and parses the response. The middle layer (send/receive) is in
// jni_bridge.cpp's nativeTlsRequest() function.
//
// For now, returns a stub response. Will be wired via JNI.

HttpResponse httpExecute(const HttpRequest& req) {
    // Stage 1: Build HTTP request string
    std::string httpRequest = buildHttpRequest(req);
    if (httpRequest.empty()) {
        return {0, "", {}, false, "Failed to build HTTP request"};
    }

    // Stage 2: Send via JNI TLS socket, receive response
    // This will call: Java String nativeTlsRequest(url, httpRequest, timeoutMs)
    // The JNI bridge handles the actual network I/O.

    // Stage 3: Parse response
    // For now, return placeholder — actual network call via JNI in next commit
    HttpResponse resp;
    resp.success = false;
    resp.errorMessage = "JNI TLS bridge not yet wired — see jni_bridge.cpp";
    return resp;
}

// ==== Form Body ====

std::string buildFormBody(const std::unordered_map<std::string, std::string>& params) {
    std::ostringstream os;
    bool first = true;
    for (const auto& [k, v] : params) {
        if (!first) os << "&";
        first = false;
        os << k << "=" << v; // URL encoding delegated to advanced impl
    }
    return os.str();
}

// ==== Rate Limit ====

RateLimitInfo parseRateLimitHeaders(const HttpResponse& response) {
    RateLimitInfo info;
    auto it = response.headers.find("X-RateLimit-Limit");
    if (it != response.headers.end()) info.limit = std::stoi(it->second);
    it = response.headers.find("X-RateLimit-Remaining");
    if (it != response.headers.end()) info.remaining = std::stoi(it->second);
    it = response.headers.find("X-RateLimit-Reset");
    if (it != response.headers.end()) info.resetMs = std::stoll(it->second) * 1000;
    return info;
}

// ==== Matrix Error ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '"')) pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') { if (json[end] == '\\') end++; end++; }
    return json.substr(pos, end - pos);
}

MatrixErrorResponse parseMatrixError(const std::string& responseBody) {
    MatrixErrorResponse err;
    err.errcode = extractJsonString(responseBody, "errcode");
    err.error = extractJsonString(responseBody, "error");
    auto retry = extractJsonString(responseBody, "retry_after_ms");
    if (!retry.empty()) err.retryAfterMs = std::stoi(retry);
    return err;
}

} // namespace progressive
