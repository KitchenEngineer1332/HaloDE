cmake_minimum_required(VERSION 3.20)
project(HaloDE VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Compiler flags
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Dependencies (using pkg-config)
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND_SERVER REQUIRED wayland-server)
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
pkg_check_modules(WLROOTS REQUIRED wlroots>=0.17.0)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
pkg_check_modules(LIBINPUT REQUIRED libinput)
pkg_check_modules(PIXMAN REQUIRED pixman-1)

# Add subdirectories
# add_subdirectory(protocols)
# add_subdirectory(libs)
# add_subdirectory(compositor)
# add_subdirectory(shell)
# add_subdirectory(services)
# add_subdirectory(plugins)
# add_subdirectory(testing)
