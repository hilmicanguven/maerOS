all:	#by default, makefile runs first label that is seen
#below creates 512 byte long binary file
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

clean:
	rm -rf ./bin/boot.bin
#below add message content into boot.bin in the size of message.
#when analysed, it is shown that second sector starts with this message
#	dd if=./message.txt >> ./boot.bin
#second sector starts with message but it is not 512bytes long.
#below ensures second sector also takes 512 bytes bby padding zeros.
#	dd if=/dev/zero bs=512 count=1 >> ./boot.bin
	
