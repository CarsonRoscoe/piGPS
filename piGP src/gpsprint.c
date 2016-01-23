#include "piGPS.h"

/*---------------------------------------------------------------------------------------
--	SOURCE FILE: 	gpsprint.c-	The drawing file of the piGPS application.
--					Utilizes curses for the UI.
--
--	PROGRAM:	piGPSd
--
--	FUNCTIONS:
--			void drawBox(int startX, int startY, int width, int height)
--			void degreeToString(double degree, char * str)
--			void colourOff(int elevation)
--			void colourOn(int elevation)
--			void draw(struct gps_data_t* data)
--
--	DATE: 		November 7th, 2015
--
--	REVISIONS:	November 7th, 2015: Created basic curses UI. Drawing boxes.
--			November 8th, 2015: Created the mandatory UI components for
--				displaying data gathered from the GPS.
--			November 9th, 2015: Added graphing the users latitude/longitude
--				as well as basic graphing for the direction of satellites
--				and the satellites altitude.
--
--	DESIGNER:	Carson Roscoe
--
--	PROGRAMMER:	Carson Roscoe
--
--	NOTES:
--	The UI is updated whenever the draw function is called. As far as external files are
--	concerned, draw is the only function and should be treated as the only "public" one.
--	The UI is built around the curses.h library.
------------------------------------------------------------------------------------------*/

/*Colour pair names based on altitude of satellite.
Default is white text. All of the colour pairs use a black background.
Colour is irrelevant. Chosing between is based on the altitude of the satilite
and the legend.*/
const int DEFAULT = 1;
const int XXLOW = 2;
const int XLOW = 3;
const int LOW = 4;
const int MEDIUM = 5;
const int HIGH = 6;
const int XHIGH = 7;

/*---------------------------------------------------------------------------------------
--	FUNCTION:	drawBox
--
--	DATE: 		November 7th, 2015
--
--	REVISIONS:	N/A
--
--	DESIGNER:	Carson Roscoe
--
--	PROGRAMMER:	Carson Roscoe
--
--	INTERFACE:	void drawBox(int startX, int startY, int width, int height)
--				startX: Horizontal position of the box. 0 is left.
--				startY: Vertical position of the box. 0 is at the top.
--				width: Width of the box to draw in characters.
--				height: Height of the box to draw in characters.
--
--	RETURNS:	void
--
--	NOTES:
--	This function is used when a simple square with a border is needed to be drawn.
--	The first two paramters are the top-left starting point, and the next two are
--	the width and height to draw from that starting point. The border is drawn in
--	the character 'X', and all non-border space is left blank.
------------------------------------------------------------------------------------------*/
void drawBox(int startX, int startY, int width, int height) {
	size_t i;
	for(i = 0; i < height; i++) {
		mvaddstr(i + startY, startX, "x");
		mvaddstr(i + startY, startX + width-1, "x");
	}

	for(i = 0; i < width; i++) {
		mvaddstr(startY, i + startX, "x");
		mvaddstr(startY + height-1, i + startX, "x");
	}
}

/*---------------------------------------------------------------------------------------
--	FUNCTION:	degreeToString
--
--	DATE: 		November 8th, 2015
--
--	REVISIONS:	N/A
--
--	DESIGNER:	Dhivya Manohar
--
--	PROGRAMMER:	Dhivya Manohar
--
--	INTERFACE:	void degreeToString(double degree, char * str)
--				degree: The degree angle to convert to a string.
--				str: The string to put the degree's string value in.
--
--	RETURNS:	void
--
--	NOTES:
--	Used to turn a degree angle into a strong of format XX° YY" ZZ' where XX° is the degree
--	rounded to a whole number, YY" are the minutes of the degree and ZZ' are the seconds.
------------------------------------------------------------------------------------------*/
void degreeToString(double degree, char * str) {
	int dsec, sec, deg, min;
	
	double fdsec, fsec, fdeg, fmin;
	if(degree < 0 || degree > 360){
		strlcpy(str,"nan",sizeof(str));
		return;
	}
	fmin = modf(degree, &fdeg);
	deg = (int)fdeg;
	
	fsec = modf(fmin*60,&fmin);
	min = (int)fmin;
	sec = (int)(fsec*10000.0);
	
	fdsec = modf(fsec * 60, &fsec);
	sec = (int)fsec;
	dsec = (int)(fdsec * 1000.0);
	sprintf(str, "%3d %02d' %02d.%03d", deg, min, sec, dsec);
}

