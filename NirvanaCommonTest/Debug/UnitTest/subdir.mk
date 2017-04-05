################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../UnitTest/TestBarAggregator.cpp \
../UnitTest/TestCPnLHist.cpp \
../UnitTest/TestCommissionCalculator.cpp \
../UnitTest/TestCountingFunction.cpp \
../UnitTest/TestDaySma.cpp \
../UnitTest/TestDistributions.cpp \
../UnitTest/TestExchange.cpp \
../UnitTest/TestGKYZ.cpp \
../UnitTest/TestHKMA.cpp \
../UnitTest/TestHKSE.cpp \
../UnitTest/TestKolmogorovSmirnov.cpp \
../UnitTest/TestLeastSqRegression.cpp \
../UnitTest/TestLinearSysSolver.cpp \
../UnitTest/TestMACD.cpp \
../UnitTest/TestNYSECME.cpp \
../UnitTest/TestOrderBook.cpp \
../UnitTest/TestPeriodSma.cpp \
../UnitTest/TestPeriodicTask.cpp \
../UnitTest/TestProbDistrnFunc.cpp \
../UnitTest/TestProbMassFunc.cpp \
../UnitTest/TestRglrMiniCtrtMgr.cpp \
../UnitTest/TestRsi.cpp \
../UnitTest/TestSFunctional.cpp \
../UnitTest/TestTickReturn.cpp \
../UnitTest/TestTradingHours.cpp \
../UnitTest/TestTrdTickAna.cpp \
../UnitTest/TestVolSurf.cpp \
../UnitTest/UTest.cpp 

OBJS += \
./UnitTest/TestBarAggregator.o \
./UnitTest/TestCPnLHist.o \
./UnitTest/TestCommissionCalculator.o \
./UnitTest/TestCountingFunction.o \
./UnitTest/TestDaySma.o \
./UnitTest/TestDistributions.o \
./UnitTest/TestExchange.o \
./UnitTest/TestGKYZ.o \
./UnitTest/TestHKMA.o \
./UnitTest/TestHKSE.o \
./UnitTest/TestKolmogorovSmirnov.o \
./UnitTest/TestLeastSqRegression.o \
./UnitTest/TestLinearSysSolver.o \
./UnitTest/TestMACD.o \
./UnitTest/TestNYSECME.o \
./UnitTest/TestOrderBook.o \
./UnitTest/TestPeriodSma.o \
./UnitTest/TestPeriodicTask.o \
./UnitTest/TestProbDistrnFunc.o \
./UnitTest/TestProbMassFunc.o \
./UnitTest/TestRglrMiniCtrtMgr.o \
./UnitTest/TestRsi.o \
./UnitTest/TestSFunctional.o \
./UnitTest/TestTickReturn.o \
./UnitTest/TestTradingHours.o \
./UnitTest/TestTrdTickAna.o \
./UnitTest/TestVolSurf.o \
./UnitTest/UTest.o 

CPP_DEPS += \
./UnitTest/TestBarAggregator.d \
./UnitTest/TestCPnLHist.d \
./UnitTest/TestCommissionCalculator.d \
./UnitTest/TestCountingFunction.d \
./UnitTest/TestDaySma.d \
./UnitTest/TestDistributions.d \
./UnitTest/TestExchange.d \
./UnitTest/TestGKYZ.d \
./UnitTest/TestHKMA.d \
./UnitTest/TestHKSE.d \
./UnitTest/TestKolmogorovSmirnov.d \
./UnitTest/TestLeastSqRegression.d \
./UnitTest/TestLinearSysSolver.d \
./UnitTest/TestMACD.d \
./UnitTest/TestNYSECME.d \
./UnitTest/TestOrderBook.d \
./UnitTest/TestPeriodSma.d \
./UnitTest/TestPeriodicTask.d \
./UnitTest/TestProbDistrnFunc.d \
./UnitTest/TestProbMassFunc.d \
./UnitTest/TestRglrMiniCtrtMgr.d \
./UnitTest/TestRsi.d \
./UnitTest/TestSFunctional.d \
./UnitTest/TestTickReturn.d \
./UnitTest/TestTradingHours.d \
./UnitTest/TestTrdTickAna.d \
./UnitTest/TestVolSurf.d \
./UnitTest/UTest.d 


# Each subdirectory must supply rules for building sources it contributes
UnitTest/%.o: ../UnitTest/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/workspaceCN/NirvanaCommon" -I"/home/qy/workspaceCN/NirvanaInfrastructure" -I"/home/qy/workspaceCN/NirvanaCommonTest/UnitTest" -I"/home/qy/workspaceCN/NirvanaInfrastructure/DataAggregation" -I"/home/qy/workspaceCN/NirvanaInfrastructure/Facilities" -I"/home/qy/workspaceCN/NirvanaInfrastructure/OrderExecutionAlgo" -I"/home/qy/workspaceCN/NirvanaInfrastructure/ProbDistributionGeneration" -I"/home/qy/workspaceCN/NirvanaInfrastructure/SharedObjects" -I"/home/qy/workspaceCN/NirvanaInfrastructure/TechIndUpdating" -I"/home/qy/workspaceCN/NirvanaInfrastructure/VolSurfaceCalculation" -I"/home/qy/workspaceCN/NirvanaCommon/Logger" -I"/home/qy/workspaceCN/NirvanaCommon/Math" -I"/home/qy/workspaceCN/NirvanaCommon/Model" -I"/home/qy/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/workspaceCN/NirvanaCommon/Util" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


