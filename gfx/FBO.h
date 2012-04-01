//
// http://www.opengl.org/wiki/Framebuffer_Objects
//

#pragma once
#include <util/shared_ptr.h>
#include <gltools/gl.h>
#include "Texture.h"

namespace base
{

	BASE_DECL_SMARTPTR_STRUCT(FBO);

	struct FBO
	{
		struct FBOSetup
		{
			FBOSetup();

			int                                      getWidth() const;
			int                                     getHeight() const;
			bool                               isMultisampled() const;
			int                                 getNumSamples() const;
			int                                 getNumOutputs() const;
			bool                             hasStencilBuffer() const;

			FBOSetup                              &width( int width );
			FBOSetup                            &height( int height );
			FBOSetup                     &multisample( bool enabled );
			FBOSetup                    &numSamples( int numSamples );
			FBOSetup                   &attach( Texture2dPtr output );
			FBOSetup             &attach( Texture2dArrayPtr outputs );
			FBOSetup                   &stencilBuffer( bool enabled );

			operator FBOPtr ();

			std::vector<Texture2dPtr>                       m_outputs;
			Texture2dArrayPtr                   m_textureArrayOutputs;
			int                                               m_width;
			int                                              m_height;
			bool                                       m_multisampled;
			int                                          m_numSamples;
			bool                                      m_stencilBuffer;
			float                                     m_clearColor[4];
			int                                        m_clearStencil;
		};

		FBO( const FBOSetup &setup );
		FBO( int _width, int _height, bool multisampled = false, int numSamples = 4 );

		static FBOSetup                                                      create();

		void setOutputs( Texture2dPtr out0, Texture2dPtr out1 = Texture2dPtr(), Texture2dPtr out2 = Texture2dPtr(), Texture2dPtr out3 = Texture2dPtr() );
		void setOutputs( Texture2dArrayPtr out );
		void setSize( int newXres, int newYres );
		void finalize();


		void begin( unsigned int clearBits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		void end();

		unsigned int fboId;
		unsigned int m_depthbufferId;
		FBOSetup             m_setup;
	};
}