/*---------------------------------------------------------------------------------------
--	FUNCTION:	colourOn
--
--	DATE: 		November 8th, 2015
--
--	REVISIONS:	N/A
--
--	DESIGNER:	Carson Roscoe
--
--	PROGRAMMER:	Carson Roscoe
--
--	INTERFACE:	void colourOn(int elevation)
--				elevation: Altitude of a satellite who's info we are displaying.
--
--	RETURNS:	void
--
--	NOTES:
--	Simply takes in the elevation of a satellite being drawn and changes the colour to
--	the appropriate colour pair.
------------------------------------------------------------------------------------------*/
void colourOn(int elevation) {
	if (elevation > (35)) 
		attron(COLOR_PAIR(XHIGH));
	 else if (elevation > (25)) 
		attron(COLOR_PAIR(HIGH));
	 else if (elevation > (15)) 
		attron(COLOR_PAIR(MEDIUM));
	 else if (elevation > (7.5)) 
		attron(COLOR_PAIR(LOW));
	 else if (elevation > (2.5)) 
		attron(COLOR_PAIR(XLOW));
	 else 
		attron(COLOR_PAIR(XXLOW));
}

/*---------------------------------------------------------------------------------------
--	FUNCTION:	colourOff
--
--	DATE: 		November 8th, 2015
--
--	REVISIONS:	N/A
--
--	DESIGNER:	Carson Roscoe
--
--	PROGRAMMER:	Carson Roscoe
--
--	INTERFACE:	void colourOff(int elevation)
--				elevation: Altitude of a satellite who's info we are displaying.
--
--	RETURNS:	void
--
--	NOTES:
--	Simply takes in the elevation of a satellite that was being drawn and turns off the 
--	associated colour pair. This is called when you are no longer writing the satillites
--	information.
------------------------------------------------------------------------------------------*/
void colourOff(int elevation) {
	if (elevation > (35)) 
		attroff(COLOR_PAIR(XHIGH));
	 else if (elevation > (25)) 
		attroff(COLOR_PAIR(HIGH));
	 else if (elevation > (15)) 
		attroff(COLOR_PAIR(MEDIUM));
	 else if (elevation > (7.5)) 
		attroff(COLOR_PAIR(LOW));
	 else if (elevation > (2.5)) 
		attroff(COLOR_PAIR(XLOW));
	 else 
		attroff(COLOR_PAIR(XXLOW));
}

