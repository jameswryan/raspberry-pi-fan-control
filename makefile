SRC = fan.c
TRG = fan
CC = gcc
CFLAGS = -g -Wall -Werror -W  
LIBS = -lpigpiod_if2 -lrt

all:
	$(CC) $(CFLAGS) -o $(TRG) $(SRC) $(LIBS)

clean:
	$(RM) $(TRG)

run:	
	./$(TRG)
