TARGET:= writer
CROSS_COMPILE:=
FILE_UTIL:= ../assignments/assignment2/fileresult.txt

CC:= $(CROSS_COMPILE)gcc
CFLAGS:= -Wall -g

.PHONY: all clean

all: $(TARGET)
$(TARGET): writer.o
	$(CC) $(CFLAGS) -o $@ $^ -lm
write.o: writer.c
	$(CC) $(CFLAGS) -c $< -o $@

clean::
	rm -f *.o $(TARGET) *.elf
