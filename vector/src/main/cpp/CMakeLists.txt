cmake_minimum_required(VERSION 3.22.1)
project("progressive_native")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Download SQLite3 amalgamation at configure time ---
set(SQLITE3_DIR "${CMAKE_BINARY_DIR}/sqlite-amalgamation-3450200")
set(SQLITE3_SRC "${SQLITE3_DIR}/sqlite3.c")
if(NOT EXISTS "${SQLITE3_SRC}")
    message(STATUS "Downloading SQLite3 amalgamation from sqlite.org...")
    file(DOWNLOAD
        "https://sqlite.org/2024/sqlite-amalgamation-3450200.zip"
        "${CMAKE_BINARY_DIR}/sqlite3.zip"
        STATUS download_status
    )
    list(GET download_status 0 download_code)
    if(NOT download_code EQUAL 0)
        message(FATAL_ERROR "Failed to download SQLite: ${download_status}")
    endif()
    message(STATUS "Extracting SQLite amalgamation...")
    file(MAKE_DIRECTORY "${SQLITE3_DIR}")
    execute_process(
        COMMAND python3 -c "import zipfile,sys; zipfile.ZipFile(sys.argv[1]).extractall(sys.argv[2])" "${CMAKE_BINARY_DIR}/sqlite3.zip" "${CMAKE_BINARY_DIR}"
        RESULT_VARIABLE extract_result
    )
    if(NOT extract_result EQUAL 0)
        message(FATAL_ERROR "SQLite extraction failed")
    endif()
    if(NOT EXISTS "${SQLITE3_SRC}")
        message(FATAL_ERROR "SQLite extraction failed — sqlite3.c not found")
    endif()
    message(STATUS "SQLite3 amalgamation extracted to ${SQLITE3_DIR}")
endif()

add_library(
    progressive_native
    SHARED
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
    src/jni_bridge.cpp
    ${SQLITE3_SRC}
)

target_include_directories(
    progressive_native
    PUBLIC
    include
    ${SQLITE3_DIR}
)

set_source_files_properties(
    "${SQLITE3_SRC}"
    PROPERTIES COMPILE_FLAGS
    "-DSQLITE_THREADSAFE=1 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1 -DSQLITE_LIKE_DOESNT_MATCH_BLOBS -DSQLITE_MAX_EXPR_DEPTH=0 -DSQLITE_OMIT_DECLTYPE -DSQLITE_OMIT_DEPRECATED -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_SHARED_CACHE -DSQLITE_USE_ALLOCA -Os"
)

find_library(log_lib log)

target_link_libraries(
    progressive_native
    ${log_lib}
)
