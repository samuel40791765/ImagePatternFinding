/************************************************************************
     File:        ArcBallCam.h

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						This is a camera UI for OpenGL Applications.

	 
						An arcball is a user interface for rotating an object 
						around its very useful for looking around the world.

						The actual mathematics to implement it are elegant, 
						but beyond the scope of CS559. This example code is 
						provided for you to use
						without having to worry about the details of 
						how it works.

     Note:
						1.	A quickly hacked together implementation of ArcBall, 
							with lots of bits lifted from Ken Shoemake's original 
							source in Graphics Gems	made into a nice C++ thing
						2.	It is simplified a lot - the controller is always 
						   at the origin,
						3.	It has a fixed radius, it doesn't have constraints

						4.	Added the double right click and the mousewheel - 
							to make a much	better UI.

					   5. the "ball" also stores all of the other camera 
							parameters (position, FoV)

						6. We take over the right mouse button - pass us your 
							events, and we'll take the ones we want.
						7. the ALT button lets you do panning
							double right click resets the arcball if you get lost

						To use it:
						1) instantiate it in your Fl_Gl_Window class
						2) call setup in the constructor of your window class
						3) call its handle method inside of your handle method
						4) use its SetProjection method when you need to set the 
							camera (like when drawing)

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/
#pragma once
#include "3DUtils.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>

//***************************************************************************
//
// * if you need to pass that to OpenGL, try...
//===========================================================================
inline float* asGlMatrix(HMatrix m) 
//===========================================================================
{
	return (float*) m; 
};

class Quat {
	public:
		enum QuatPart {X=0, Y=1, Z=2, W=3, QuatLen};

	public:
		Quat();						/* gives the identity */
		Quat(float x, float y, float z, float w);
		Quat(const Quat&);			/* copy constructor */

	public:
		// conversions
		void toMatrix(HMatrix) const;

		// operations
		Quat conjugate() const;
		// make multiply look like multiply
		Quat operator* (const Quat&) const;
		// Normalize the quaternion back to length 1
		void renorm();

	public:
		// the data
		float x, y, z, w;
};


//**************************************************************************
//
// The arcball implementation
//
//**************************************************************************
class ArcBallCam {
	public:
		ArcBallCam();

	public:
		// you must call setup before doing anything!
		void setup(QWidget* wind,
					float fieldOfView=40.0,						 // field of view, in degrees
					float eyeZ = 20.0,							 // how far away camera is from the center of the world
					float isx=0, float isy=0, float isz = 0 // initial rotation
					);

		//*********************************************************************
		//
		//	all you really need is: handle and setProjection
		//
		//*********************************************************************

		// this sets the projection for viewing. it clears (and sets) the
		// projection and modelview matrices
		// note: we might not want to clear out the projection matrix
		// (for example, if there is a pick matrix), so we give the option
		// of not doing the load identity
		void setProjection(bool doClear=true);

		// Reset to a basic configuration
		void reset();

		//*********************************************************************
		//
		// Simplified user interface
		//
		//*********************************************************************
		// multiply matrix ontop of stack
		void multMatrix();	

		// call this when the mouse goes down
		void down(const float x, const float y);

		// this updates the cached positions - call it when the mouse is dragged
		void computeNow(const float nowX, const float nowY);

		// this gets the global matrix (start and now)
		void getMatrix(HMatrix) const;

		// Spin the ball by some vector - if you don't understand
		// how an arcball works, you probably don't care about this
		// but: basically you give it a vector to rotate the world around
		// - the length of the vector is how much to rotate around that
		//   axis
		void spin(float x, float y, float z);
		// Internal routine
		// this gets the position of the mouse in "normalized device coordinates"
		// that is X & Y go from -1 to 1 (with zero at the center)
		// this really goes with the window, but its handy here, and we can't
		// assume the window does it
		void getMouseNDC(float mx, float my, float& x, float& y);

	public:
		// This keeps track of the rotation - the current rotation is
		// start*now
		Quat				start;	// orientation at the start of drag
		Quat				now;		// quaternion "now" (while dragging)

		float				downX;	// where the mouse went down
		float				downY;	

		// are we panning or rotating (or not tracking)
		enum {
			None, 
			Rotate, 
			Pan
		} mode;	

			
		
		float				panX;		// this keeps track of the panning - the current pan is 
		float				panY;		

		float				eyeX;		// in eyeX, eyeY, but this is the displacement for undoing the last
		float				eyeY;		// addition
		float				eyeZ;

		float				initEyeZ;
		float				fieldOfView; // View of field


		
		float				isx;		// save a preferred rotation to return to
		float				isy;
		float				isz;

		QWidget* wind;	// Draw window
};
