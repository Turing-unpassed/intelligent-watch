cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

include(${CMAKE_SOURCE_DIR}/cmake/find_header.cmake)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME intelligent-watch)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
add_subdirectory(cmake/SEGGER_RTT)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

file(GLOB_RECURSE SOURCES
    "Hardware/*.c"
    "Hardware/*.cpp"
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${SOURCES}
)

include_sub_directories_recursively(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Hardware)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
    LOG_ENABLE
    LOG_INFO_ENABLE
    LOG_WARNING_ENABLE
    LOG_ERROR_ENABLE
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    SEGGER_RTT
    # Add user defined libraries
)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD # 指定自定义构建的对象是.elf
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex # 使用objcopy工具将elf文件格式转换成hex文件
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin # 使用ogjcopy工具将elf文件转换为二进制格式
    COMMENT "EXCUTABLE SIZE:"
    COMMAND ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf # 使用size工具显示elf文件的大小信息
)

add_custom_target(Download_Jlink 
    COMMAND JFlash -openprj"${CMAKE_SOURCE_DIR}/ToolChain/stm32f103.jflash" -open"${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex",0x08000000 -auto -startapp -exit
    DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex
    COMMENT "Download ${CMAKE_PROJECT_NAME}.hex to target device"
)

add_custom_target(Download_Daplink 
    COMMAND openocd -f ${CMAKE_SOURCE_DIR}/Toolchain/openocd_dap.cfg -c "program ${workspaceFolder}/build/intelligent-watch.bin verify reset exit"
    DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    COMMENT "Download ${CMAKE_PROJECT_NAME}.bin to target device"
)
