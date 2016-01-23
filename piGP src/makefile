CC = gcc -Wall
CLIB = -lgps -lcurses -lm


piGPS: piGPS.o gps-utils.o gpsprint.o 
	$(CC) -o piGPS piGPS.o gps-utils.o gpsprint.o $(CLIB) 

clean:
	rm -f *.o core.* piGPS
piGPS.o: piGPS.c piGPS.h
	$(CC) -c piGPS.c
gps-utils.o: gps-utils.c piGPS.h
	$(CC) -c gps-utils.c 
gpsprint.o: gpsprint.c piGPS.h
	$(CC) -c gpsprint.c 