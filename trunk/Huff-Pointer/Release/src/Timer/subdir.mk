################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Timer/Timer.cpp 

OBJS += \
./src/Timer/Timer.o 

CPP_DEPS += \
./src/Timer/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Timer/%.o: ../src/Timer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


