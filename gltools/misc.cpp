#include "misc.h"

#include "gl.h"

#include <iostream>

namespace base
{
	//
	// sets light0 parameters accordingly
	//
	void headLight()
	{
		float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		float diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
		float specularLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
		float position[] = { 0.0f, 0.0f, 1.0f, 0.0f }; // headlight (directional)

		// head-light
		glPushMatrix();
		glLoadIdentity();
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glPopMatrix();
	}

	//
	// draw orientation vectors for given transform
	//
	void drawTransform( const math::Matrix44f &tm )
	{
		math::Vec3f t = tm.getTranslation();
		math::Vec3f right = t + tm.getRight(false);
		math::Vec3f up = t + tm.getUp(false);
		math::Vec3f dir = t + tm.getDir(false);

		glLineWidth( 5.0f );
		glBegin( GL_LINES );
		// right axes
		glColor3f( 0.8f, 0.0f, 0.0f );
		glVertex3f( t.x, t.y, t.z );
		glVertex3f( right.x, right.y, right.z );
		// forward axes
		glColor3f( 0.0f, 0.0f, 0.8f );
		glVertex3f( t.x, t.y, t.z );
		glVertex3f( dir.x, dir.y, dir.z );
		// up axes
		glColor3f( 0.0f, 0.8f, 0.0f );
		glVertex3f( t.x, t.y, t.z );
		glVertex3f( up.x, up.y, up.z );

		glEnd();
	}



	//
	// draw a orientation grid identical to the ones used in popular dcc apps
	//
	void drawGrid( bool axes )
	{

		float gridSizeHalved = 5.0f / 2.0f;
		float gridSpacing = 0.5f;
		float y = 0.0f;

		glLineWidth( 1.0f );
		glBegin( GL_LINES );
		// main axes
		glColor3f( 0.3f, 0.3f, 0.3f );
		glVertex3f( -gridSizeHalved, y, 0.0f );
		glVertex3f(  gridSizeHalved, y, 0.0f );
		glVertex3f( 0.0f, y, -gridSizeHalved );
		glVertex3f( 0.0f, y,  gridSizeHalved );

		// gridlines
		glColor3f( 0.7f, 0.7f, 0.7f );
		int i1 = 1;
		while( i1*gridSpacing < gridSizeHalved )
		{
			// grid in x
			glVertex3f( i1*gridSpacing, y, -gridSizeHalved );
			glVertex3f( i1*gridSpacing , y , gridSizeHalved );

			glVertex3f( -i1*gridSpacing , y , -gridSizeHalved );
			glVertex3f( -i1*gridSpacing , y , gridSizeHalved );
			
			// grid in z
			glVertex3f( -gridSizeHalved , y , i1*gridSpacing );
			glVertex3f( gridSizeHalved , y , i1*gridSpacing );

			glVertex3f( -gridSizeHalved ,  y , -i1*gridSpacing );
			glVertex3f( gridSizeHalved , y , -i1*gridSpacing );

			i1++;
		}

		// wrap lines all around to make the grid closed
		glVertex3f( -gridSizeHalved , y , -gridSizeHalved );
		glVertex3f( -gridSizeHalved, y , gridSizeHalved );

		glVertex3f( -gridSizeHalved, y , gridSizeHalved );
		glVertex3f( gridSizeHalved , y , gridSizeHalved );

		glVertex3f( gridSizeHalved , y , gridSizeHalved );
		glVertex3f( gridSizeHalved, y , -gridSizeHalved );

		glVertex3f( gridSizeHalved, y , -gridSizeHalved );
		glVertex3f( -gridSizeHalved , y , -gridSizeHalved );

		glEnd();

		// draw koordinate axes
		if(axes)
		{
			glLineWidth( 5.0f );
			glBegin( GL_LINES );
			// right axes
			glColor3f( 0.8f, 0.0f, 0.0f );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glVertex3f( 1.0f, 0.0f, 0.0f );
			// forward axes
			glColor3f( 0.0f, 0.0f, 0.8f );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glVertex3f( 0.0f, 0.0f, 1.0f );
			// up axes
			glColor3f( 0.0f, 0.8f, 0.0f );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glVertex3f( 0.0f, 1.0f, 0.0f );

			glEnd();
		}
	}



	std::string glErrorString( int glErrorCode )
	{
		switch(glErrorCode)
		{
		case GL_NO_ERROR:
				return "No error has been recorded.";break;
		case GL_INVALID_ENUM:
				return "An unacceptable value is specified for an enumerated argument.";break;
		case GL_INVALID_VALUE:
				return "A numeric argument is out of range.";break;
		case GL_INVALID_OPERATION:
			return "The specified operation is not allowed in the current state.";break;
		case GL_STACK_OVERFLOW:
			return "This command would cause a stack overflow.";break;
		case GL_STACK_UNDERFLOW:
			return "This command would cause a stack underflow.";break;
		case GL_OUT_OF_MEMORY:
			return "There is not enough memory left to execute the command.";break;
		case GL_TABLE_TOO_LARGE:
			return "GL_TABLE_TOO_LARGE";break;
		default:
			return "";
		};
		return "";
	}
	bool checkGlError(const char *file, int line)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::cerr << "OpenGL Error at " << file << " line " << line << ":" << glErrorString(err) << std::endl;
			return true;
		}
		return false;
	}
}
