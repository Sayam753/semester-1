/*****************************************************************************
   Project: CEDAR Logic Simulator
   Copyright 2006 Cedarville University, Benjamin Sprague,
                     Matt Lewellyn, and David Knierim
   All rights reserved.
   For license information see license.txt included with distribution.   

   guiText: Encapsulation of text for GUI objects
*****************************************************************************/

#ifndef GUITEXT_H_
#define GUITEXT_H_

#ifdef __WXMAC__
#  ifdef __DARWIN__
#    include <OpenGL/glu.h>
#  else
#    include <glu.h>
#  endif
#else
#  include <GL/glu.h>
#endif

#include <string>
using namespace std;

struct GLbox {
	GLdouble top;
	GLdouble bottom;
	GLdouble left;
	GLdouble right;
};

class guiText {
public:
	guiText();
	virtual ~guiText();
	
	// *************** Action methods *********************

	// Render using current settings on current canvas:
	void draw( void );

	// Return the bounding box of the text object (in local-space coordinates + scale and translation):
	GLbox getBoundingBox( void );

	// *************** Mutator methods ****************************
	void setColor( GLdouble c1, GLdouble c2, GLdouble c3, GLdouble c4 ) {
		color[0] = c1;
		color[1] = c2;
		color[2] = c3;
		color[3] = c4;
	};
	
	void getColor( GLdouble& c1, GLdouble& c2, GLdouble& c3, GLdouble& c4 ) {
		c1 = color[0];
		c2 = color[1];
		c3 = color[2];
		c4 = color[3];
	};

	// Set and get the position of the text object (Defaults to <0.0, 0.0>)
	void setPosition( GLdouble x, GLdouble y ) {
		translate[0] = x;
		translate[1] = y;
	};

	void getPosition( GLdouble& x, GLdouble& y ) {
		x = translate[0];
		y = translate[1];
	};
	
	// Set the scale factor by setting a text height and aspect ratio (w / h).
	// NOTE: You can't get these values back from this class, or any direct scale info.
	void setSize( GLdouble textHeight, GLdouble aspect = 1.0 );
	
	// Get and set the text string.
	string getText( void ) { return textString; };
	void setText( string newString ) { textString = newString; };

private:
	// The text color:
	GLfloat color[4];
	
	// X and Y scale factors:
	GLdouble scale[2];
	
	// X and Y translation factors (Optional):
	GLdouble translate[2];

	// The text string to be displayed:	
	string textString;
	
	// The font loading initialization flag:
	static bool fontIsLoaded;
	
	// The counted reference object to tell when there are no more guiText objects
	// that exist:
	static unsigned long numTextObjects;
};

#endif /*GUITEXT_H_*/
