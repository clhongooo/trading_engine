################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SharedObjects/CorrelMatrices.cpp \
../SharedObjects/HKIntradaySeasonality.cpp \
../SharedObjects/HSIDailyHLRangeEstimation.cpp \
../SharedObjects/MDI_Acknowledgement.cpp \
../SharedObjects/MarketData.cpp \
../SharedObjects/ProbDistributions.cpp \
../SharedObjects/ShortTermAlpha.cpp \
../SharedObjects/StyCPnLHist.cpp \
../SharedObjects/SystemConfig.cpp \
../SharedObjects/TechIndicators.cpp \
../SharedObjects/VolSurfaces.cpp 

OBJS += \
./SharedObjects/CorrelMatrices.o \
./SharedObjects/HKIntradaySeasonality.o \
./SharedObjects/HSIDailyHLRangeEstimation.o \
./SharedObjects/MDI_Acknowledgement.o \
./SharedObjects/MarketData.o \
./SharedObjects/ProbDistributions.o \
./SharedObjects/ShortTermAlpha.o \
./SharedObjects/StyCPnLHist.o \
./SharedObjects/SystemConfig.o \
./SharedObjects/TechIndicators.o \
./SharedObjects/VolSurfaces.o 

CPP_DEPS += \
./SharedObjects/CorrelMatrices.d \
./SharedObjects/HKIntradaySeasonality.d \
./SharedObjects/HSIDailyHLRangeEstimation.d \
./SharedObjects/MDI_Acknowledgement.d \
./SharedObjects/MarketData.d \
./SharedObjects/ProbDistributions.d \
./SharedObjects/ShortTermAlpha.d \
./SharedObjects/StyCPnLHist.d \
./SharedObjects/SystemConfig.d \
./SharedObjects/TechIndicators.d \
./SharedObjects/VolSurfaces.d 


# Each subdirectory must supply rules for building sources it contributes
SharedObjects/%.o: ../SharedObjects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/ATU" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/PCH" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -O0 -g3 -c -fmessage-length=0 -Wfatal-errors -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