/*---------------------------------------------------------------------------------------
--	FUNCTION:	draw
--
--	DATE: 		November 8th, 2015
--
--	REVISIONS:	N/A
--
--	DESIGNER:	Carson Roscoe
--
--	PROGRAMMER:	Carson Roscoe
--
--	INTERFACE:	void draw(struct gps_data_t* data)
--				data: Data structure containing data received from the satallites.
--
--	RETURNS:	void
--
--	NOTES:
--	This function handles all drawing of the UI. It is the only publically available function
--	to be accesible outside of the source file. The function takes in a parameter containing
--	all necessary data to be drawn to the screen. It utilizes the drawBox function to layout
--	the screen dynamically and draws each section of data into different quadrants.
------------------------------------------------------------------------------------------*/
void draw(struct gps_data_t* data) {
	int frameWidth;
	int frameHeight;
	int frameStartX;
	int frameStartY;	
	char tempstr[128];
	char temptime[128];
	size_t i;
	int height;
	int width;

	//Initiate the window if it is not already.
	if (mainwin == NULL) {
		delwin(mainwin);
		if ( (mainwin = initscr()) == NULL ) {
			fprintf(stderr, "Error initialising ncurses.\n");
			exit(EXIT_FAILURE);
			
		}
		nodelay(mainwin, 1);
		start_color();
	}
		
	//Gets the width and height of the window.
	getmaxyx(mainwin, height, width);

	//Clears all previous data on the screen.
	erase();
	
	//Generates the text/background colour pairs used.
	init_pair(DEFAULT, COLOR_WHITE, COLOR_BLACK);
	init_pair(XXLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(XLOW, COLOR_CYAN, COLOR_BLACK);
	init_pair(LOW, COLOR_BLUE, COLOR_BLACK);
	init_pair(MEDIUM, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(HIGH, COLOR_GREEN, COLOR_BLACK);
	init_pair(XHIGH, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(DEFAULT));

	//Draw window.
	drawBox(0, 0, width, height);

	//Draw graphing frame.
	frameWidth = width - 21;
	frameHeight = height - data->satellites_visible - 7;
	frameStartX = 5;
	frameStartY = 2;
	drawBox(frameStartX, frameStartY, frameWidth, frameHeight);

	//Write the longitude degrees around the frame.
	mvaddstr(frameStartY - 1, frameStartX, "180");
	mvaddstr(frameStartY - 1, frameStartX + frameWidth - 3, "180");
	mvaddstr(frameStartY - 1, frameStartX + frameWidth / 4, "90");
	mvaddstr(frameStartY - 1, frameStartX + (frameWidth / 4)*3, "90");
	mvaddstr(frameStartY - 1, frameStartX + frameWidth / 2, "0");
	mvaddstr(frameStartY + frameHeight, frameStartX, "180");
	mvaddstr(frameStartY + frameHeight, frameStartX + frameWidth - 3, "180");
	mvaddstr(frameStartY + frameHeight, frameStartX + frameWidth / 4, "90");
	mvaddstr(frameStartY + frameHeight, frameStartX + (frameWidth / 4)*3, "90");
	mvaddstr(frameStartY + frameHeight, frameStartX + frameWidth / 2, "0");

	//Write the latitude degrees around the frame.
	mvaddstr(frameStartY, frameStartX - 3, "90");
	mvaddstr(frameStartY + frameHeight/4, frameStartX - 3, "45");
	mvaddstr(frameStartY + (frameHeight/4)*3, frameStartX - 3, "45");
	mvaddstr(frameStartY + frameHeight/2, frameStartX - 3, "0");
	mvaddstr(frameStartY + frameHeight - 1, frameStartX - 3, "90");
	mvaddstr(frameStartY, frameStartX + frameWidth + 1, "90");
	mvaddstr(frameStartY + frameHeight/4, frameStartX + frameWidth + 1, "45");
	mvaddstr(frameStartY + (frameHeight/4)*3, frameStartX + frameWidth + 1, "45");
	mvaddstr(frameStartY + frameHeight/2, frameStartX + frameWidth + 1, "0");
	mvaddstr(frameStartY + frameHeight - 1, frameStartX + frameWidth + 1, "90");

	//Display the NW/NE/SW/SE labels for clarity.
	mvaddstr(frameStartY - 1, frameStartX - 3, "NW");
	mvaddstr(frameStartY - 1, frameStartX + frameWidth + 1, "NE");
	mvaddstr(frameStartY + frameHeight, frameStartX - 3, "SW");
	mvaddstr(frameStartY + frameHeight, frameStartX + frameWidth + 1, "SE");

	//Draw the users position
	double lo, la;
	int posx, posy;
	modf(data->fix.longitude, &lo);
	modf(data->fix.latitude, &la);
	posx = (lo+180)/360 * frameWidth; //-180 to 180
	posy = (((-1)*la)+90)/180 * frameHeight; //-90 to 90
	mvaddstr(posy, posx, "O");

	//Draw the satallites direction surounding the user and the colour associated with
	//the altitude the satalite.
	if (data->satellites_visible >= 3) {
		attroff(COLOR_PAIR(DEFAULT));
		for(i = 0; i < data->satellites_visible; i++) {
			int tempx = sin(data->azimuth[i])*5;
			int tempy = cos(data->azimuth[i])*5;
			colourOn(data->elevation[i]);
			mvaddstr(posy + tempy, posx + tempx, "S");
			colourOff(data->elevation[i]);
		}
		attron(COLOR_PAIR(DEFAULT));
	}
	

	//Draw legend
	drawBox(width - 12, 0, 12, height - data->satellites_visible - 3);
	mvaddstr(1, width - 10, "LEGEND");
	mvaddstr(2, width - 11, "---------");
	attroff(COLOR_PAIR(DEFAULT));
	attron(COLOR_PAIR(XHIGH));
	mvaddstr(3, width - 10, "40000km");
	attroff(COLOR_PAIR(XHIGH));
	attron(COLOR_PAIR(HIGH));
	mvaddstr(4, width - 10, "30000km");
	attroff(COLOR_PAIR(HIGH));
	attron(COLOR_PAIR(MEDIUM));
	mvaddstr(5, width - 10, "20000km");
	attroff(COLOR_PAIR(MEDIUM));
	attron(COLOR_PAIR(LOW));
	mvaddstr(6, width - 10, "10000km");
	attroff(COLOR_PAIR(LOW));
	attron(COLOR_PAIR(XLOW));
	mvaddstr(7, width - 10, "5000km");
	attroff(COLOR_PAIR(XLOW));
	attron(COLOR_PAIR(XXLOW));
	mvaddstr(8, width - 10, "1000km");
	attroff(COLOR_PAIR(XXLOW));
	attron(COLOR_PAIR(DEFAULT));
	mvaddstr(9, width - 10, "O = You");
	mvaddstr(10, width - 10, "X = Satt.");

	//Draw the data structures timestamp/lat/long/prn/elevation/azimuth/snr/usedflag at bottoms
	drawBox(0, height-(data->satellites_visible) -4, width, 8); 
	for(i = 0; i < data->satellites_visible; i++) {
		sprintf(tempstr, "PRN: %d Elevation: %d Azimuth: %d SNR: %f Used: %s", data->PRN[i], data->elevation[i], data->azimuth[i], data->ss[i], data->used[i] > 0 ? "Y" : "N");
		mvaddstr(height-data->satellites_visible -3 + i, 2, tempstr);
	}
	unix_to_iso8601(data->fix.time, temptime, sizeof(temptime));
	sprintf(tempstr, "Time stamp(UTF): %s", temptime);
	mvaddstr(height-3, 2, tempstr);
	char lat[64];
	char lon[64];
	degreeToString(fabs(data->fix.latitude), lat);
	degreeToString(fabs(data->fix.longitude), lon);
	if (data->satellites_visible > 2)
		sprintf(tempstr, "Latitude: %s %c\t Longitude: %s %c", lat, data->fix.latitude < 0 ? 'S' : 'N' , lon, data->fix.longitude < 0 ? 'E' : 'W' );
	else
		sprintf(tempstr, "No fix." );
	mvaddstr(height-2, 2, tempstr);

	refresh();
}