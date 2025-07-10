cmake_minimum_required(VERSION 3.10.0)
project(SDL_Shooter VERSION 0.1.0 LANGUAGES C CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置编译选项
add_compile_options(/W4)

# 设置编译输出
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 没有cmake文件的情况下
# # windows
# if(WIN32)

# include_directories("D:/project_game/project_1/SDL2/include")

# link_directories("D:/project_game/project_1/SDL2/lib/x64")
# elseif(APPLE)
# # macOS
# include_directories("/usr/local/include/SDL2")
# link_directories("/usr/local/lib")

# endif()

set(CMAKE_PREFIX_PATH "SDL2/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(${TARGET} "src/main.cpp" 
                        "src/Game.cpp" 
                        "src/SceneMain.cpp")
# 设置编译选项：使用UTF-8编码
target_compile_options(${TARGET} PRIVATE /utf-8)
target_link_libraries(${TARGET} ${SDL2_LIBRARIES} SDL2_image::SDL2_image 
                    SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf)
