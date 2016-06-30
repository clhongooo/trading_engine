################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Util/Acct.cpp \
../Util/BarAggregator.cpp \
../Util/BarProvider.cpp \
../Util/BestBidAskTrade.cpp \
../Util/CME.cpp \
../Util/CPnLHist.cpp \
../Util/CommissionCalculator.cpp \
../Util/Constants.cpp \
../Util/ContFut.cpp \
../Util/DeriCtrt.cpp \
../Util/Exchange.cpp \
../Util/FuturesCtrt.cpp \
../Util/HKFE.cpp \
../Util/HKMA.cpp \
../Util/HKSE.cpp \
../Util/IParam.cpp \
../Util/NYSE.cpp \
../Util/OHLC.cpp \
../Util/OParam.cpp \
../Util/OptionCtrt.cpp \
../Util/OrderBook.cpp \
../Util/PParam.cpp \
../Util/PeriodicTask.cpp \
../Util/PortChromosome.cpp \
../Util/QuoteProvider.cpp \
../Util/RegularMiniCtrtManager.cpp \
../Util/SDateTime.cpp \
../Util/SFunctional.cpp \
../Util/STool.cpp \
../Util/SystemState.cpp \
../Util/Tokenizer.cpp \
../Util/TradeTick.cpp \
../Util/TradeTickProvider.cpp \
../Util/TradeWarehouse.cpp \
../Util/TradingHours.cpp 

OBJS += \
./Util/Acct.o \
./Util/BarAggregator.o \
./Util/BarProvider.o \
./Util/BestBidAskTrade.o \
./Util/CME.o \
./Util/CPnLHist.o \
./Util/CommissionCalculator.o \
./Util/Constants.o \
./Util/ContFut.o \
./Util/DeriCtrt.o \
./Util/Exchange.o \
./Util/FuturesCtrt.o \
./Util/HKFE.o \
./Util/HKMA.o \
./Util/HKSE.o \
./Util/IParam.o \
./Util/NYSE.o \
./Util/OHLC.o \
./Util/OParam.o \
./Util/OptionCtrt.o \
./Util/OrderBook.o \
./Util/PParam.o \
./Util/PeriodicTask.o \
./Util/PortChromosome.o \
./Util/QuoteProvider.o \
./Util/RegularMiniCtrtManager.o \
./Util/SDateTime.o \
./Util/SFunctional.o \
./Util/STool.o \
./Util/SystemState.o \
./Util/Tokenizer.o \
./Util/TradeTick.o \
./Util/TradeTickProvider.o \
./Util/TradeWarehouse.o \
./Util/TradingHours.o 

CPP_DEPS += \
./Util/Acct.d \
./Util/BarAggregator.d \
./Util/BarProvider.d \
./Util/BestBidAskTrade.d \
./Util/CME.d \
./Util/CPnLHist.d \
./Util/CommissionCalculator.d \
./Util/Constants.d \
./Util/ContFut.d \
./Util/DeriCtrt.d \
./Util/Exchange.d \
./Util/FuturesCtrt.d \
./Util/HKFE.d \
./Util/HKMA.d \
./Util/HKSE.d \
./Util/IParam.d \
./Util/NYSE.d \
./Util/OHLC.d \
./Util/OParam.d \
./Util/OptionCtrt.d \
./Util/OrderBook.d \
./Util/PParam.d \
./Util/PeriodicTask.d \
./Util/PortChromosome.d \
./Util/QuoteProvider.d \
./Util/RegularMiniCtrtManager.d \
./Util/SDateTime.d \
./Util/SFunctional.d \
./Util/STool.d \
./Util/SystemState.d \
./Util/Tokenizer.d \
./Util/TradeTick.d \
./Util/TradeTickProvider.d \
./Util/TradeWarehouse.d \
./Util/TradingHours.d 


# Each subdirectory must supply rules for building sources it contributes
Util/%.o: ../Util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/qy/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/workspaceCN/NirvanaCommon/Util" -I"/home/qy/workspaceCN/NirvanaCommon/Logger" -I"/home/qy/workspaceCN/NirvanaCommon/Math" -I"/home/qy/workspaceCN/NirvanaCommon/Model" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -O0 -g3 -c -fmessage-length=0 -Wfatal-errors -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


