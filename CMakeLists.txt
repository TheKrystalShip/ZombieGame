cmake_minimum_required(VERSION 3.28)
project(zombie VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-but-set-variable")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SOURCES src/main.cpp
            src/Agent.cpp
            src/Bullet.cpp
            src/Gun.cpp
            src/Human.cpp
            src/Level.cpp
            src/MainGame.cpp
            src/Player.cpp
            src/Zombie.cpp
)

add_executable(zombie ${SOURCES})

target_include_directories(zombie PUBLIC include ${TOASTER_INCLUDES})
target_link_libraries(zombie ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} toaster)

install(TARGETS zombie RUNTIME DESTINATION bin)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME      ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION   ${PROJECT_VERSION})
include(CPack)
