/* 
	VideoDemonstrator.c
    Copyright (C) 2017  Karuna Kiran Bhadra
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
OpenGL project   : "Demonstration of a video player"

Description      : This is a video only player, working with pre-existing clips
				   that have been integrated into the codebase.
				   The player works by displaying rapidly screenshots in .jpg format
				   of the video in question. These screenshots have been taken in 
				   one frame intervals in the popular Open Source video client 
				   'VidoeLAN Client' also known as VLC. We have included options 
				   in the player for slowing down and speeding up playback and play/
				   pause alternation. A seek bar has also been provided.
					
Author 			:  Karuna Kiran Bhadra

Author email 	:  arkansascropper@gmail.com
			       
Requirements 	:  The machine of the compiler must have the following libraries 
				   installed :=
				   1. OpenGL
				   2. GLUT or FreeGLUT
				   3. Simple OpenGL Image Loading Library ( SOIL )
				   4. GCC or MinGW or its equivalent on the platform in question
				   			       
Recommendations :  Keep the frame folders that have been provided with the 
				   source-code in the same directory as the executable.
*/

#include <stdio.h>						
#include <GL/freeglut.h>									//The more update version of the GLUT library
#include <SOIL/SOIL.h>										//Simple OpenGL Image Loading library. ( SOIL )
#include <unistd.h>
#include <string.h>											//String library to manipulate the directory and frame strings
#include <dirent.h>											//Directory operations to count the no of frames for initalization


#define ESCAPE 27										 	//ASCII for ESCAPE key
#define SPACE 32										 	//ASCII for SPACE bar

int ch;														//Holds the value of the choice selection
int width, height;											//Width and height of the frames
GLint window;												//Integer reference for the window opened
int sleepdur;												//Duration of frame delay in microseconds
int p = 1;													//Play is P = 1. Pause is P = 0
int i = 0;													//Loop variable
int n = 0;													//Holds number of frames
GLfloat someColor[ 3 ][ 3 ] = { { 0.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 } };					
char *image_Arr[ 100000 ]; 									//Holds frame data

															
void init( ) {												//Initializations : start init()
		
	char str[40];											//String for holding the complete frame address in filesystem
	int l;													//Loop variable
	char *formt;											//Holds formatted string
	char di[30];											//Holds frame directory name
	DIR *dp;												//DIRECTORY type pointer to point to the selected frame directory
	struct dirent *ep;										//Directory entry
	
	//If Choice is the first clip
	if( ch == 1 ) 
	{
	
		strcpy( di, "Puny_God" );
		sleepdur = 20000;
		
	}
	
	//If choice is second clip
	else if( ch == 2 ) 
	{
	
		strcpy( di, "Tom_And_Jerry" );
		sleepdur = 40000;
		
	}	
	
	//Open Directory
	dp = opendir( di );
	
	//If directory opened successfully
	if( dp != NULL ) 
	{
		//While the entry is  not null
		while( ep = readdir( dp ) ) 
		{
			
			//Avoid self pointer and parent pointer
			if( ( strcmp( ep->d_name, "." ) == 0 ) || ( strcmp( ep->d_name, ".." ) == 0 ) )
				continue;
			n++;											//Increment file counter
			
		}
		//After the count is finished close the directory
		( void ) closedir( dp );
	}
	
	//If directory opening is not successful
	else
		perror( "Could not open directory" );
	
	//Initializr image_Arr[] with the frame names
	for( l = 0; l < n; l++ ) {
		
		//Form formatted string
		sprintf( str, "%s/scene%.5d.jpg", di, ( l + 1 ) );
		//Assign memory to image_Arr[l] dynamically to hold string 
		image_Arr[ l ] = (char * ) malloc( (strlen(str)) * sizeof(char) );
		//Copy into image_Arr[ l ] formatted string
		strcpy( image_Arr[ l ], str );
		
	}
	
	//Clear Background to black
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    
    //Create Texture objecct to hold image as textures
    //integer to hold texture reference
    GLuint tex;
    glGenTextures( 1, &tex );								//Generate first availabe texture name
    glBindTexture( GL_TEXTURE_2D, tex );					//Create texture name
    
    //If texture smaller than given area, scale up with linear wrapping
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    //If texture larger than given area, scale down with nearest wrapping
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    //Set projection mode for orthogonal view
    glMatrixMode( GL_PROJECTION );
    //Load identity matrix
    glLoadIdentity();
    //Set orthogonal clipping coordinates
    gluOrtho2D( 0.0, 800.0, 0.0, 400.0 );
    
}															//End init()



