cmake_minimum_required(VERSION 3.7)
project(SHA256)


set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/SHA256.cpp
)

source_group("src" FILES ${PROJECT_SRCS})

find_package(PkgConfig REQUIRED)

if(APPLE)
  set(ENV{PKG_CONFIG_PATH}  "/usr/local/opt/openssl/lib/pkgconfig")
endif()

pkg_check_modules(OPENSSL REQUIRED OpenSSL)

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${OPENSSL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${CMAKE_PROJECT_NAME}/apps)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION bin)
