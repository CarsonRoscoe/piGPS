#include "piGPS.h"
/*---------------------------------------------------------------------------------------
--	SOURCE FILE: 	gpspiGPS.c-	The drawing file of the piGPS application.
--					Utilizes curses for the UI.
--
--	PROGRAM:	piGPSd
--
--	FUNCTION:
--			int main()
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
--			N/A
------------------------------------------------------------------------------------------*/
/* Creates a static fixsource_t strucute*/
static fixsource_t source;

/* Intializes a constant interger*/
const int FAILED_TO_OPEN = 2;

int main() {
	int flags = WATCH_ENABLE|WATCH_JSON;
	data = malloc(sizeof(struct gps_data_t));

	if (gps_open(source.server, source.port, data ) != 0) {
		exitGPS(FAILED_TO_OPEN);
	}
	
	if (source.device != NULL)
		flags |= WATCH_DEVICE;

	if (gps_stream(data, flags, source.device) != 0) {
		exitGPS(FAILED_TO_OPEN);
	}
	
	//starts reading data
	readGPS(data);
	return 0;
}