void display() 												//Start display() function
{
			
    unsigned char *image;									//Unsugned byte type to hold the image data address
    
    glEnable( GL_TEXTURE_2D );    
    if( p == 1 ) 											//If condition is play, iterate through the images
    {
    
    	//Load up the image into image pointer by SOIL_load_image
     	image = SOIL_load_image( image_Arr[ i ], &width, &height, 0, SOIL_LOAD_RGB );
     	//Generate texture
       	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
        //Clear buffer to preset values
       	glClear( GL_COLOR_BUFFER_BIT );
        
        //Draw a quad with the texture attached to its vertices	
        glBegin( GL_QUADS );
        glColor3f( 1.0, 1.0, 1.0 );							//Background black
        	//Bottom-left
        	glTexCoord2f( 0.0f, 1.0f );						
        	glVertex2f( 30.0f, 60.0f  );
        	//Bottom-right
        	glTexCoord2f( 1.0f, 1.0f );
        	glVertex2f( 770.0f, 60.0f );
        	//Top-right
    		glTexCoord2f( 1.0f, 0.0f );
        	glVertex2f( 770.0f, 370.0f );
        	//Top-left
        	glTexCoord2f( 0.0f, 0.0f );
        	glVertex2f( 30.0f, 370.0f );
        glEnd();
        
        i++;												//Increment frame
    	i = i % n;											//Keep frame inside frame count limit
    	
    }
    
    else 													//If condition is play
    {
    	
    	//load last static frame
      	image = SOIL_load_image( image_Arr[ i ], &width, &height, 0, SOIL_LOAD_RGB );
      	//Generate texture
       	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
        
        //Clear buffer to preset values
       	glClear( GL_COLOR_BUFFER_BIT );
       	
       	//Draw a quad with the texture attached to its vertices	
        glBegin( GL_QUADS );				
        glColor3f( 1.0, 1.0, 1.0 );							//Background black
        	//Bottom-left
        	glTexCoord2f( 0.0f, 1.0f );
        	glVertex2f( 30.0f, 60.0f  );
        	//Bottom-right
        	glTexCoord2f( 1.0f, 1.0f );
        	glVertex2f( 770.0f, 60.0f );
        	//Top-right
        	glTexCoord2f( 1.0f, 0.0f );
        	glVertex2f( 770.0f, 370.0f );
        	//Top-left
    		glTexCoord2f( 0.0f, 0.0f );
        	glVertex2f( 30.0f, 370.0f );
        glEnd();
        
    }
   	glDisable( GL_TEXTURE_2D );          											
    if( p == 0 ) 											//If paused draw play icon
    {
    
       	//We are drawing a triangle here
    	glBegin( GL_POLYGON );
    	glColor3fv( someColor[ 0 ] );
        	glVertex2f( 400.0, 10.0 );
        	glVertex2f( 417.3, 20.0 );
        	glVertex2f( 400.0, 30.0 );
        glEnd();
    }
    
    else 													//If currently playing draw pause icon
    {
        //We draw two separate vertical rectangles here
        //First one
        glColor3fv( someColor[ 0 ] );	
        glBegin( GL_POLYGON );
        	glVertex2f( 400.0, 10.0 );
        	glVertex2f( 405.0, 10.0 );
        	glVertex2f( 405.0, 30.0 );
        	glVertex2f( 400.0, 30.0 );
        glEnd();
        
        //Second one
        glBegin( GL_POLYGON );
        	glVertex2f( 410.0, 10.0 );
        	glVertex2f( 415.0, 10.0 );
        	glVertex2f( 415.0, 30.0 );
        	glVertex2f( 410.0, 30.0 );
        glEnd();
        
    }
    
    //This is the seek status bar that will be drawn depending on which frame is playing    
    glBegin( GL_POLYGON );
    //Color has been set to cyan. The contrast depends on the frame contrast.
    glColor3f( 0.0, 1.0, 1.0 );
    	glVertex2f( 0.0, 40.0 );
    	//Find out the ratio of the frame rendered to total frame to determine length
    	glVertex2f( ( (GLfloat) i / (GLfloat) n ) * 800.0, 40.0 );
    	glVertex2f( ( (GLfloat) i / (GLfloat) n ) * 800.0, 50.0 );
    	glVertex2f( 0.0, 50.0 );
    glEnd();
    
    //subtract sign to slow down the playback displayed at to the left of the play/pause button
    glBegin( GL_POLYGON );
    //Color chosen to be white but the frame determines contrast.
    glColor3fv( someColor[ 2 ] );
    	glVertex2f( 194.0, 15.0 );
    	glVertex2f( 209.0, 15.0 );
    	glVertex2f( 209.0, 20.0 );
    	glVertex2f( 194.0, 20.0 );
    glEnd();
    
    //Plus sign to speed up playback displayed to the right of the play/pause button
    //Drawing the plus is done by making two rectangles intersect at right angles.
    //First rectangle
    glBegin( GL_QUADS );
    glColor3fv( someColor[ 1 ] );
    	glVertex2f( 592.0, 17.0 );
    	glVertex2f( 607.0, 17.0 );
    	glVertex2f( 607.0, 22.0 );
    	glVertex2f( 592.0, 22.0 );
    glEnd();
    //Second rectangle
    glBegin( GL_QUADS );
    glColor3fv( someColor[ 1 ] );
    	glVertex2f( 597.0, 12.0 );
    	glVertex2f( 602.5, 12.0 );
    	glVertex2f( 602.5, 28.0 );
    	glVertex2f( 597.0, 28.0 );
    glEnd();
  	  
    glFlush();												//Display all the generated contents
    SOIL_free_image_data( image );							//free up the image data at pointer
        
    usleep(sleepdur);   									//Introduce delay to prevent too-fast playback
}


