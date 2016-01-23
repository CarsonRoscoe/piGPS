#include "piGPS.h"

/*---------------------------------------------------------------------------------------
--	SOURCE FILE: 	gpspiGPS.c-	The drawing file of the piGPS application.
--					Utilizes curses for the UI.
--
--	PROGRAM:	piGPSd
--
--	FUNCTION:
--			void readGPS(struct gps_data_t *data)
--			void exitGPS(int code)
--			
--
--	DATE: 		November 7th, 2015
--
--	REVISIONS:	November 7th, 2015: Created file with all methods in c++. 
--			November 8th, 2015: Changed file with all to methods to c.
--
--
--	DESIGNER:	Dhivya Manohar
--
--	PROGRAMMER:	Dhivya Manohar
--
--	NOTES:
--			Class is called to read the data coming in and to error handle any
--			errors that may occur and to display data to a Window. 
------------------------------------------------------------------------------------------*/

/* Intializes constant intergers*/
const int QUIT = 0;
const int FAILED_TO_READ = 1;

/*---------------------------------------------------------------------------------------
--	FUNCTION:	readGPS
--
--	DATE: 		November 8th, 2015
--
--	REVISIONS:	N/A
--
--	DESIGNER:	Dhivya Manohar
--
--	PROGRAMMER:	Dhivya Manohar
--
--	INTERFACE:	void readGPS(struct gps_data_t *data)
--				*data : pointer to a structure that contains 
--					all the information from a single satellite
--
--	RETURNS:	void
--
--	NOTES:
--	This function is used to continuously read the data call the draw method from 
	gpsprint.c to display data and map approximately map coordinates the Window.
------------------------------------------------------------------------------------------*/

void readGPS(struct gps_data_t *data) {
	//Reading portion
	while(PIGS != FLY) {
		if (gps_waiting(data, 1000000)) {
			if (gps_read(data) == -1) {
				fprintf(stderr, "Error in reading.\n");
				exitGPS(FAILED_TO_READ);
			} else {
				draw(data);
			}
		}
		if (mainwin != NULL) {
			if (wgetch(mainwin) == 'q') {
				exitGPS(QUIT);
				return;
			}
		}
		
	}
}
/*---------------------------------------------------------------------------------------
--	FUNCTION:	exitGPS
--
--	DATE: 		November 8th, 2015
--
--	REVISIONS:	N/A
--
--	DESIGNER:	Dhivya Manohar
--
--	PROGRAMMER:	Dhivya Manohar
--
--	INTERFACE:	void exitGPS(int code)
--				code : contains the value that tells the method on how to 
--					close the programs
--					
--
--	RETURNS:	void
--
--	NOTES:
--	This function is called when there is an error in the program or user decides to 
--	quit program. This function also closes and frees all structures and streams.
------------------------------------------------------------------------------------------*/
void exitGPS(int code) {
	switch(code) {
		//Closing
		case 0: 
			fprintf(stderr, "Closing piGPS.\n");
			break;
		//Failed to open
		case 1: 
			fprintf(stderr, "Failed to open GPSD stream.\n");
			break;
		//Failed to read
		case 2:
			fprintf(stderr, "Error in reading from socket.\n");
			break;
	}
	//Closing curse items.
	erase();
	refresh();
	delwin(mainwin);
	endwin();

	//Closing GPSD items.
	gps_stream(data, WATCH_DISABLE, NULL);
	gps_close(data);
	free(data);
	exit(code == QUIT ? EXIT_SUCCESS : EXIT_FAILURE);
} 
