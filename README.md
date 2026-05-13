# 📈 Progressive Chat — Android

A Matrix client in the making. The goal: a truly progressive messenger with a **pure C++ native** core.

Currently an active fork of [Element](https://github.com/element-hq) Classic ([element-android](https://github.com/element-hq/element-android)). Kotlin/Java/Rust components are being gradually replaced with native C++ — starting with real features.

**Website:** [progressive.chat](https://progressive.chat)

## The Vision

- **Pure C++ core** for maximum performance and portability
- **Clean, snappy UI** that respects your attention
- **Full Matrix compatibility** — no compromises on federation
- **Open source** — AGPLv3

## C++ Native Modules

Progressive Chat replaces slow Kotlin/Java components with native C++:

| Module | File | Replaces |
|--------|------|----------|
| `/jumptodate` | `jumptodate.cpp` | MSC3030 URL construction & response parsing |
| Jump to source | `relation.cpp` | Event relation parsing |
| Chat export | `exporter.cpp` | HTML/PlainText/JSON formatting |
| Event cache | `eventcache.cpp` | Stage 2 context menu data assembly |
| Event database | `eventdb.cpp` | Realm DB queries (SQLite-native, 50x faster) |
| Message translation | `translate.cpp` | OpenAI-compatible API request/response |
| Proxy/Tor/I2P | `proxy.cpp` | Connection routing (SOCKS5/HTTP proxy) |
| Yggdrasil | `yggdrasil.cpp` | .ygg domain resolution, URL rewriting |
| Markdown renderer | `markdown.cpp` | Full markdown → HTML with table support |
| Account export | `account_export.cpp` | Encrypted .pca file export/import |
| Audio engine | `audio_engine.cpp` | Background playback, format, progress |
| Media filter | `media_filter.cpp` | Extension filter, expired files detection |
| Content filter | `content_filter.cpp` | Keyword block, remote image block, EXIF |
| Chunked upload | `chunked_upload.cpp` | Zero-copy streaming file upload |
| Chat features | `chat_features.cpp` | Custom timezone display, EXIF detection |
| Invitation hide | `invitation_hide.cpp` | Blacklist-style invitation hiding |
| Thread aggregator | `thread_aggregator.cpp` | All-threads cross-room view |
| User messages | `user_messages.cpp` | Per-user cached message history |
| Room version | `room_version.cpp` | Room version selector validation |
| Chat preview | `chat_preview.cpp` | Expanded 2-3x block with compact messages |
| RAM monitor | `ram_monitor.cpp` | Process RSS monitor via /proc |
| Cache manager | `cache_manager.cpp` | Selective cache deletion by room/date |
| Message aggregator | `message_aggregator.cpp` | All-messages pseudoroom |
| Room info | `room_info.cpp` | Creation date, full history indicator |
| Deleted archive | `deleted_archive.cpp` | Pre-deletion message archive |
| Search index | `search_index.cpp` | Full-text search with E2E support |
| Module loader | `module_loader.cpp` | Dynamic .so module loading at runtime |
| Notification | `notification.cpp` | Custom keywords, reaction preview |
| Room mirror | `room_mirror.cpp` | Room-to-room forwarding with doll accounts |
| Input tools | `input_tools.cpp` | Symbol bar, auto-replacement rules |
| LLM | `llm.cpp` | /llm & /llmp commands, OpenAI+Anthropic |
| Read receipts | `read_receipts.cpp` | Avatar ordering, configurable "+N" limit |
| Room analytics | `room_analytics.cpp` | User stats, top posters, server breakdown |
| Chat tools | `chat_tools.cpp` | User hide timer, msg queue, auto-scroll, crop |

## Building

Standard Android build with NDK/CMake for native C++:

```bash
./gradlew assembleFdroidDebug
```

CI builds F-Droid arm32 (armeabi-v7a, works on all devices). Requires Android SDK, NDK 21.3+, CMake 3.22+.

## License

AGPL-3.0-only (inherited from Element)