void myMouse( int button, int state, int x, int y ) 		//Mouse event handler
{
	//If playback is set to play
    if( p == 1 ) 
    {
    	//If left button is pressed and next state of button is down
        if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) 
        {	
        	
        	//If the area in which the click has occured corresponds to play/pause button area
            if( ( ( x >= 400 && x <= 418 ) && ( y >= 370 && y <= 390 ) ) || ( ( x >= 663 && x <=713 ) && ( y >= 648 && y <= 685 ) ) ) 
            {
            	someColor[ 0 ][ 0 ] = 0.0;
        		someColor[ 0 ][ 1 ] = 0.0;
        		someColor[ 0 ][ 2 ] = 1.0;
            	//Display "pause in the terminal
                //set playback mode to pause
                p = 0;
                
            }
            
            //If the click area corresponds to the area for the slow-playback button
            else if( ( ( x >= 194 && x <= 209 ) && ( y >= 380 && y <= 385 ) ) || ( ( x >= 333.5 && x <= 349.5 ) && ( y >= 648 && y <= 685 ) ) )
            {
        		someColor[ 2 ][ 0 ] = 0.0;
        		someColor[ 2 ][ 1 ] = 0.0;
        		someColor[ 2 ][ 2 ] = 1.0;
            	//increase delay in frame display
            	sleepdur *= 1.5;
            }
            
            //If the click area corresponds to the area for the faster-playback button
            else if( ( ( x >= 594 && x <= 609 ) && ( y >= 370 && y <= 390 ) ) || ( ( x >= 1016.5 && x <= 1032.5 ) && ( y >= 648 && y <= 685 ) ) )
            {
        		someColor[ 1 ][ 0 ] = 0.0;
        		someColor[ 1 ][ 1 ] = 0.0;
        		someColor[ 1 ][ 2 ] = 1.0;
            	//Reduce delay in frame display
            	sleepdur *= 0.5;
       		}
        
    	}
    	
    	//If left button is pressed and next state of button is down
    	if( button == GLUT_LEFT_BUTTON && state == GLUT_UP ) 
        {	
        	
        	//If the area in which the click has occured corresponds to play/pause button area
            if( ( ( x >= 400 && x <= 418 ) && ( y >= 370 && y <= 390 ) ) || ( ( x >= 663 && x <=713 ) && ( y >= 648 && y <= 685 ) ) ) 
            {
            	someColor[ 0 ][ 0 ] = 0.0;
        		someColor[ 0 ][ 1 ] = 1.0;
        		someColor[ 0 ][ 2 ] = 1.0;
                
            }
            
            //If the click area corresponds to the area for the slow-playback button
            else if( ( ( x >= 194 && x <= 209 ) && ( y >= 380 && y <= 385 ) ) || ( ( x >= 333.5 && x <= 349.5 ) && ( y >= 648 && y <= 685 ) ) )
            {
        		someColor[ 2 ][ 0 ] = 0.0;
        		someColor[ 2 ][ 1 ] = 1.0;
        		someColor[ 2 ][ 2 ] = 1.0;
            }
            
            //If the click area corresponds to the area for the faster-playback button
            else if( ( ( x >= 594 && x <= 609 ) && ( y >= 370 && y <= 390 ) ) || ( ( x >= 1016.5 && x <= 1032.5 ) && ( y >= 648 && y <= 685 ) ) )
            {
            	//Change to yellow when clicked
        		someColor[ 1 ][ 0 ] = 0.0;
        		someColor[ 1 ][ 1 ] = 1.0;
        		someColor[ 1 ][ 2 ] = 1.0;
       		}
        
    	}

    }
    
    //If playback mode is pause
    else if( p == 0 ) 
    {
    	//If left mouse button is pressed and the next state button is down
        if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) 
        {
        	//If the area in which the click has occured corresponds to play/pause button area
            if( ( ( x >= 400 && x <= 450 ) && ( y >= 370 && y <= 390 ) ) || ( ( x >= 663 && x <=713 ) && ( y >= 648 && y <= 685 ) ) ) 
            {
                //Change playback mode
                p = 1;
                
            }
        }
    }
    
    //To seek from the seek bar... Just as it should be done
    if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
    	if( y >= 351 && y <= 360 )
    		i = ( ( GLfloat ) x / 800 ) * n;
    	else if( y >= 610 && y <= 629 )
    		i = ( ( GLfloat ) x / 1366 ) * n;
    }
}															//End myMouse()

