#pragma once

#include <math/Math.h>



namespace base
{
	void                                      drawGrid( bool axes = true );  ///< draw a orientation grid identical to the ones used in popular dcc apps
	void                        drawTransform( const math::Matrix44f &tm );  ///< draw orientation vectors for given transform
	void                                                       headLight();  ///< sets light0 parameters accordingly
}





