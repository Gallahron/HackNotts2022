cmake_minimum_required(VERSION 3.0.0)
project(HN22-Client VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# enable pkg-config
find_package(PkgConfig REQUIRED)

# enable pthreads
find_package(Threads)

# get modules with pkg-config
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2 SDL2_ttf SDL2_image SDL2_net)

# add source files
add_executable(HN22-Client
	"${PROJECT_SOURCE_DIR}/src/main.c"
	"${PROJECT_SOURCE_DIR}/src/log.c"
	"${PROJECT_SOURCE_DIR}/src/game.c"
	"${PROJECT_SOURCE_DIR}/src/state.c"
	"${PROJECT_SOURCE_DIR}/src/render.c"
	"${PROJECT_SOURCE_DIR}/src/listener.c"
	"${PROJECT_SOURCE_DIR}/src/parse.c"
	"${PROJECT_SOURCE_DIR}/src/map.c"
	"${PROJECT_SOURCE_DIR}/src/input.c"
)

# link libraries
target_link_libraries(HN22-Client
	PRIVATE PkgConfig::SDL2
	PRIVATE Threads::Threads
)

# set compile options
target_compile_options(HN22-Client PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)