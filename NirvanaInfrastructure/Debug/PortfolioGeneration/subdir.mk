################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PortfolioGeneration/PortfolioGenerator.cpp \
../PortfolioGeneration/StrategyA1.cpp \
../PortfolioGeneration/StrategyA6.cpp \
../PortfolioGeneration/StrategyB1.cpp \
../PortfolioGeneration/StrategyB1HKF.cpp \
../PortfolioGeneration/StrategyB2.cpp \
../PortfolioGeneration/StrategyB2HK.cpp \
../PortfolioGeneration/StrategyB2US1.cpp \
../PortfolioGeneration/StrategyB2US2.cpp \
../PortfolioGeneration/StrategyB2US3.cpp \
../PortfolioGeneration/StrategyBase.cpp \
../PortfolioGeneration/StrategyETFR.cpp \
../PortfolioGeneration/StrategyNIR1.cpp \
../PortfolioGeneration/StrategyR1.cpp \
../PortfolioGeneration/StrategyR3.cpp \
../PortfolioGeneration/StrategyR7.cpp \
../PortfolioGeneration/StrategyR8.cpp \
../PortfolioGeneration/StrategyR9.cpp \
../PortfolioGeneration/StrategyS11A.cpp \
../PortfolioGeneration/StrategyS13.cpp \
../PortfolioGeneration/StrategyTest.cpp 

OBJS += \
./PortfolioGeneration/PortfolioGenerator.o \
./PortfolioGeneration/StrategyA1.o \
./PortfolioGeneration/StrategyA6.o \
./PortfolioGeneration/StrategyB1.o \
./PortfolioGeneration/StrategyB1HKF.o \
./PortfolioGeneration/StrategyB2.o \
./PortfolioGeneration/StrategyB2HK.o \
./PortfolioGeneration/StrategyB2US1.o \
./PortfolioGeneration/StrategyB2US2.o \
./PortfolioGeneration/StrategyB2US3.o \
./PortfolioGeneration/StrategyBase.o \
./PortfolioGeneration/StrategyETFR.o \
./PortfolioGeneration/StrategyNIR1.o \
./PortfolioGeneration/StrategyR1.o \
./PortfolioGeneration/StrategyR3.o \
./PortfolioGeneration/StrategyR7.o \
./PortfolioGeneration/StrategyR8.o \
./PortfolioGeneration/StrategyR9.o \
./PortfolioGeneration/StrategyS11A.o \
./PortfolioGeneration/StrategyS13.o \
./PortfolioGeneration/StrategyTest.o 

CPP_DEPS += \
./PortfolioGeneration/PortfolioGenerator.d \
./PortfolioGeneration/StrategyA1.d \
./PortfolioGeneration/StrategyA6.d \
./PortfolioGeneration/StrategyB1.d \
./PortfolioGeneration/StrategyB1HKF.d \
./PortfolioGeneration/StrategyB2.d \
./PortfolioGeneration/StrategyB2HK.d \
./PortfolioGeneration/StrategyB2US1.d \
./PortfolioGeneration/StrategyB2US2.d \
./PortfolioGeneration/StrategyB2US3.d \
./PortfolioGeneration/StrategyBase.d \
./PortfolioGeneration/StrategyETFR.d \
./PortfolioGeneration/StrategyNIR1.d \
./PortfolioGeneration/StrategyR1.d \
./PortfolioGeneration/StrategyR3.d \
./PortfolioGeneration/StrategyR7.d \
./PortfolioGeneration/StrategyR8.d \
./PortfolioGeneration/StrategyR9.d \
./PortfolioGeneration/StrategyS11A.d \
./PortfolioGeneration/StrategyS13.d \
./PortfolioGeneration/StrategyTest.d 


# Each subdirectory must supply rules for building sources it contributes
PortfolioGeneration/%.o: ../PortfolioGeneration/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/DataAggregation" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/Facilities" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/OrderExecutionAlgo" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/PortfolioGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/ProbDistributionGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/TechIndUpdating" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/VolSurfaceCalculation" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/LowPriorityFacilities" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Logger" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -I"/home/qy/Dropbox/nirvana/workspaceCN/atu_tcp_library/include" -O0 -g3 -c -fmessage-length=0 -Wfatal-errors -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


