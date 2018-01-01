# compilation script
#       @by pietrek.j@gmail.com

#F_CPU=8000000
F_CPU=24000000
F_APB1=4000000
F_APB2=4000000
F_SYSCLK=F_CPU/8

linker_file=stm32f4_flash
startup_file=startup_stm32f40xx
CPU=cortex-m4
FPU=fpv4-sp-d16
MCU=STM32F401xC
#FPU_OPT= -mfpu=${FPU} -mfloat-abi=hard

CONFIG= -D${MCU} -DDEBUG -DF_CPU=${F_CPU} -DF_SYSCLK=${F_SYSCLK}
CFLAGS= -Os -s -mcpu=${CPU} -mlittle-endian -mthumb ${FPU_OPT} --specs=nosys.specs -fmax-errors=3 -Wno-attributes
INCLUDE= -I./ -I./include -I./periph_inc
LINKER_RES= -Tconfig/${linker_file}.ld startup.o 

ARCH=arm-none-eabi-
GCC=${ARCH}g++ -std=c++14
GDB=${ARCH}gdb
SIZE=${ARCH}size
OBJCOPY=${ARCH}objcopy

all: main.hex

startup.o: config/${startup_file}.s
	#${GCC} ${CFLAGS} -c config/startup_stm32f401.c -o startup.o
	${GCC} ${CFLAGS} -c config/${startup_file}.s -o startup.o


main.hex: Makefile main.cpp startup.o
	${GCC} ${CFLAGS} ${LINKER_RES} main.cpp -o main.elf ${INCLUDE} ${CONFIG}
	${SIZE} main.elf
	${OBJCOPY} -Oihex main.elf main.hex
	#cp main.hex /home/pietrek/shared/

run: main.hex
	./run.sh

debug: main.hex
	./run.sh
	${GDB} -x config/.gdbinit

asm: Makefile main.cpp startup.o
	${GCC} -S ${CFLAGS} ${LINKER_RES} main.cpp -o main.s ${INCLUDE} ${CONFIG}
	vim main.s

ppc: Makefile main.cpp
	${GCC} -E ${CFLAGS} ${LINKER_RES} main.cpp -o a.h ${INCLUDE} ${CONFIG}
	vim a.h

clean:
	rm -rf main.hex main.s main.elf ./periph_gen ./init_gen ./startup.o init.h periph.h lrtos.h a.h 2> /dev/null

