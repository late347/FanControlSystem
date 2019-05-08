################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DigitalIoPin.cpp \
../src/EventQueue.cpp \
../src/FanControl.cpp \
../src/I2C.cpp \
../src/LimitedCounter.cpp \
../src/LiquidCrystal.cpp \
../src/ModbusMaster.cpp \
../src/Printer.cpp \
../src/SerialPort.cpp \
../src/StateMachine.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc15xx.cpp \
../src/esp_project_abb_modbus.cpp 

C_SRCS += \
../src/ITM_write.c \
../src/crp.c \
../src/retarget_itm.c \
../src/sysinit.c 

OBJS += \
./src/DigitalIoPin.o \
./src/EventQueue.o \
./src/FanControl.o \
./src/I2C.o \
./src/ITM_write.o \
./src/LimitedCounter.o \
./src/LiquidCrystal.o \
./src/ModbusMaster.o \
./src/Printer.o \
./src/SerialPort.o \
./src/StateMachine.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc15xx.o \
./src/crp.o \
./src/esp_project_abb_modbus.o \
./src/retarget_itm.o \
./src/sysinit.o 

CPP_DEPS += \
./src/DigitalIoPin.d \
./src/EventQueue.d \
./src/FanControl.d \
./src/I2C.d \
./src/LimitedCounter.d \
./src/LiquidCrystal.d \
./src/ModbusMaster.d \
./src/Printer.d \
./src/SerialPort.d \
./src/StateMachine.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc15xx.d \
./src/esp_project_abb_modbus.d 

C_DEPS += \
./src/ITM_write.d \
./src/crp.d \
./src/retarget_itm.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=c++11 -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Arvi\Documents\MCUXpressoIDE_10.1.1_606\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Arvi\Documents\MCUXpressoIDE_10.1.1_606\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Arvi\Documents\MCUXpressoIDE_10.1.1_606\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Arvi\Documents\MCUXpressoIDE_10.1.1_606\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


