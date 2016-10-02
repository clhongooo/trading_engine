################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/marketdata/ATU_Abstract_MDI.cpp 

OBJS += \
./src/marketdata/ATU_Abstract_MDI.o 

CPP_DEPS += \
./src/marketdata/ATU_Abstract_MDI.d 


# Each subdirectory must supply rules for building sources it contributes
src/marketdata/%.o: ../src/marketdata/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/workspaceCN/oti_mdi_common/include" -I"/home/qy/workspaceCN/loglibrary/include" -I"/home/qy/workspaceCN/oti_mdi_common/src/common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


