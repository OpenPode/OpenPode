################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Hexapode.cpp \
../src/Paw.cpp \
../src/Paw_math_model.cpp 

OBJS += \
./src/Hexapode.o \
./src/Paw.o \
./src/Paw_math_model.o 

CPP_DEPS += \
./src/Hexapode.d \
./src/Paw.d \
./src/Paw_math_model.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


