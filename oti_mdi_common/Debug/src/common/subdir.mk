################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/common/ConfigMgr.cpp \
../src/common/Toolbox.cpp 

OBJS += \
./src/common/ConfigMgr.o \
./src/common/Toolbox.o 

CPP_DEPS += \
./src/common/ConfigMgr.d \
./src/common/Toolbox.d 


# Each subdirectory must supply rules for building sources it contributes
src/common/%.o: ../src/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/workspaceCN/oti_mdi_common/include" -I"/home/qy/workspaceCN/loglibrary/include" -I"/home/qy/workspaceCN/oti_mdi_common/src/common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


