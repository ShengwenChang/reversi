cmake_minimum_required(VERSION 3.15)

# Sets target executable variable
set(TARGET "reversi_desktop")


# Sets project name
project(${TARGET} LANGUAGES C CXX)

# Verifies dependencies
find_package(SDL3 REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Sets source files variable
file(GLOB SOURCES 
    "src/*.cpp"
    "src/*.c"
)

# Adds target exe from source
add_executable(${TARGET} ${SOURCES})
target_include_directories(${TARGET} PRIVATE "src/include")

# Links dependencies
target_link_libraries(${TARGET} PRIVATE SDL3::SDL3)
target_link_libraries(${TARGET} PRIVATE imgui::imgui)

# Adds C++ 17 standard
target_compile_features(${TARGET} PRIVATE cxx_std_17)