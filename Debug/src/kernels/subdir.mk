################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CL_SRCS += \
../src/kernels/image.cl 

CL_DEPS += \
./src/kernels/image.d 

OBJS += \
./src/kernels/image.o 


# Each subdirectory must supply rules for building sources it contributes
src/kernels/%.o: ../src/kernels/%.cl
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


