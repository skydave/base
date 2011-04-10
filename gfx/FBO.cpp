#include "FBO.h"
#include <iostream>
#include <gltools/gl.h>

namespace base
{

	FBO::FBO( int _width, int _height ) : width(_width), height(_height), numAttachments(0)
	{
		glGenFramebuffersEXT(1, &fboId);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);


		glGenRenderbuffersEXT(1, &m_depthbufferId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbufferId);

		//
		// depth
		//
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

		// attach it to fbo
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthbufferId);
	}



	void FBO::setOutputs( Texture2dPtr out0, Texture2dPtr out1, Texture2dPtr out2, Texture2dPtr out3 )
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		Texture2dPtr out[4] = { out0, out1, out2, out3 };

		numAttachments = 0;
		for( int i=0; i<4;++i )
			if( out[i] )
			{
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, out[i]->m_id, 0);
				++numAttachments;
			}

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
		//
		// draw into fbo
		//
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		//finalize();

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
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

}