################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/PortfolioOptimization/PortfolioOptimizer.cpp 

OBJS += \
./src/PortfolioOptimization/PortfolioOptimizer.o 

CPP_DEPS += \
./src/PortfolioOptimization/PortfolioOptimizer.d 


# Each subdirectory must supply rules for building sources it contributes
src/PortfolioOptimization/%.o: ../src/PortfolioOptimization/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D '__SVN_REV__="$(shell svnversion -n /home/qy/Dropbox/nirvana/workspaceCN/TradingEngine)"' -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/include" -I"/home/qy/Dropbox/nirvana/workspaceCN/oti_mdi_common/include" -I"/home/qy/Dropbox/nirvana/workspaceCN/loglibrary/include" -I"/home/qy/Dropbox/nirvana/workspaceCN/atu_tcp_library/include" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/DataAggregation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/OrderExecutionAlgo" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/PortfolioOptimization" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/ProbDistribution" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/VolSurfaceCalculation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/TechIndUpdating" -O3 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


