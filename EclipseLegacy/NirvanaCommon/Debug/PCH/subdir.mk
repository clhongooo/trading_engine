################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PCH/PCH.cpp 

OBJS += \
./PCH/PCH.o 

CPP_DEPS += \
./PCH/PCH.d 


# Each subdirectory must supply rules for building sources it contributes
PCH/%.o: ../PCH/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/qy/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/workspaceCN/NirvanaCommon/Util" -I"/home/qy/workspaceCN/NirvanaCommon/Logger" -I"/home/qy/workspaceCN/NirvanaCommon/Math" -I"/home/qy/workspaceCN/NirvanaCommon/Model" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -O0 -g3 -c -fmessage-length=0 -Wfatal-errors -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


