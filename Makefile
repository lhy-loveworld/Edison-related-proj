CC = gcc
CFLAGS  = -g -Wall -lmraa
TARGET = servo
platform = $(shell uname -r | grep 'edison')

all: build

build: $(TARGET).c

ifeq ($(platform),)
	 $(CC) -o $(TARGET) $(TARGET).c $(CFLAGS) -DDUMMY
else
	 $(CC) -o $(TARGET) $(TARGET).c $(CFLAGS)
endif

clean: 
	 $(RM) $(TARGET)