#include "FBO.h"
#include <iostream>
#include <gltools/gl.h>

namespace base
{

	FBO::FBO( int _width, int _height ) : width(_width), height(_height), numAttachments(0)
	{
		// create framebuffer (A framebuffer object is really just a container that attaches textures and renderbuffers to itself to create a complete configuration needed by the renderer.)
		glGenFramebuffersEXT(1, &fboId);
		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);


		// create render buffer for depth (A renderbuffer is a simple 2D graphics image in a specified format. This format usually is defined as color, depth or stencil data.)
		glGenRenderbuffersEXT(1, &m_depthbufferId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbufferId);

		// initialize renderbuffer so that it can hold depth ( glRenderbufferStorage — establish data storage, format and dimensions of a renderbuffer object's image -> create depthmap buffer)
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

		// attach depth renderbuffer it to fbo
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthbufferId);
	}



	void FBO::setOutputs( Texture2dPtr out0, Texture2dPtr out1, Texture2dPtr out2, Texture2dPtr out3 )
	{
		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		Texture2dPtr out[4] = { out0, out1, out2, out3 };

		numAttachments = 0;
		for( int i=0; i<4;++i )
			if( out[i] )
			{
				// attach texture to framebuffer
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, out[i]->m_id, 0);
				++numAttachments;
			}

		// unbind framebuffer
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	void FBO::setOutputs( Texture2dArrayPtr out )
	{
		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		glBindTexture( GL_TEXTURE_2D_ARRAY, out->m_id );

		if( out->m_zres > 4 )
			std::cout << "FBO::setOutputs( Texture2dArrayPtr out ) warning current only 4 layers supported \n";
		int numLayers = std::min( 4, out->m_zres );
		numAttachments = 0;
        for( int i = 0; i < numLayers; ++i )
		{
            glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, out->m_id, 0, i );
			++numAttachments;
		}

		// unbind framebuffer
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}


	void FBO::setSize( int newXres, int newYres )
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbufferId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, newXres, newYres);


		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		width = newXres;
		height = newYres;
	}


	void FBO::finalize()
	{
		//check
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);


		if( status == GL_FRAMEBUFFER_COMPLETE_EXT  )
			std::cout << "FBO successfull finalized\n";
		else
			std::cout << "FBO finalization error " << status << std::endl;

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	void FBO::begin( bool clear )
	{
		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		// save current states
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,width, height);

		// Set the render target
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT };
		glDrawBuffers(numAttachments, buffers);

		if( clear )
		{
			glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}//else
		//	glDisable( GL_DEPTH_TEST );
	}

	void FBO::end()
	{
		glPopAttrib();
		// unbind framebuffer
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

}
