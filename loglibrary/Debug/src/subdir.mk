################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ATU_Feed_Logger.cpp \
../src/ATU_Logger.cpp \
../src/ATU_Timer.cpp \
../src/LogStream.cpp 

OBJS += \
./src/ATU_Feed_Logger.o \
./src/ATU_Logger.o \
./src/ATU_Timer.o \
./src/LogStream.o 

CPP_DEPS += \
./src/ATU_Feed_Logger.d \
./src/ATU_Logger.d \
./src/ATU_Timer.d \
./src/LogStream.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/Dropbox/nirvana/workspaceCN/loglibrary/include" -I"/home/qy/Dropbox/nirvana/workspaceCN/loglibrary/src" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


