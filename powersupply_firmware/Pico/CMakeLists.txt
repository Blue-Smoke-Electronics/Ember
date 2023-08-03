# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(BlueSmoke C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# enable try/catch execeptions
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

FILE(GLOB src src/*.c)
# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Display.cpp
    src/Flash.cpp
    src/Sprite.cpp
    src/GUI.cpp
    src/Onoff.cpp
    src/Heartbeat.cpp
    src/Font.cpp
    src/CommandLineInterface.cpp
    src/PSU.cpp
    src/LinReg.cpp
    src/Booster.cpp
    src/Analog.cpp
    src/Knobs.cpp
    src/Battery.cpp
    src/Overheat.cpp
    src/Powersaver.cpp
    src/SmallBoosters.cpp
)

# Create C header file with the name <pio program>.pio.h
#pico_generate_pio_header(${PROJECT_NAME}  
        #${CMAKE_CURRENT_LIST_DIR}/test.pio
#)

# Create map/bin/hex/uf2 files
#pico_add_extra_outputs(${PROJECT_NAME})


target_include_directories(${PROJECT_NAME} PUBLIC 

    include/
    flash_data/
    

)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_pio
    hardware_pwm
    hardware_spi
    hardware_adc
    hardware_flash
    hardware_dma
    hardware_watchdog
    #FreeRTOS
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME} )
