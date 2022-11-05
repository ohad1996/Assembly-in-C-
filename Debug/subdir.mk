################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../base.c \
../filehandler.c \
../firsttransition.c \
../general.c \
../instruction.c \
../label.c \
../linkedList.c \
../macro.c \
../macrohandler.c \
../main.c \
../mem.c \
../node.c \
../psw.c \
../reg.c \
../secondtransition.c \
../word.c 

OBJS += \
./base.o \
./filehandler.o \
./firsttransition.o \
./general.o \
./instruction.o \
./label.o \
./linkedList.o \
./macro.o \
./macrohandler.o \
./main.o \
./mem.o \
./node.o \
./psw.o \
./reg.o \
./secondtransition.o \
./word.o 

C_DEPS += \
./base.d \
./filehandler.d \
./firsttransition.d \
./general.d \
./instruction.d \
./label.d \
./linkedList.d \
./macro.d \
./macrohandler.d \
./main.d \
./mem.d \
./node.d \
./psw.d \
./reg.d \
./secondtransition.d \
./word.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


