################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../LowPriorityFacilities/PriceForwarderToNextTier.cpp 

OBJS += \
./LowPriorityFacilities/PriceForwarderToNextTier.o 

CPP_DEPS += \
./LowPriorityFacilities/PriceForwarderToNextTier.d 


# Each subdirectory must supply rules for building sources it contributes
LowPriorityFacilities/%.o: ../LowPriorityFacilities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/qy/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/workspaceCN/NirvanaCommon/Math" -I"/home/qy/workspaceCN/NirvanaCommon/Model" -I"/home/qy/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/workspaceCN/NirvanaInfrastructure/DataAggregation" -I"/home/qy/workspaceCN/NirvanaInfrastructure/Facilities" -I"/home/qy/workspaceCN/NirvanaInfrastructure/OrderExecutionAlgo" -I"/home/qy/workspaceCN/NirvanaInfrastructure/SharedObjects" -I"/home/qy/workspaceCN/NirvanaInfrastructure/Strategies" -I"/home/qy/workspaceCN/NirvanaInfrastructure/StrategyTests" -I"/home/qy/workspaceCN/NirvanaInfrastructure/ProbDistributionGeneration" -I"/home/qy/workspaceCN/NirvanaInfrastructure/TechIndUpdating" -I"/home/qy/workspaceCN/NirvanaInfrastructure/VolSurfaceCalculation" -I"/home/qy/workspaceCN/NirvanaInfrastructure/LowPriorityFacilities" -I"/home/qy/workspaceCN/NirvanaCommon/Util" -I"/home/qy/workspaceCN/NirvanaCommon/Logger" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -I"/home/qy/workspaceCN/atu_tcp_library/include" -O0 -g3 -c -fmessage-length=0 -Wfatal-errors -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


