     1|#include <jni.h>
     2|#include <string>
     3|#include <android/log.h>
     4|#include "progressive/jumptodate.hpp"
     5|#include "progressive/relation.hpp"
     6|#include "progressive/exporter.hpp"
     7|#include "progressive/eventcache.hpp"
     8|#include "progressive/eventdb.hpp"
     9|#include "progressive/translate.hpp"
    10|#include "progressive/proxy.hpp"
    11|#include "progressive/yggdrasil.hpp"
    12|#include "progressive/markdown.hpp"
    13|#include "progressive/account_export.hpp"
    14|#include "progressive/audio_engine.hpp"
    15|#include "progressive/media_filter.hpp"
    16|
    17|#define LOG_TAG "ProgressiveNative"
    18|#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
    19|#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
    20|
    21|extern "C" {
    22|
    23|/*
    24| * Class: im.vector.app.features.jumptodate.ProgressiveNative
    25| * Method: nativeValidateAndBuild
    26| * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;
    27| *
    28| * Returns JSON string: {"url": "...", ...} or {"error": "..."}
    29| */
    30|JNIEXPORT jstring JNICALL
    31|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeValidateAndBuild(
    32|    JNIEnv* env,
    33|    jclass /* this */,
    34|    jstring jRoomId,
    35|    jstring jDateString,
    36|    jstring jServerUrl,
    37|    jstring jAccessToken,
    38|    jboolean jIsEnabled
    39|) {
    40|    // Feature flag check — C++ gates the feature
    41|    if (!jIsEnabled) {
    42|        const char* err = R"({"error":"/jumptodate is disabled. Enable it in Settings → Labs."})";
    43|        return env->NewStringUTF(err);
    44|    }
    45|    if (!jRoomId || !jDateString || !jServerUrl || !jAccessToken) {
    46|        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
    47|        env->ThrowNew(exClass, "All parameters must be non-null");
    48|        return nullptr;
    49|    }
    50|
    51|    // Convert Java strings to C++ strings
    52|    const char* roomIdCh     = env->GetStringUTFChars(jRoomId, nullptr);
    53|    const char* dateCh       = env->GetStringUTFChars(jDateString, nullptr);
    54|    const char* serverUrlCh  = env->GetStringUTFChars(jServerUrl, nullptr);
    55|    const char* accessTokenCh = env->GetStringUTFChars(jAccessToken, nullptr);
    56|
    57|    progressive::JumpToDateRequest request;
    58|    request.roomId        = std::string(roomIdCh);
    59|    request.dateString    = std::string(dateCh);
    60|    request.serverBaseUrl = std::string(serverUrlCh);
    61|    request.accessToken   = std::string(accessTokenCh);
    62|
    63|    // Release Java strings
    64|    env->ReleaseStringUTFChars(jRoomId, roomIdCh);
    65|    env->ReleaseStringUTFChars(jDateString, dateCh);
    66|    env->ReleaseStringUTFChars(jServerUrl, serverUrlCh);
    67|    env->ReleaseStringUTFChars(jAccessToken, accessTokenCh);
    68|
    69|    LOGD("nativeJumpToDate called: roomId=%s date=%s", request.roomId.c_str(), request.dateString.c_str());
    70|
    71|    // Validate date and compute timestamp
    72|    if (!progressive::validateAndComputeTimestamp(request)) {
    73|        std::string errorJson = R"({"error":")" + request.errorMessage + R"("})";
    74|        return env->NewStringUTF(errorJson.c_str());
    75|    }
    76|
    77|    // Build the MSC3030 URL
    78|    std::string url = progressive::buildMsc3030Url(request);
    79|    LOGD("MSC3030 URL: %s", url.c_str());
    80|
    81|    // Return the URL and timestamp to Kotlin — HTTP call happens in Kotlin layer
    82|    std::string resultJson =
    83|        R"({"url":")" + url +
    84|        R"(","accessToken":")" + request.accessToken +
    85|        R"(","timestamp":)" + std::to_string(request.originServerTs) +
    86|        R"(})";
    87|    return env->NewStringUTF(resultJson.c_str());
    88|}
    89|
    90|/*
    91| * Class: im.vector.app.features.jumptodate.ProgressiveNative
    92| * Method: nativeParseResponse
    93| * Signature: (Ljava/lang/String;I)Ljava/lang/String;
    94| *
    95| * Parses the HTTP response body and returns JSON with eventId or error.
    96| */
    97|JNIEXPORT jstring JNICALL
    98|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseResponse(
    99|    JNIEnv* env,
   100|    jclass /* this */,
   101|    jstring jResponseBody,
   102|    jint httpStatus
   103|) {
   104|    if (!jResponseBody) {
   105|        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
   106|        env->ThrowNew(exClass, "Response body must be non-null");
   107|        return nullptr;
   108|    }
   109|
   110|    const char* bodyCh = env->GetStringUTFChars(jResponseBody, nullptr);
   111|    std::string body(bodyCh);
   112|    env->ReleaseStringUTFChars(jResponseBody, bodyCh);
   113|
   114|    LOGD("nativeParseResponse: status=%d body_len=%zu", httpStatus, body.size());
   115|
   116|    auto result = progressive::parseTimestampToEventResponse(body, httpStatus);
   117|
   118|    if (result.success) {
   119|        std::string json = R"({"eventId":")" + result.eventId + R"("})";
   120|        return env->NewStringUTF(json.c_str());
   121|    } else {
   122|        std::string json =
   123|            R"({"error":")" + result.errorMessage +
   124|            R"(","statusCode":)" + std::to_string(result.statusCode) + R"(})";
   125|        return env->NewStringUTF(json.c_str());
   126|    }
   127|}
   128|
   129|/*
   130| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   131| * Method: nativeParseRelation
   132| * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
   133| *
   134| * Parses event JSON to find the source event this event relates to.
   135| * @param eventJson  Full JSON of the Matrix event
   136| * @param allowedTypes  Comma-separated list of allowed relation types (e.g. "m.annotation,m.reference")
   137| *                      or empty string for all types.
   138| *
   139| * Returns JSON: {"sourceEventId": "$xyz", "relationType": "m.annotation"} or {"isRelation": false}
   140| */
   141|JNIEXPORT jstring JNICALL
   142|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseRelation(
   143|    JNIEnv* env,
   144|    jclass /* this */,
   145|    jstring jEventJson,
   146|    jstring jAllowedTypes
   147|) {
   148|    if (!jEventJson) {
   149|        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
   150|        env->ThrowNew(exClass, "Event JSON must be non-null");
   151|        return nullptr;
   152|    }
   153|
   154|    const char* jsonCh = env->GetStringUTFChars(jEventJson, nullptr);
   155|    std::string json(jsonCh);
   156|    env->ReleaseStringUTFChars(jEventJson, jsonCh);
   157|
   158|    LOGD("nativeParseRelation: eventJson_len=%zu", json.size());
   159|
   160|    auto relation = progressive::parseRelation(json);
   161|
   162|    if (!relation.isRelation) {
   163|        return env->NewStringUTF(R"json({"isRelation": false})json");
   164|    }
   165|
   166|    // Check if this relation type is allowed
   167|    bool allowed = true;
   168|    if (jAllowedTypes) {
   169|        const char* allowedCh = env->GetStringUTFChars(jAllowedTypes, nullptr);
   170|        std::string allowedStr(allowedCh);
   171|        env->ReleaseStringUTFChars(jAllowedTypes, allowedCh);
   172|
   173|        if (!allowedStr.empty()) {
   174|            allowed = progressive::isJumpableRelationType(relation.relationType) &&
   175|                      allowedStr.find(relation.relationType) != std::string::npos;
   176|        }
   177|    } else {
   178|        allowed = progressive::isJumpableRelationType(relation.relationType);
   179|    }
   180|
   181|    if (!allowed) {
   182|        return env->NewStringUTF(R"json({"isRelation": false})json");
   183|    }
   184|
   185|    std::string resultJson =
   186|        R"({"isRelation": true, "sourceEventId": ")" + relation.sourceEventId +
   187|        R"(", "relationType": ")" + relation.relationType + R"("})";
   188|    return env->NewStringUTF(resultJson.c_str());
   189|}
   190|
   191|/*
   192| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   193| * Method: nativeFormatEventHtml
   194| * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;
   195| *
   196| * Formats a single event as HTML.
   197| * @param senderName  Display name of the sender
   198| * @param timestamp   ISO 8601 timestamp string
   199| * @param body        Plain text body
   200| * @param msgType     Message type (m.text, m.image, etc.)
   201| * @param fileName    Attachment filename if applicable
   202| * @param mediaSize   Attachment size in bytes as string (empty if none)
   203| * @param relationType Relation type string (m.annotation, m.reference, etc.)
   204| * @param isContinuation Whether this event is from the same sender as previous
   205| */
   206|JNIEXPORT jstring JNICALL
   207|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatEventHtml(
   208|    JNIEnv* env,
   209|    jclass /* this */,
   210|    jstring jSenderName,
   211|    jstring jTimestamp,
   212|    jstring jBody,
   213|    jstring jMsgType,
   214|    jstring jFileName,
   215|    jstring jMediaSize,
   216|    jstring jRelationType,
   217|    jboolean jIsContinuation
   218|) {
   219|    ExportEvent event;
   220|    event.senderName   = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
   221|    event.timestamp    = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
   222|    event.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
   223|    event.msgType      = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
   224|    event.fileName     = jFileName ? std::string(env->GetStringUTFChars(jFileName, nullptr)) : "";
   225|    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";
   226|
   227|    if (jMediaSize) {
   228|        auto sizeStr = std::string(env->GetStringUTFChars(jMediaSize, nullptr));
   229|        if (!sizeStr.empty()) event.mediaSize = std::stoll(sizeStr);
   230|        env->ReleaseStringUTFChars(jMediaSize, sizeStr.c_str());
   231|    }
   232|
   233|    // Release all strings
   234|    if (jSenderName)   env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
   235|    if (jTimestamp)    env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
   236|    if (jBody)         env->ReleaseStringUTFChars(jBody, event.body.c_str());
   237|    if (jMsgType)      env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
   238|    if (jFileName)     env->ReleaseStringUTFChars(jFileName, event.fileName.c_str());
   239|    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());
   240|
   241|    auto html = progressive::formatEventHtml(event, jIsContinuation);
   242|    return env->NewStringUTF(html.c_str());
   243|}
   244|
   245|/*
   246| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   247| * Method: nativeFormatEventPlainText
   248| * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
   249| *
   250| * Formats a single event as plain text line.
   251| */
   252|JNIEXPORT jstring JNICALL
   253|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatEventPlainText(
   254|    JNIEnv* env,
   255|    jclass /* this */,
   256|    jstring jSenderName,
   257|    jstring jTimestamp,
   258|    jstring jBody,
   259|    jstring jMsgType,
   260|    jstring jFileName,
   261|    jstring jRelationType
   262|) {
   263|    ExportEvent event;
   264|    event.senderName   = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
   265|    event.timestamp    = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
   266|    event.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
   267|    event.msgType      = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
   268|    event.fileName     = jFileName ? std::string(env->GetStringUTFChars(jFileName, nullptr)) : "";
   269|    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";
   270|
   271|    if (jSenderName)   env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
   272|    if (jTimestamp)    env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
   273|    if (jBody)         env->ReleaseStringUTFChars(jBody, event.body.c_str());
   274|    if (jMsgType)      env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
   275|    if (jFileName)     env->ReleaseStringUTFChars(jFileName, event.fileName.c_str());
   276|    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());
   277|
   278|    auto text = progressive::formatEventPlainText(event);
   279|    return env->NewStringUTF(text.c_str());
   280|}
   281|
   282|/*
   283| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   284| * Method: nativeBuildHtmlExport
   285| * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/String;
   286| *
   287| * Builds a complete HTML export document from event HTML strings.
   288| * @param roomName    Room display name
   289| * @param roomTopic   Room topic
   290| * @param exportDate  ISO date of export
   291| * @param eventHtmls  Array of pre-rendered event HTML strings
   292| */
   293|JNIEXPORT jstring JNICALL
   294|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildHtmlExport(
   295|    JNIEnv* env,
   296|    jclass /* this */,
   297|    jstring jRoomName,
   298|    jstring jRoomTopic,
   299|    jstring jExportDate,
   300|    jobjectArray jEventHtmls
   301|) {
   302|    auto roomName   = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
   303|    auto roomTopic  = jRoomTopic ? std::string(env->GetStringUTFChars(jRoomTopic, nullptr)) : "";
   304|    auto exportDate = jExportDate ? std::string(env->GetStringUTFChars(jExportDate, nullptr)) : "";
   305|
   306|    if (jRoomName)  env->ReleaseStringUTFChars(jRoomName, roomName.c_str());
   307|    if (jRoomTopic) env->ReleaseStringUTFChars(jRoomTopic, roomTopic.c_str());
   308|    if (jExportDate) env->ReleaseStringUTFChars(jExportDate, exportDate.c_str());
   309|
   310|    // Build HTML document — events are already formatted, wrap them
   311|    std::ostringstream html;
   312|    html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
   313|    html << "<meta charset=\"UTF-8\">\n";
   314|    html << "<title>" << escapeHtml(roomName) << " — Chat Export</title>\n";
   315|    html << R"(<style>
   316|body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; margin: 0; padding: 16px; background: #f5f5f5; }
   317|.mx_ExportHeader { background: #fff; border-radius: 8px; padding: 16px; margin-bottom: 16px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
   318|.mx_ExportHeader h1 { margin: 0 0 8px; font-size: 1.5em; }
   319|.mx_ExportHeader p { margin: 4px 0; color: #666; font-size: 0.9em; }
   320|.mx_EventTile { background: #fff; border-radius: 8px; padding: 12px 16px; margin-bottom: 8px; box-shadow: 0 1px 2px rgba(0,0,0,0.05); }
   321|.mx_EventTile_continuation { margin-top: -4px; border-radius: 0 0 8px 8px; }
   322|.mx_EventTile_info { margin-bottom: 6px; }
   323|.mx_EventTile_sender { font-weight: 600; color: #333; margin-right: 8px; }
   324|.mx_MessageTimestamp { color: #999; font-size: 0.85em; }
   325|.mx_EventTile_body { color: #222; line-height: 1.5; }
   326|.mx_EventTile_attachment { background: #f0f0f0; border-radius: 4px; padding: 8px; margin-bottom: 8px; }
   327|.mx_Attachment_name { font-weight: 500; }
   328|.mx_Attachment_size { color: #999; margin-left: 8px; }
   329|.mx_EventTile_reaction { font-style: italic; color: #666; }
   330|.mx_EventTile_content { white-space: pre-wrap; word-wrap: break-word; }
   331|hr { border: none; border-top: 1px solid #e0e0e0; margin: 16px 0; }
   332|</style>\n";
   333|    html << "</head>\n<body>\n";
   334|    html << "<div class=\"mx_ExportHeader\">\n";
   335|    html << "  <h1>" << escapeHtml(roomName) << "</h1>\n";
   336|    if (!roomTopic.empty()) html << "  <p>" << escapeHtml(roomTopic) << "</p>\n";
   337|    html << "  <p>Exported: " << exportDate << "</p>\n";
   338|
   339|    jsize count = env->GetArrayLength(jEventHtmls);
   340|    html << "  <p>Total messages: " << count << "</p>\n";
   341|    html << "</div>\n";
   342|
   343|    for (jsize i = 0; i < count; ++i) {
   344|        auto jHtml = (jstring)env->GetObjectArrayElement(jEventHtmls, i);
   345|        if (jHtml) {
   346|            auto htmlStr = std::string(env->GetStringUTFChars(jHtml, nullptr));
   347|            html << htmlStr;
   348|            env->ReleaseStringUTFChars(jHtml, htmlStr.c_str());
   349|        }
   350|    }
   351|
   352|    html << "<hr>\n<p style=\"color:#999;text-align:center;\">Exported with Progressive Chat</p>\n";
   353|    html << "</body>\n</html>";
   354|
   355|    return env->NewStringUTF(html.str().c_str());
   356|}
   357|
   358|// --- Event Cache (global singleton for Stage 2 acceleration) ---
   359|static progressive::EventCache g_eventCache;
   360|
   361|/*
   362| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   363| * Method: nativeCachePut
   364| * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V
   365| */
   366|JNIEXPORT void JNICALL
   367|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCachePut(
   368|    JNIEnv* env,
   369|    jclass /* this */,
   370|    jstring jEventId,
   371|    jstring jSenderId,
   372|    jstring jSenderName,
   373|    jstring jTimestamp,
   374|    jstring jBody,
   375|    jstring jMsgType,
   376|    jstring jEventType,
   377|    jstring jRelationType,
   378|    jstring jSourceEventId,
   379|    jboolean jSentByMe
   380|) {
   381|    CachedEvent event;
   382|    event.eventId     = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
   383|    event.senderId    = jSenderId ? std::string(env->GetStringUTFChars(jSenderId, nullptr)) : "";
   384|    event.senderName  = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
   385|    event.timestamp   = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
   386|    event.body        = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
   387|    event.msgType     = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
   388|    event.eventType   = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
   389|    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";
   390|    event.sourceEventId = jSourceEventId ? std::string(env->GetStringUTFChars(jSourceEventId, nullptr)) : "";
   391|    event.sentByMe    = jSentByMe;
   392|
   393|    // Release strings
   394|    if (jEventId)     env->ReleaseStringUTFChars(jEventId, event.eventId.c_str());
   395|    if (jSenderId)    env->ReleaseStringUTFChars(jSenderId, event.senderId.c_str());
   396|    if (jSenderName)  env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
   397|    if (jTimestamp)   env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
   398|    if (jBody)        env->ReleaseStringUTFChars(jBody, event.body.c_str());
   399|    if (jMsgType)     env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
   400|    if (jEventType)   env->ReleaseStringUTFChars(jEventType, event.eventType.c_str());
   401|    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());
   402|    if (jSourceEventId) env->ReleaseStringUTFChars(jSourceEventId, event.sourceEventId.c_str());
   403|
   404|    g_eventCache.put(event);
   405|}
   406|
   407|/*
   408| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   409| * Method: nativeCacheGetContext
   410| * Signature: (Ljava/lang/String;)Ljava/lang/String;
   411| */
   412|JNIEXPORT jstring JNICALL
   413|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheGetContext(
   414|    JNIEnv* env,
   415|    jclass /* this */,
   416|    jstring jEventId
   417|) {
   418|    if (!jEventId) {
   419|        return env->NewStringUTF(R"json({"cached": false})json");
   420|    }
   421|
   422|    auto eventId = std::string(env->GetStringUTFChars(jEventId, nullptr));
   423|    env->ReleaseStringUTFChars(jEventId, eventId.c_str());
   424|
   425|    auto json = g_eventCache.getContextData(eventId);
   426|    return env->NewStringUTF(json.c_str());
   427|}
   428|
   429|/*
   430| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   431| * Method: nativeCacheClear
   432| * Signature: ()V
   433| */
   434|JNIEXPORT void JNICALL
   435|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheClear(
   436|    JNIEnv* /* env */,
   437|    jclass /* this */
   438|) {
   439|    g_eventCache.clear();
   440|    LOGD("EventCache cleared");
   441|}
   442|
   443|/*
   444| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   445| * Method: nativeCacheSize
   446| * Signature: ()I
   447| */
   448|JNIEXPORT jint JNICALL
   449|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheSize(
   450|    JNIEnv* /* env */,
   451|    jclass /* this */
   452|) {
   453|    return static_cast<jint>(g_eventCache.size());
   454|}
   455|
   456|// --- Event Database (SQLite-based replacement for slow Realm queries) ---
   457|static progressive::EventDatabase g_eventDb;
   458|
   459|/*
   460| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   461| * Method: nativeDbOpen
   462| * Signature: (Ljava/lang/String;)Z
   463| */
   464|JNIEXPORT jboolean JNICALL
   465|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbOpen(
   466|    JNIEnv* env,
   467|    jclass /* this */,
   468|    jstring jDbPath
   469|) {
   470|    if (!jDbPath) return JNI_FALSE;
   471|    auto path = std::string(env->GetStringUTFChars(jDbPath, nullptr));
   472|    env->ReleaseStringUTFChars(jDbPath, path.c_str());
   473|    return g_eventDb.open(path) ? JNI_TRUE : JNI_FALSE;
   474|}
   475|
   476|/*
   477| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   478| * Method: nativeDbClose
   479| * Signature: ()V
   480| */
   481|JNIEXPORT void JNICALL
   482|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbClose(JNIEnv*, jclass) {
   483|    g_eventDb.close();
   484|}
   485|
   486|/*
   487| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   488| * Method: nativeDbInsertEvent
   489| * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JIZ)V
   490| */
   491|JNIEXPORT void JNICALL
   492|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbInsertEvent(
   493|    JNIEnv* env, jclass,
   494|    jstring jEventId, jstring jRoomId, jstring jSenderId, jstring jSenderName,
   495|    jstring jTimestamp, jstring jBody, jstring jMsgType, jstring jEventType,
   496|    jstring jRelationType, jstring jSourceEventId,
   497|    jlong jOriginServerTs, jint jDisplayIndex, jboolean jSentByMe
   498|) {
   499|    DbEvent e;
   500|    e.eventId       = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
   501|    e.roomId        = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
   502|    e.senderId      = jSenderId ? std::string(env->GetStringUTFChars(jSenderId, nullptr)) : "";
   503|    e.senderName    = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
   504|    e.timestamp     = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
   505|    e.body          = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
   506|    e.msgType       = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
   507|    e.eventType     = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
   508|    e.relationType  = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";
   509|    e.sourceEventId = jSourceEventId ? std::string(env->GetStringUTFChars(jSourceEventId, nullptr)) : "";
   510|    e.originServerTs = jOriginServerTs;
   511|    e.displayIndex  = jDisplayIndex;
   512|    e.sentByMe      = jSentByMe;
   513|
   514|    if (jEventId)     env->ReleaseStringUTFChars(jEventId, e.eventId.c_str());
   515|    if (jRoomId)      env->ReleaseStringUTFChars(jRoomId, e.roomId.c_str());
   516|    if (jSenderId)    env->ReleaseStringUTFChars(jSenderId, e.senderId.c_str());
   517|    if (jSenderName)  env->ReleaseStringUTFChars(jSenderName, e.senderName.c_str());
   518|    if (jTimestamp)   env->ReleaseStringUTFChars(jTimestamp, e.timestamp.c_str());
   519|    if (jBody)        env->ReleaseStringUTFChars(jBody, e.body.c_str());
   520|    if (jMsgType)     env->ReleaseStringUTFChars(jMsgType, e.msgType.c_str());
   521|    if (jEventType)   env->ReleaseStringUTFChars(jEventType, e.eventType.c_str());
   522|    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, e.relationType.c_str());
   523|    if (jSourceEventId) env->ReleaseStringUTFChars(jSourceEventId, e.sourceEventId.c_str());
   524|
   525|    g_eventDb.insertEvent(e);
   526|}
   527|
   528|/*
   529| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   530| * Method: nativeDbGetContext
   531| * Signature: (Ljava/lang/String;)Ljava/lang/String;
   532| */
   533|JNIEXPORT jstring JNICALL
   534|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbGetContext(
   535|    JNIEnv* env, jclass, jstring jEventId
   536|) {
   537|    if (!jEventId) return env->NewStringUTF(R"json({"cached": false})json");
   538|    auto id = std::string(env->GetStringUTFChars(jEventId, nullptr));
   539|    env->ReleaseStringUTFChars(jEventId, id.c_str());
   540|
   541|    auto json = g_eventDb.getContextJson(id);
   542|    return env->NewStringUTF(json.c_str());
   543|}
   544|
   545|/*
   546| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   547| * Method: nativeDbClearRoom
   548| * Signature: (Ljava/lang/String;)V
   549| */
   550|JNIEXPORT void JNICALL
   551|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbClearRoom(
   552|    JNIEnv* env, jclass, jstring jRoomId
   553|) {
   554|    if (!jRoomId) return;
   555|    auto id = std::string(env->GetStringUTFChars(jRoomId, nullptr));
   556|    env->ReleaseStringUTFChars(jRoomId, id.c_str());
   557|    g_eventDb.clearRoom(id);
   558|}
   559|
   560|/*
   561| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   562| * Method: nativeDbCount
   563| * Signature: ()I
   564| */
   565|JNIEXPORT jint JNICALL
   566|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbCount(JNIEnv*, jclass) {
   567|    return g_eventDb.count();
   568|}
   569|
   570|// --- Translation ---
   571|
   572|/*
   573| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   574| * Method: nativeBuildTranslateRequest
   575| * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
   576| */
   577|JNIEXPORT jstring JNICALL
   578|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildTranslateRequest(
   579|    JNIEnv* env, jclass,
   580|    jstring jText, jstring jSourceLang, jstring jTargetLang,
   581|    jstring jApiEndpoint, jstring jApiToken, jstring jModel
   582|) {
   583|    TranslateRequest req;
   584|    req.text           = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
   585|    req.sourceLanguage = jSourceLang ? std::string(env->GetStringUTFChars(jSourceLang, nullptr)) : "";
   586|    req.targetLanguage = jTargetLang ? std::string(env->GetStringUTFChars(jTargetLang, nullptr)) : "";
   587|    req.apiEndpoint    = jApiEndpoint ? std::string(env->GetStringUTFChars(jApiEndpoint, nullptr)) : "";
   588|    req.apiToken       = jApiToken ? std::string(env->GetStringUTFChars(jApiToken, nullptr)) : "";
   589|    req.model          = jModel ? std::string(env->GetStringUTFChars(jModel, nullptr)) : "gpt-4o-mini";
   590|
   591|    if (jText)        env->ReleaseStringUTFChars(jText, req.text.c_str());
   592|    if (jSourceLang)  env->ReleaseStringUTFChars(jSourceLang, req.sourceLanguage.c_str());
   593|    if (jTargetLang)  env->ReleaseStringUTFChars(jTargetLang, req.targetLanguage.c_str());
   594|    if (jApiEndpoint) env->ReleaseStringUTFChars(jApiEndpoint, req.apiEndpoint.c_str());
   595|    if (jApiToken)    env->ReleaseStringUTFChars(jApiToken, req.apiToken.c_str());
   596|    if (jModel)       env->ReleaseStringUTFChars(jModel, req.model.c_str());
   597|
   598|    auto body = progressive::buildTranslateRequestBody(req);
   599|    return env->NewStringUTF(body.c_str());
   600|}
   601|
   602|/*
   603| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   604| * Method: nativeParseTranslateResponse
   605| * Signature: (Ljava/lang/String;I)Ljava/lang/String;
   606| */
   607|JNIEXPORT jstring JNICALL
   608|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseTranslateResponse(
   609|    JNIEnv* env, jclass,
   610|    jstring jResponseBody, jint jHttpStatus
   611|) {
   612|    if (!jResponseBody) {
   613|        return env->NewStringUTF(R"json({"success": false, "error": "Empty response"})json");
   614|    }
   615|    auto body = std::string(env->GetStringUTFChars(jResponseBody, nullptr));
   616|    env->ReleaseStringUTFChars(jResponseBody, body.c_str());
   617|
   618|    auto result = progressive::parseTranslateResponse(body, jHttpStatus);
   619|
   620|    if (result.success) {
   621|        std::string json = R"({"success": true, "translatedText": ")" + result.translatedText + R"("})";
   622|        return env->NewStringUTF(json.c_str());
   623|    } else {
   624|        std::string json = R"({"success": false, "error": ")" + result.errorMessage + R"(", "statusCode": )"
   625|            + std::to_string(result.statusCode) + "}";
   626|        return env->NewStringUTF(json.c_str());
   627|    }
   628|}
   629|
   630|// --- Proxy Configuration ---
   631|
   632|/*
   633| * Class: im.vector.app.features.jumptodate.ProgressiveNative
   634| * Method: nativeComputeProxyConfig
   635| * Signature: (IILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;
   636| */
   637|JNIEXPORT jstring JNICALL
   638|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeProxyConfig(
   639|    JNIEnv* env, jclass,
   640|    jint jConnType, jint jProxyType,
   641|    jstring jHost, jint jPort,
   642|    jstring jUsername, jstring jPassword
   643|) {
   644|    auto connType = static_cast<ConnectionType>(jConnType);
   645|    auto proxyType = static_cast<ProxyType>(jProxyType);
   646|
   647|    auto host     = jHost ? std::string(env->GetStringUTFChars(jHost, nullptr)) : "";
   648|    auto username = jUsername ? std::string(env->GetStringUTFChars(jUsername, nullptr)) : "";
   649|    auto password = jPassword ? std::string(env->GetStringUTFChars(jPassword, nullptr)) : "";
   650|
   651|    if (jHost)     env->ReleaseStringUTFChars(jHost, host.c_str());
   652|    if (jUsername) env->ReleaseStringUTFChars(jUsername, username.c_str());
   653|    if (jPassword) env->ReleaseStringUTFChars(jPassword, password.c_str());
   654|
   655|    auto config = progressive::computeProxyConfig(
   656|        connType, proxyType, host, jPort, username, password
   657|    );
   658|
   659|    auto json = config.toJson();
   660|    return env->NewStringUTF(json.c_str());
   661|}
   662|
   663|// --- Yggdrasil ---
   664|
   665|JNIEXPORT jboolean JNICALL
   666|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsYggdrasilAddress(
   667|    JNIEnv* env, jclass, jstring jAddr
   668|) {
   669|    if (!jAddr) return JNI_FALSE;
   670|    auto addr = std::string(env->GetStringUTFChars(jAddr, nullptr));
   671|    env->ReleaseStringUTFChars(jAddr, addr.c_str());
   672|    return progressive::isYggdrasilAddress(addr) ? JNI_TRUE : JNI_FALSE;
   673|}
   674|
   675|JNIEXPORT jboolean JNICALL
   676|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsYggdrasilDomain(
   677|    JNIEnv* env, jclass, jstring jHost
   678|) {
   679|    if (!jHost) return JNI_FALSE;
   680|    auto host = std::string(env->GetStringUTFChars(jHost, nullptr));
   681|    env->ReleaseStringUTFChars(jHost, host.c_str());
   682|    return progressive::isYggdrasilDomain(host) ? JNI_TRUE : JNI_FALSE;
   683|}
   684|
   685|JNIEXPORT jstring JNICALL
   686|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildYggHomeserverUrl(
   687|    JNIEnv* env, jclass, jstring jAddr, jint jPort, jboolean jTls
   688|) {
   689|    if (!jAddr) return env->NewStringUTF("");
   690|    auto addr = std::string(env->GetStringUTFChars(jAddr, nullptr));
   691|    env->ReleaseStringUTFChars(jAddr, addr.c_str());
   692|    auto url = progressive::buildYggHomeserverUrl(addr, jPort, jTls);
   693|    return env->NewStringUTF(url.c_str());
   694|}
   695|
   696|JNIEXPORT jstring JNICALL
   697|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeRewriteHomeserverUrl(
   698|    JNIEnv* env, jclass, jstring jOriginalUrl, jstring jYggAddr
   699|) {
   700|    if (!jOriginalUrl || !jYggAddr) return env->NewStringUTF("");
   701|    auto original = std::string(env->GetStringUTFChars(jOriginalUrl, nullptr));
   702|    auto ygg = std::string(env->GetStringUTFChars(jYggAddr, nullptr));
   703|    env->ReleaseStringUTFChars(jOriginalUrl, original.c_str());
   704|    env->ReleaseStringUTFChars(jYggAddr, ygg.c_str());
   705|    auto result = progressive::rewriteHomeserverUrl(original, ygg);
   706|    return env->NewStringUTF(result.c_str());
   707|}
   708|
   709|// --- Markdown ---
   710|
   711|JNIEXPORT jstring JNICALL
   712|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMarkdownToHtml(
   713|    JNIEnv* env, jclass, jstring jMarkdown, jboolean jEnableTables
   714|) {
   715|    if (!jMarkdown) return env->NewStringUTF("");
   716|    auto md = std::string(env->GetStringUTFChars(jMarkdown, nullptr));
   717|    env->ReleaseStringUTFChars(jMarkdown, md.c_str());
   718|
   719|    MdConfig config;
   720|    config.enableTables = jEnableTables;
   721|    auto html = progressive::markdownToHtml(md, config);
   722|    return env->NewStringUTF(html.c_str());
   723|}
   724|
   725|JNIEXPORT jstring JNICALL
   726|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseMarkdownTable(
   727|    JNIEnv* env, jclass, jstring jTableBlock, jboolean jWithScroll
   728|) {
   729|    if (!jTableBlock) return env->NewStringUTF("");
   730|    auto block = std::string(env->GetStringUTFChars(jTableBlock, nullptr));
   731|    env->ReleaseStringUTFChars(jTableBlock, block.c_str());
   732|
   733|    auto html = progressive::parseMarkdownTable(block, jWithScroll);
   734|    return env->NewStringUTF(html.c_str());
   735|}
   736|
   737|// --- Account Export ---
   738|
   739|JNIEXPORT jstring JNICALL
   740|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEncryptAccount(
   741|    JNIEnv* env, jclass,
   742|    jstring jUserId, jstring jToken, jstring jRefreshToken,
   743|    jstring jHomeServer, jstring jDeviceId, jstring jDeviceName,
   744|    jstring jDisplayName, jstring jAvatarUrl,
   745|    jboolean jIncludeCache, jstring jPassphrase
   746|) {
   747|    AccountData data;
   748|    data.userId       = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
   749|    data.accessToken  = jToken ? std::string(env->GetStringUTFChars(jToken, nullptr)) : "";
   750|    data.refreshToken = jRefreshToken ? std::string(env->GetStringUTFChars(jRefreshToken, nullptr)) : "";
   751|    data.homeServerUrl = jHomeServer ? std::string(env->GetStringUTFChars(jHomeServer, nullptr)) : "";
   752|    data.deviceId     = jDeviceId ? std::string(env->GetStringUTFChars(jDeviceId, nullptr)) : "";
   753|    data.deviceName   = jDeviceName ? std::string(env->GetStringUTFChars(jDeviceName, nullptr)) : "";
   754|    data.displayName  = jDisplayName ? std::string(env->GetStringUTFChars(jDisplayName, nullptr)) : "";
   755|    data.avatarUrl    = jAvatarUrl ? std::string(env->GetStringUTFChars(jAvatarUrl, nullptr)) : "";
   756|    data.includeCache = jIncludeCache;
   757|
   758|    auto pass = jPassphrase ? std::string(env->GetStringUTFChars(jPassphrase, nullptr)) : "";
   759|
   760|    // Release
   761|    if (jUserId) env->ReleaseStringUTFChars(jUserId, data.userId.c_str());
   762|    if (jToken) env->ReleaseStringUTFChars(jToken, data.accessToken.c_str());
   763|    if (jRefreshToken) env->ReleaseStringUTFChars(jRefreshToken, data.refreshToken.c_str());
   764|    if (jHomeServer) env->ReleaseStringUTFChars(jHomeServer, data.homeServerUrl.c_str());
   765|    if (jDeviceId) env->ReleaseStringUTFChars(jDeviceId, data.deviceId.c_str());
   766|    if (jDeviceName) env->ReleaseStringUTFChars(jDeviceName, data.deviceName.c_str());
   767|    if (jDisplayName) env->ReleaseStringUTFChars(jDisplayName, data.displayName.c_str());
   768|    if (jAvatarUrl) env->ReleaseStringUTFChars(jAvatarUrl, data.avatarUrl.c_str());
   769|    if (jPassphrase) env->ReleaseStringUTFChars(jPassphrase, pass.c_str());
   770|
   771|    auto result = progressive::encryptAccountData(data, pass);
   772|    return env->NewStringUTF(result.c_str());
   773|}
   774|
   775|JNIEXPORT jstring JNICALL
   776|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDecryptAccount(
   777|    JNIEnv* env, jclass, jstring jEncrypted, jstring jPassphrase
   778|) {
   779|    if (!jEncrypted || !jPassphrase) return env->NewStringUTF("");
   780|    auto enc = std::string(env->GetStringUTFChars(jEncrypted, nullptr));
   781|    auto pass = std::string(env->GetStringUTFChars(jPassphrase, nullptr));
   782|    env->ReleaseStringUTFChars(jEncrypted, enc.c_str());
   783|    env->ReleaseStringUTFChars(jPassphrase, pass.c_str());
   784|
   785|    auto data = progressive::decryptAccountData(enc, pass);
   786|    if (data.userId.empty()) return env->NewStringUTF(R"({"error": "Decryption failed"})");
   787|
   788|    auto json = progressive::accountToJson(data);
   789|    return env->NewStringUTF(json.c_str());
   790|}
   791|
   792|// --- Audio ---
   793|
   794|JNIEXPORT jstring JNICALL
   795|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatDuration(
   796|    JNIEnv* env, jclass, jlong jMs
   797|) {
   798|    auto s = progressive::formatDuration(jMs);
   799|    return env->NewStringUTF(s.c_str());
   800|}
   801|
   802|JNIEXPORT jstring JNICALL
   803|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatPositionInfo(
   804|    JNIEnv* env, jclass, jlong jPos, jlong jDur
   805|) {
   806|    auto s = progressive::formatPositionInfo(jPos, jDur);
   807|    return env->NewStringUTF(s.c_str());
   808|}
   809|
   810|JNIEXPORT jfloat JNICALL
   811|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeProgress(
   812|    JNIEnv*, jclass, jlong jPos, jlong jDur
   813|) {
   814|    return progressive::computeProgress(jPos, jDur);
   815|}
   816|
   817|JNIEXPORT jboolean JNICALL
   818|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsSupportedAudio(
   819|    JNIEnv* env, jclass, jstring jMime
   820|) {
   821|    if (!jMime) return JNI_FALSE;
   822|    auto mime = std::string(env->GetStringUTFChars(jMime, nullptr));
   823|    env->ReleaseStringUTFChars(jMime, mime.c_str());
   824|    return progressive::isSupportedAudioType(mime) ? JNI_TRUE : JNI_FALSE;
   825|}
   826|
   827|// --- Media Filter ---
   828|
   829|JNIEXPORT jstring JNICALL
   830|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetFileExtension(
   831|    JNIEnv* env, jclass, jstring jFileName, jstring jMimeType
   832|) {
   833|    auto fn = jFileName ? std::string(env->GetStringUTFChars(jFileName, nullptr)) : "";
   834|    auto mt = jMimeType ? std::string(env->GetStringUTFChars(jMimeType, nullptr)) : "";
   835|    if (jFileName) env->ReleaseStringUTFChars(jFileName, fn.c_str());
   836|    if (jMimeType) env->ReleaseStringUTFChars(jMimeType, mt.c_str());
   837|    auto ext = progressive::getFileExtension(fn, mt);
   838|    return env->NewStringUTF(ext.c_str());
   839|}
   840|
   841|JNIEXPORT jboolean JNICALL
   842|Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidMxcUri(
   843|    JNIEnv* env, jclass, jstring jUri
   844|) {
   845|    if (!jUri) return JNI_FALSE;
   846|    auto uri = std::string(env->GetStringUTFChars(jUri, nullptr));
   847|    env->ReleaseStringUTFChars(jUri, uri.c_str());
   848|    return progressive::isValidMxcUri(uri) ? JNI_TRUE : JNI_FALSE;
   849|}
   850|
   851|} // extern "C"
   852|