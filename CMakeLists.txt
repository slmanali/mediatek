cmake_minimum_required(VERSION 3.10)
project(MediaTekGPSUtils)

set(CMAKE_CXX_STANDARD 17)

# Include Boost for serial communication
find_package(Boost REQUIRED COMPONENTS system)

# Add executable
add_executable(MediaTekGPSUtils
    src/main.cpp
    src/SerialPort.cpp
    src/EPOParser.cpp
    src/GPSCommands.cpp
)

# Link Boost libraries
target_link_libraries(MediaTekGPSUtils PRIVATE Boost::system)