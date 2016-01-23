#ifndef piGPS_H
#define piGPS_H
#include <stdlib.h>
#include <stdio.h>
#include <gps.h>
#include <curses.h>
#include <math.h>


#define PIGS 0
#define FLY 1

/*variable for window used in multiple files*/
WINDOW * mainwin;

/*structure pointer that holds data for one satellite */
static struct gps_data_t* data;


/*structure that holds data for one computer */
typedef struct{
	char *sped;
	char *server;
	char *port;
	char *device;
}fixsource_t;
/*function prototypes*/
void readGPS(struct gps_data_t*);
void print(struct gps_data_t*);
void draw(struct gps_data_t*);
void exitGPS(int code);
#endif