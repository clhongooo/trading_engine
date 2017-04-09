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
	g++ -std=c++0x -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaMath/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaMath/Model" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/DataAggregation" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/Facilities" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/OrderExecutionAlgo" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/PortfolioGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/ProbDistributionGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/TechIndUpdating" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/VolSurfaceCalculation" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -I"/home/qy/Dropbox/nirvana/workspaceCN/atu_tcp_library/include" -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


