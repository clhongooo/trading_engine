################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Main.cpp 

OBJS += \
./Main.o 

CPP_DEPS += \
./Main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/workspaceCN/NirvanaCommon" -I"/home/qy/workspaceCN/NirvanaInfrastructure" -I"/home/qy/workspaceCN/AutomatedTests/UnitTest" -I"/home/qy/workspaceCN/NirvanaInfrastructure/DataAggregation" -I"/home/qy/workspaceCN/NirvanaInfrastructure/Facilities" -I"/home/qy/workspaceCN/NirvanaInfrastructure/OrderExecutionAlgo" -I"/home/qy/workspaceCN/NirvanaInfrastructure/ProbDistributionGeneration" -I"/home/qy/workspaceCN/NirvanaInfrastructure/SharedObjects" -I"/home/qy/workspaceCN/NirvanaInfrastructure/TechIndUpdating" -I"/home/qy/workspaceCN/NirvanaInfrastructure/VolSurfaceCalculation" -I"/home/qy/workspaceCN/NirvanaCommon/Logger" -I"/home/qy/workspaceCN/NirvanaCommon/Math" -I"/home/qy/workspaceCN/NirvanaCommon/Model" -I"/home/qy/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/workspaceCN/NirvanaCommon/Util" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


