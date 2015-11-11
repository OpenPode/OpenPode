################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Hexapode.cpp \
../src/PCA9685.cpp \
../src/Paw.cpp \
../src/Paw_math_model.cpp \
../src/Servo.cpp \
../src/Side.cpp \
../src/i2cdev.cpp \
../src/main.cpp \
../src/time_t.cpp \
../src/timer_t.cpp 

OBJS += \
./src/Hexapode.o \
./src/PCA9685.o \
./src/Paw.o \
./src/Paw_math_model.o \
./src/Servo.o \
./src/Side.o \
./src/i2cdev.o \
./src/main.o \
./src/time_t.o \
./src/timer_t.o 

CPP_DEPS += \
./src/Hexapode.d \
./src/PCA9685.d \
./src/Paw.d \
./src/Paw_math_model.d \
./src/Servo.d \
./src/Side.d \
./src/i2cdev.d \
./src/main.d \
./src/time_t.d \
./src/timer_t.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


