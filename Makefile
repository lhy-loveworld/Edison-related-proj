CC = gcc
CFLAGS  = -g -Wall -lmraa
TARGET = servo
platform = $(shell uname -r | grep 'edison')

all: build

build: $(TARGET).c

ifeq ($(platform),)
	 $(CC) -o $(TARGET)_tcp $(TARGET).c $(CFLAGS) -DDUMMY
	 $(CC) -o $(TARGET)_tls $(TARGET).c $(CFLAGS) -DDUMMY
else
	 $(CC) -o $(TARGET)_tcp $(TARGET).c $(CFLAGS)
	 $(CC) -o $(TARGET)_tls $(TARGET).c $(CFLAGS)
endif

clean: 
	 $(RM) $(TARGET)