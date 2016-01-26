################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TechIndUpdating/TechIndUpdater.cpp \
../src/TechIndUpdating/TechIndUpdater_HKFE.cpp \
../src/TechIndUpdating/TechIndUpdater_HKSE.cpp 

OBJS += \
./src/TechIndUpdating/TechIndUpdater.o \
./src/TechIndUpdating/TechIndUpdater_HKFE.o \
./src/TechIndUpdating/TechIndUpdater_HKSE.o 

CPP_DEPS += \
./src/TechIndUpdating/TechIndUpdater.d \
./src/TechIndUpdating/TechIndUpdater_HKFE.d \
./src/TechIndUpdating/TechIndUpdater_HKSE.d 


# Each subdirectory must supply rules for building sources it contributes
src/TechIndUpdating/%.o: ../src/TechIndUpdating/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D '__SVN_REV__="$(shell svnversion -n /home/qy/Dropbox/nirvana/workspaceCN/TradingEngine)"' -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/include" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/DataAggregation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/OrderExecutionAlgo" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/PortfolioGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/ProbDistributionGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/TechIndUpdating" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/VolSurfaceCalculation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Facilities" -I"/home/qy/Dropbox/nirvana/workspaceCN/atu_tcp_library/include" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


