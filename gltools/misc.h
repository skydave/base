#pragma once
#include <string>
#include <math/Math.h>

#define CHECKGLERROR() checkGlError(__FILE__, __LINE__)

namespace base
{
	void                                      drawGrid( bool axes = true );  ///< draw a orientation grid identical to the ones used in popular dcc apps
	void                        drawTransform( const math::Matrix44f &tm );  ///< draw orientation vectors for given transform
	void                                                       headLight();  ///< sets light0 parameters accordingly

	std::string                           glErrorString( int glErrorCode );
	bool                          checkGlError(const char *file, int line); // returns true if there was an error
}





