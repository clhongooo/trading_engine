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
	g++ -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommonTest/UnitTest" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/DataAggregation" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/Facilities" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/OrderExecutionAlgo" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/PortfolioGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/ProbDistributionGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/TechIndUpdating" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/VolSurfaceCalculation" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


