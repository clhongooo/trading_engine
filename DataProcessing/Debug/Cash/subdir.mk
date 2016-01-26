################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Cash/DailyOHLCToMarketFeed.cpp \
../Cash/EnsureIncTime.cpp \
../Cash/MarketFeedCondensor.cpp \
../Cash/MarketFeedMerger.cpp \
../Cash/MarketFeedToOHLC.cpp \
../Cash/MinuteOHLCToMarketFeed.cpp \
../Cash/ToBBAT.cpp 

OBJS += \
./Cash/DailyOHLCToMarketFeed.o \
./Cash/EnsureIncTime.o \
./Cash/MarketFeedCondensor.o \
./Cash/MarketFeedMerger.o \
./Cash/MarketFeedToOHLC.o \
./Cash/MinuteOHLCToMarketFeed.o \
./Cash/ToBBAT.o 

CPP_DEPS += \
./Cash/DailyOHLCToMarketFeed.d \
./Cash/EnsureIncTime.d \
./Cash/MarketFeedCondensor.d \
./Cash/MarketFeedMerger.d \
./Cash/MarketFeedToOHLC.d \
./Cash/MinuteOHLCToMarketFeed.d \
./Cash/ToBBAT.d 


# Each subdirectory must supply rules for building sources it contributes
Cash/%.o: ../Cash/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/PCH" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


