cmake_minimum_required(VERSION 3.22.1)
project("progressive_native")
set(CMAKE_CXX_STANDARD 20)
set(SQLITE3_DIR "${CMAKE_BINARY_DIR}/sqlite-amalgamation-3450200")
set(SQLITE3_SRC "${SQLITE3_DIR}/sqlite3.c")
if(NOT EXISTS "${SQLITE3_SRC}")
    file(DOWNLOAD "https://sqlite.org/2024/sqlite-amalgamation-3450200.zip" "${CMAKE_BINARY_DIR}/sqlite3.zip" STATUS s); list(GET s 0 c)
    if(NOT c EQUAL 0) message(FATAL_ERROR "sqlite dl fail") endif()
    file(MAKE_DIRECTORY "${SQLITE3_DIR}")
    exec_program(python3 ARGS "-c" "import zipfile,sys;zipfile.ZipFile(sys.argv[1]).extractall(sys.argv[2])" "${CMAKE_BINARY_DIR}/sqlite3.zip" "${CMAKE_BINARY_DIR}" RETURN_VALUE r)
    if(NOT r EQUAL 0) message(FATAL_ERROR "sqlite extract fail") endif()
endif()
add_library(progressive_native SHARED
        src/jumptodate.cpp
        src/json_parser.cpp
        src/relation.cpp
        src/exporter.cpp
        src/eventcache.cpp
        src/eventdb.cpp
        src/translate.cpp
        src/proxy.cpp
        src/yggdrasil.cpp
        src/markdown.cpp
        src/account_export.cpp
        src/audio_engine.cpp
        src/media_filter.cpp
        src/content_filter.cpp
        src/network_stats.cpp
        src/masquerade.cpp
        src/user_mask.cpp
        src/chunked_upload.cpp
        src/chat_features.cpp
        src/invitation_hide.cpp
        src/thread_aggregator.cpp
        src/user_messages.cpp
        src/room_version.cpp
        src/chat_preview.cpp
        src/ram_monitor.cpp
        src/cache_manager.cpp
        src/message_aggregator.cpp
        src/room_info.cpp
        src/deleted_archive.cpp
        src/search_index.cpp
        src/module_loader.cpp
        src/notification.cpp
        src/room_mirror.cpp
        src/input_tools.cpp
        src/llm.cpp
        src/jni_bridge.cpp
    ${SQLITE3_SRC})
target_include_directories(progressive_native PUBLIC include ${SQLITE3_DIR})
set_source_files_properties("${SQLITE3_SRC}" PROPERTIES COMPILE_FLAGS "-DSQLITE_THREADSAFE=1 -DSQLITE_OMIT_LOAD_EXTENSION -Os")
find_library(log_lib log)
target_link_libraries(progressive_native ${log_lib})
