################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Nirvana/HSIContFut.cpp \
../Nirvana/SubInterval.cpp 

OBJS += \
./Nirvana/HSIContFut.o \
./Nirvana/SubInterval.o 

CPP_DEPS += \
./Nirvana/HSIContFut.d \
./Nirvana/SubInterval.d 


# Each subdirectory must supply rules for building sources it contributes
Nirvana/%.o: ../Nirvana/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/PCH" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