void myKeyBoard( unsigned char key, int x, int y )			//Keyboard event handler
{
	//stops the program from trashing this function
    usleep( 100 );
    //If key pressed is ESC
    if( key == ESCAPE ) 
    {
    	//Destroy the current window and free all resources
        glutDestroyWindow( window );
        //Exit function
        exit(0);
    }
    
    //if key pressed is SPACE BAR
    if( key == SPACE ) 
    {
    	//if playback mode is "Play"
    	if( p == 1 ) 
    	{
    	
    		//Change color to blue
    		someColor[ 0 ][ 0 ] = 0.0;
        	someColor[ 0 ][ 1 ] = 0.0;
        	someColor[ 0 ][ 2 ] = 1.0;
    		//Change it to "Pause"
    		p = 0;
    	}
    	
    	//Else if playback mode is "Pause"
    	else
    	{	
    		//Change color to blue
    		someColor[ 0 ][ 0 ] = 0.0;
        	someColor[ 0 ][ 1 ] = 1.0;
       		someColor[ 0 ][ 2 ] = 1.0;
    		//Change it to "Play"
    		p = 1;
    	}
    }
}															//End myKeyBoard()
    
void main( int argc, char **argv )							//This is where execution begins 
{
	
	char title[40];											//To store the widow title
	//Menu for clip selection
	printf( "Enter one of the following Clip indexes: \n" );
	printf( "1. BAM BAM!!\n" );
	printf( "2. Tom And Jerry\n" );
	printf( "> " );
	scanf( "%d", &ch );										//Input at terminal
	//Selection of title
	if( ch == 1 ) 
		strcpy( title, "BAM BAM!!" );						//Something special!!!
	else if( ch == 2 )
		strcpy( title, "Tom and Jerry" );					//Who doesn't like Tom And Jerry?
		while( ch != 1 && ch != 2 ) 
		{
			printf( "Enter again from given clips: " );
			scanf( "%d", &ch );
		}
	
	//Initialize OpenGL functions	
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE );			//Display mode set to RGB and SINGLE
    glutInitWindowSize( 800, 400 );							//Initial window size kept at 400 and 800
    glutInitWindowPosition( 20, 20 );						//Initial window position
    window = glutCreateWindow( title );						//Assign window reference for doing close operation
    glutIdleFunc( display );								//Select idle function. This is what keeps it looping!
    glutDisplayFunc( display );								//Assign display function. Poor display. Does all the work.
    glutMouseFunc( myMouse );								//Assign mouse handler function. Clickety-click.
    glutKeyboardFunc( myKeyBoard );							//Assign keybord handler function. Tap-tap.
    
    init();													//Initialize all the frame names and texture elements
    
    glutMainLoop();											//Keep calling main()
}															//End main()

//Phew!!



