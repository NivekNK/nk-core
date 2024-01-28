#================ Default Config ===================
cmake_minimum_required(VERSION 3.24)

project(nk-core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(${PROJECT_NAME} STATIC)

#============== Platform Specific ==================
target_compile_definitions(${PROJECT_NAME}
PRIVATE
    $<$<CONFIG:Debug>:NK_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:NK_RELEASE_DEBUG_INFO>
    $<$<CONFIG:Release>:NK_RELEASE>
)

if (WIN32)
    add_compile_definitions(NK_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
    add_compile_definitions(NK_PLATFORM_LINUX)
endif()

#==================== Sources ======================
target_sources(${PROJECT_NAME}
PRIVATE
    src/test.cpp
)

#==================== Threads ======================
# if(NOT CMAKE_THREAD_PREFER_PTHREAD)
#     set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
# endif()
#
# if(NOT THREADS_PREFER_PTHREAD_FLAG)
#     set(THREADS_PREFER_PTHREAD_FLAG TRUE)
# endif()
#
# find_package(Threads REQUIRED)

#================== Include Files ==================
# target_link_libraries(${PROJECT_NAME}
# PRIVATE
#     Threads::Threads
# )

target_include_directories(${PROJECT_NAME}
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_subdirectory(tests EXCLUDE_FROM_ALL)
