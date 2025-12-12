cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)

project(SEGGER_RTT)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_SOURCE_DIR}/Middleware/segger_rtt/Inc
)

target_sources(${PROJECT_NAME} INTERFACE
    ${CMAKE_SOURCE_DIR}/Middleware/segger_rtt/Src/SEGGER_RTT.c
    ${CMAKE_SOURCE_DIR}/Middleware/segger_rtt/Src/SEGGER_RTT_printf.c
    ${CMAKE_SOURCE_DIR}/Middleware/segger_rtt/Src/SEGGER_RTT_ASM_ARMv7M.s
)