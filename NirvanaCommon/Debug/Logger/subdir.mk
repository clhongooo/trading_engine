################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Logger/Logger.cpp 

OBJS += \
./Logger/Logger.o 

CPP_DEPS += \
./Logger/Logger.d 


# Each subdirectory must supply rules for building sources it contributes
Logger/%.o: ../Logger/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -O0 -g3 -c -fmessage-length=0 -Wfatal-errors -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


