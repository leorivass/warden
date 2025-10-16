cmake_minimum_required(VERSION 3.16)
PROJECT(scheduler C)

set(MCU atmega2560)
set(F_CPU 16000000UL)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os")

set(SOURCES 
    mcu/main.c
	mcu/src/usart.c
)

include_directories(${CMAKE_SOURCE_DIR}/mcu/includes)

add_executable(main.helf ${SOURCES})

add_custom_target(create_hex ALL
	COMMAND avr-objcopy -O ihex main.helf main.hex
	DEPENDS main.helf
)

add_custom_target(flash
	COMMAND avrdude -v -c wiring -p m2560 -P /dev/ttyACM0 -U flash:w:main.hex -D
	DEPENDS main.hex
)