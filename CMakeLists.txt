cmake_minimum_required(VERSION 3.16)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

project(dbus_tutorial LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(GLIB 2.00.0 REQUIRED gio gobject)
find_package(DBus REQUIRED)
find_package(Threads REQUIRED)

file(GLOB_RECURSE SOURCES "nm_cli/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLIB_INCLUDE_DIRS}
    ${DBUS_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GLIB_LIBRARIES}
    ${DBUS_LIBRARIES}
    ${GLIB_GIO_LIBRARIES}
    ${GLIB_GOBJECT_LIBRARIES}
    Threads::Threads)
