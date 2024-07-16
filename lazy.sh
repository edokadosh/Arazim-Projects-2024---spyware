make
gcc -O3 -Wall -std=c11 message_sender.c -o sender
gcc -O3 -Wall -std=c11 message_reader.c -o reader
insmod message_slot.ko
mknod /dev/slot1 c 235 1