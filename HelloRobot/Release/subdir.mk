################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Main.cpp \
../Map.cpp \
../Node.cpp \
../STC.cpp \
../behavior.cpp \
../behaviorForward.cpp \
../behaviorLeft.cpp \
../behaviorRight.cpp \
../driver.cpp \
../lodepng.cpp \
../robot.cpp \
../robotState.cpp \
../wayPointManager.cpp 

OBJS += \
./Main.o \
./Map.o \
./Node.o \
./STC.o \
./behavior.o \
./behaviorForward.o \
./behaviorLeft.o \
./behaviorRight.o \
./driver.o \
./lodepng.o \
./robot.o \
./robotState.o \
./wayPointManager.o 

CPP_DEPS += \
./Main.d \
./Map.d \
./Node.d \
./STC.d \
./behavior.d \
./behaviorForward.d \
./behaviorLeft.d \
./behaviorRight.d \
./driver.d \
./lodepng.d \
./robot.d \
./robotState.d \
./wayPointManager.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


