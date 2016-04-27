# piGPS
GPS application written in C for Raspberry Pi (*nix). Utilizes GPSD for all GPS functionality and the curses library for all UI work.

The intention of this application was to turn a Raspberry Pi into a GPS. A GPS dongle is required for the application to work, as well as having GPSD setup on your Raspberry Pi.

All satellites detected will have their information displayed. If three or more satellites are visible to the GPS, we have enough information to display the exact location of the user on a 2D ascii map of the world (based on lattitude and longitude). 

The user is drawn as an 'X' on the screen, and all satellites are displayed around the user as well. The sattelites have their colour changed based on elevation, essentially allowing us to get a 3D (okay, 2.5D?) graphing on a 2D ASCII based console application.

# How To Compile

Before compiling, you must have the GPSD daemon service setup. Follow instructions here to setup GPSD: http://www.catb.org/gpsd/installation.html

After setting up GPSD, clone or download the files onto your *nix system.

Open up terminal, navigate to the directory of the files and then type "make" into the terminal. This should invoke the Makefile which will compile the program.

# How To Run

Just like compiling, you must have GPSD daemon service setup to utilize the application.

Once you have, clone/download this repository and navigate to the folder. In terminal, run the file "piGPS"

# Design Document

The design document for the program is the DesignDocument.pdf. That is where you will see state diagrams regarding how the program was planned to be structured, along with pseudocode. The end program did not perfectly match the design, however it is, for the most part, the same as the end structure.

# Testing Document

The testing document for the program is the TestingDocument.pdf. This is where all the test cases were run with test summaries at the top and more detailed test breakdowns (with screenshots) in the pages proceeding the summary.

# Collaboration
This project was written in collaboration with Dhivya Manohar (https://github.com/DhivManohar)
