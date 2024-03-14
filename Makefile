#Which files should be linked ->
FILES = ./build/kernel.asm.o ./build/kernel.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
#by default, makefile runs first label that is seen

#all label says these
#	I need a file named: 		./bin/boot.bin, /bin/kernel.bin (these will be created)
all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
#	add bootloader to our os.bin (first sector of binary file)
	dd if=./bin/boot.bin >> ./bin/os.bin	
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

#below creates 512 byte long binary file
#nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

#assemble our file to object files for each file
./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

#link the kernel
./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

#interrupt descriptor table assembled
./build/idt/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o
#
./build/idt/idt.o: ./src/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I./src/idt $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o
#
./build/memory/memory.o: ./src/memory/memory.c
	i686-elf-gcc ${INCLUDES} ${FLAGS} -I./src/memory -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

#
./build/io/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o


clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf ${FILES}
	rm -rf ./build/kernelfull.o

#below add message content into boot.bin in the size of message.
#when analysed, it is shown that second sector starts with this message
#	dd if=./message.txt >> ./boot.bin
#second sector starts with message but it is not 512bytes long.
#below ensures second sector also takes 512 bytes bby padding zeros.
#	dd if=/dev/zero bs=512 count=1 >> ./boot.bin
	
