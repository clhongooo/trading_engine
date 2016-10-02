################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/contract_manager/AtuContract.cpp \
../src/contract_manager/ContractManager.cpp \
../src/contract_manager/IniContractReader.cpp \
../src/contract_manager/JsonContractReader.cpp 

OBJS += \
./src/contract_manager/AtuContract.o \
./src/contract_manager/ContractManager.o \
./src/contract_manager/IniContractReader.o \
./src/contract_manager/JsonContractReader.o 

CPP_DEPS += \
./src/contract_manager/AtuContract.d \
./src/contract_manager/ContractManager.d \
./src/contract_manager/IniContractReader.d \
./src/contract_manager/JsonContractReader.d 


# Each subdirectory must supply rules for building sources it contributes
src/contract_manager/%.o: ../src/contract_manager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/workspaceCN/oti_mdi_common/include" -I"/home/qy/workspaceCN/loglibrary/include" -I"/home/qy/workspaceCN/oti_mdi_common/src/common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


