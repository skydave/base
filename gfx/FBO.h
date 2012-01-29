#pragma once
#include <util/shared_ptr.h>
#include <gltools/gl.h>
#include "Texture.h"

namespace base
{

	BASE_DECL_SMARTPTR_STRUCT(FBO);

	struct FBO
	{
		FBO( int _width, int _height );

		void setOutputs( Texture2dPtr out0, Texture2dPtr out1 = Texture2dPtr(), Texture2dPtr out2 = Texture2dPtr(), Texture2dPtr out3 = Texture2dPtr() );
		void setOutputs( Texture2dArrayPtr out );
		void setSize( int newXres, int newYres );
		void finalize();


		void begin( unsigned int clearBits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		void end();

		unsigned int fboId;
		int width, height;
		char numAttachments;
		unsigned int m_depthbufferId;
	};
}
