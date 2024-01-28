#================ Default Config ===================
cmake_minimum_required(VERSION 3.24)

project(nk-core-tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (UNIX AND NOT APPLE)
    set(OPERATIVE_SYSTEM "Linux")
elseif(WIN32)
    set(OPERATIVE_SYSTEM "Win32")
else()
    message(FATAL_ERROR "Operative system not supported!")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin/${OPERATIVE_SYSTEM}-${CMAKE_BUILD_TYPE}/tests")

enable_testing()

add_executable(${PROJECT_NAME})

#============== Platform Specific ==================
target_compile_definitions(${PROJECT_NAME}
PRIVATE
    $<$<CONFIG:Debug>:NK_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:NK_RELEASE_DEBUG_INFO>
    $<$<CONFIG:Release>:NK_RELEASE>
    NK_TESTING=1
)

if (UNIX AND NOT APPLE)
    add_compile_definitions(NK_PLATFORM_LINUX)
elseif (WIN32)
    add_compile_definitions(NK_PLATFORM_WINDOWS)
endif()

set(BUILD_SHARED_LIBS_OLD ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(vendor/googletest)

set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_OLD} CACHE BOOL "" FORCE)

#==================== Sources ======================
target_sources(${PROJECT_NAME}
PRIVATE
    src/main.cpp
    src/test/test.cpp
)

#==================== Threads ======================
if(NOT CMAKE_THREAD_PREFER_PTHREAD)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
endif()

if(NOT THREADS_PREFER_PTHREAD_FLAG)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
endif()

find_package(Threads REQUIRED)

#================== Include Files ==================
target_link_libraries(${PROJECT_NAME}
PRIVATE
    Threads::Threads
    GTest::gtest_main
    nk-core
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    ../include/
    ../src/
)

#===================== Tests ======================
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
