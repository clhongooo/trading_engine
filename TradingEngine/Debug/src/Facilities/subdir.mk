################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Facilities/MarkToMarket.cpp 

OBJS += \
./src/Facilities/MarkToMarket.o 

CPP_DEPS += \
./src/Facilities/MarkToMarket.d 


# Each subdirectory must supply rules for building sources it contributes
src/Facilities/%.o: ../src/Facilities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D '__SVN_REV__="$(shell svnversion -n /home/qy/Dropbox/nirvana/workspaceCN/TradingEngine)"' -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/include" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/DataAggregation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/OrderExecutionAlgo" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/PortfolioGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/ProbDistributionGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/TechIndUpdating" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/VolSurfaceCalculation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Facilities" -I"/home/qy/Dropbox/nirvana/workspaceCN/atu_tcp_library/include" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


