################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/order/ATU_Abstract_OTI.cpp \
../src/order/ParsedMarketData.cpp 

OBJS += \
./src/order/ATU_Abstract_OTI.o \
./src/order/ParsedMarketData.o 

CPP_DEPS += \
./src/order/ATU_Abstract_OTI.d \
./src/order/ParsedMarketData.d 


# Each subdirectory must supply rules for building sources it contributes
src/order/%.o: ../src/order/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/workspaceCN/oti_mdi_common/include" -I"/home/qy/workspaceCN/loglibrary/include" -I"/home/qy/workspaceCN/oti_mdi_common/src/common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


