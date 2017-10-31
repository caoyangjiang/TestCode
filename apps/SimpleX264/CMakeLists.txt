cmake_minimum_required(VERSION 3.7)
project(SimpleX264)

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/SimpleX264.cpp
)

source_group("src" FILES ${PROJECT_SRCS})

find_package( PkgConfig REQUIRED )

if(APPLE)
  set(ENV{PKG_CONFIG_PATH}  "/usr/local/opt/x264/lib/pkgconfig")
endif()

pkg_check_modules( X264 REQUIRED x264 )

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${X264_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${X264_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${CMAKE_PROJECT_NAME}/apps)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION bin)
