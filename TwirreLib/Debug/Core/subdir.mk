################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Actuator.cpp \
../Core/Device.cpp \
../Core/Sensor.cpp \
../Core/Value.cpp 

OBJS += \
./Core/Actuator.o \
./Core/Device.o \
./Core/Sensor.o \
./Core/Value.o 

CPP_DEPS += \
./Core/Actuator.d \
./Core/Device.d \
./Core/Sensor.d \
./Core/Value.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


