# PREFIX="$(HOME)/opt/cross"
# TARGET=i686-elf
# PATH:="$(PREFIX)/bin:$(PATH)"

#Which files should be linked ->
FILES = ./build/kernel.asm.o ./build/kernel.o ./build/idt/idt.asm.o ./build/idt/idt.o 	\
		./build/memory/memory.o ./build/io/io.asm.o ./build/memory/heap/heap.o 			\
		./build/memory/heap/kheap.o ./build/memory/paging/paging.o ./build/memory/paging/paging.asm.o \
		./build/disk/disk.o ./build/disk/streamer.o ./build/fs/pparser.o ./build/fs/file.o ./build/fs/fat/fat16.o \
		./build/string/string.o ./build/gdt/gdt.o ./build/gdt/gdt.asm.o ./build/task/tss.asm.o \
		./build/task/task.o ./build/task/process.o ./build/task/task.asm.o \
		./build/isr80h/isr80h.o ./build/isr80h/heap.o ./build/isr80h/misc.o ./build/isr80h/io.o ./build/keyboard/keyboard.o \
		./build/keyboard/classic.o ./build/loader/formats/elf.o ./build/loader/formats/elfloader.o

INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
#by default, makefile runs first label that is seen

#all label says these
#	I need a file named: 		./bin/boot.bin, /bin/kernel.bin (these will be created)
all: clean ./bin/boot.bin ./bin/kernel.bin user_programs
	rm -rf ./bin/os.bin
#	add bootloader to our os.bin (first sector of binary file)
	dd if=./bin/boot.bin >> ./bin/os.bin	
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin
	sudo mount -t vfat ./bin/os.bin /mnt/d
#	# Copy a file over
	sudo cp ./hello.txt /mnt/d
	sudo cp ./programs/blank/blank.elf /mnt/d
#	sudo cp ./programs/shell/shell.elf /mnt/d
	sudo umount /mnt/d
	
#below creates 512 byte long binary file
#nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./bin/kernel.bin: $(FILES)
	@mkdir -p $(@D)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

#assemble our file to object files for each file
./bin/boot.bin: ./src/boot/boot.asm
	@mkdir -p $(@D)
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

#link the kernel
./build/kernel.asm.o: ./src/kernel.asm
	@mkdir -p $(@D)
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

#./build/kernel.o: ./src/kernel.c
#	i686-elf-gcc ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

#interrupt descriptor table assembled
./build/idt/idt.asm.o: ./src/idt/idt.asm
	@mkdir -p $(@D)
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

#global descriptor table assembled
./build/gdt/gdt.asm.o: ./src/gdt/gdt.asm
	@mkdir -p $(@D)
	nasm -f elf -g ./src/gdt/gdt.asm -o ./build/gdt/gdt.asm.o

./build/task/tss.asm.o: ./src/task/tss.asm
	@mkdir -p $(@D)
	nasm -f elf -g ./src/task/tss.asm -o ./build/task/tss.asm.o

./build/task/task.asm.o: ./src/task/task.asm
	@mkdir -p $(@D)
	@nasm -f elf -g ./src/task/task.asm -o ./build/task/task.asm.o

./build/%.o: ./src/%.c
	@mkdir -p $(@D)
#	@echo $(@D)
#	@echo $^
#	@echo $@
	@echo "i am compiling -> $^ "
	@i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $^ -o $@
#
#./build/memory/memory.o: ./src/memory/memory.c
#	i686-elf-gcc ${INCLUDES} ${FLAGS} -I./src/memory -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o
#
#./build/memory/heap/heap.o: ./src/memory/heap/heap.c
#	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o
#
#./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
#	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o
#	
#./build/memory/paging/paging.o: ./src/memory/paging/paging.c
#	i686-elf-gcc $(INCLUDES) -I./src/memory/paging $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o

#./build/fs/pparser.o: ./src/fs/pparser.c
#	i686-elf-gcc $(INCLUDES) -I./src/fs $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o

./build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	@mkdir -p $(@D)
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o

./build/io/io.asm.o: ./src/io/io.asm
	@mkdir -p $(@D)
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

#user land programs
user_programs:
	cd ./programs/stdlib && $(MAKE) all
	cd ./programs/blank && $(MAKE) all

user_programs_clean:
	cd ./programs/stdlib && $(MAKE) clean
	cd ./programs/blank && $(MAKE) clean


clean: user_programs_clean
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
	
