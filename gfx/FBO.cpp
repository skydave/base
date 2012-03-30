#include "FBO.h"
#include <iostream>
#include <gltools/gl.h>

namespace base
{

	FBO::FBOSetup::FBOSetup()
	{
		m_width = 512;
		m_height = 512;
		m_multisampled = false;
		m_numSamples = -1;
	}

	int FBO::FBOSetup::getWidth() const{return m_width;}
	int FBO::FBOSetup::getHeight() const{return m_height;}
	bool FBO::FBOSetup::isMultisampled() const{return (m_multisampled != 0);}
	int FBO::FBOSetup::getNumSamples() const{return m_numSamples;}
	int FBO::FBOSetup::getNumOutputs() const
	{
		if(!m_outputs.empty())
			return (int)(m_outputs.size());
		else if(m_textureArrayOutputs)
			return std::min( 4, m_textureArrayOutputs->m_zres );
		else
			return 0;
	};

	FBO::FBOSetup &FBO::FBOSetup::width( int width )
	{
		m_width = width;
		return *this;
	}

	FBO::FBOSetup &FBO::FBOSetup::height( int height )
	{
		m_height = height;
		return *this;
	}

	FBO::FBOSetup &FBO::FBOSetup::multisample( bool enabled )
	{
		m_multisampled = enabled;
		return *this;
	}

	FBO::FBOSetup &FBO::FBOSetup::numSamples( int numSamples )
	{
		m_numSamples = numSamples;
		return *this;
	}

	FBO::FBOSetup &FBO::FBOSetup::attach( Texture2dPtr output )
	{
		m_outputs.push_back( output );
		return *this;
	}

	FBO::FBOSetup &FBO::FBOSetup::attach( Texture2dArrayPtr outputs )
	{
		if( outputs->m_zres > 4 )
			std::cout << "FBO::setOutputs( Texture2dArrayPtr out ) warning current only 4 layers supported \n";
		m_textureArrayOutputs = outputs;
		return *this;
	}

	
	FBO::FBOSetup::operator FBOPtr()
	{
		return FBOPtr( new FBO( *this ) );
	}



	FBO::FBO( const FBOSetup &setup ) : m_setup(setup)
	{
		// create framebuffer (A framebuffer object is really just a container that attaches textures and renderbuffers to itself to create a complete configuration needed by the renderer.)
		glGenFramebuffersEXT(1, &fboId);
		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);


		// create depth buffer and attach it to FBO ==========================
		// create render buffer for depth (A renderbuffer is a simple 2D graphics image in a specified format. This format usually is defined as color, depth or stencil data.)
		glGenRenderbuffersEXT(1, &m_depthbufferId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbufferId);


		// initialize renderbuffer so that it can hold depth ( glRenderbufferStorage — establish data storage, format and dimensions of a renderbuffer object's image -> create depthmap buffer)
		if( !m_setup.isMultisampled() )
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_setup.getWidth(), m_setup.getHeight());
		else
			glRenderbufferStorageMultisample( GL_RENDERBUFFER_EXT, m_setup.getNumSamples(), GL_DEPTH_COMPONENT, m_setup.getWidth(), m_setup.getHeight() );

		// attach depth renderbuffer it to fbo
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthbufferId);

		// FBO is incomplete at this point because we havent created and attached a colorbuffer. This is done
		// in setOutputs and we use glFramebufferTexture2DEXT


		// attach outputs since FBOSetup provides us with all attachments =============
		if( !setup.m_outputs.empty() )
		{
			int i = 0;
			for( std::vector<Texture2dPtr>::const_iterator it = setup.m_outputs.begin(); it != setup.m_outputs.end(); ++it, ++i )
			{
				Texture2dPtr out = *it;
				// attach texture to framebuffer
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, out->m_target, out->m_id, 0);
			}
		}else
		if( setup.m_textureArrayOutputs )
		{
			// make framebuffer active
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

			glBindTexture( GL_TEXTURE_2D_ARRAY, setup.m_textureArrayOutputs->m_id );

			if( setup.m_textureArrayOutputs->m_zres > 4 )
				std::cout << "FBO::setOutputs( Texture2dArrayPtr out ) warning current only 4 layers supported \n";
			int numLayers = setup.getNumOutputs();
			for( int i = 0; i < numLayers; ++i )
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, setup.m_textureArrayOutputs->m_id, 0, i );

			// unbind framebuffer
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}else
		{
			// error, no outputs specified
		}

		//  finalize ===============
		finalize();

		// unbind framebuffer
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}


	FBO::FBO( int _width, int _height, bool multisampled, int numSamples )
	{
		m_setup.width(_width);
		m_setup.height(_height);
		m_setup.multisample(multisampled);
		m_setup.numSamples(numSamples);

		// create framebuffer (A framebuffer object is really just a container that attaches textures and renderbuffers to itself to create a complete configuration needed by the renderer.)
		glGenFramebuffersEXT(1, &fboId);
		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);


		// create depth buffer and attach it to FBO ==========================
		// create render buffer for depth (A renderbuffer is a simple 2D graphics image in a specified format. This format usually is defined as color, depth or stencil data.)
		glGenRenderbuffersEXT(1, &m_depthbufferId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbufferId);


		// initialize renderbuffer so that it can hold depth ( glRenderbufferStorage — establish data storage, format and dimensions of a renderbuffer object's image -> create depthmap buffer)
		if( !m_setup.isMultisampled() )
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_setup.getWidth(), m_setup.getHeight());
		else
			glRenderbufferStorageMultisample( GL_RENDERBUFFER_EXT, m_setup.getNumSamples(), GL_DEPTH_COMPONENT, m_setup.getWidth(), m_setup.getHeight() );

		// attach depth renderbuffer it to fbo
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthbufferId);

		// FBO is incomplete at this point because we havent created and attached a colorbuffer. This is done
		// in setOutputs and we use glFramebufferTexture2DEXT
	}


	FBO::FBOSetup FBO::create()
	{
		return FBOSetup();
	}


	void FBO::setOutputs( Texture2dPtr out0, Texture2dPtr out1, Texture2dPtr out2, Texture2dPtr out3 )
	{
		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		Texture2dPtr out[4] = { out0, out1, out2, out3 };

		m_setup.m_outputs.clear();
		for( int i=0; i<4;++i )
			if( out[i] )
				m_setup.attach( out[i] );

		int i = 0;
		for( std::vector<Texture2dPtr>::const_iterator it = m_setup.m_outputs.begin(); it != m_setup.m_outputs.end(); ++it, ++i )
		{
			Texture2dPtr out = *it;
			// attach texture to framebuffer
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, out->m_target, out->m_id, 0);
		}
		// unbind framebuffer
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	void FBO::setOutputs( Texture2dArrayPtr out )
	{
		m_setup.attach( out );

		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		glBindTexture( GL_TEXTURE_2D_ARRAY, out->m_id );

		int numLayers = m_setup.getNumOutputs();
        for( int i = 0; i < numLayers; ++i )
            glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, out->m_id, 0, i );

		// unbind framebuffer
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}


	void FBO::setSize( int newXres, int newYres )
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbufferId);
		if( !m_setup.isMultisampled() )
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, newXres, newYres);
		else
			glRenderbufferStorageMultisample( GL_RENDERBUFFER_EXT, m_setup.getNumSamples(), GL_DEPTH_COMPONENT, newXres, newYres );


		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		m_setup.width(newXres);
		m_setup.height(newYres);
	}


	void FBO::finalize()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch (status)
		{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				std::cout << "FBO successfull finalized\n";
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				std::cout << "FBO error: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				std::cout << "FBO error: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				std::cout << "FBO error: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				std::cout << "FBO error: GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				std::cout << "FBO error: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				std::cout << "FBO error: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT" << std::endl;
			break;
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				std::cout << "FBO error: GL_FRAMEBUFFER_UNSUPPORTED_EXT" << std::endl;
			break;
			default:
				std::cout << "FBO error: Unknown error" << std::endl;
			break;
		}

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	void FBO::begin( unsigned int clearBits )
	{
		// make framebuffer active
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		// save current states
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,m_setup.getWidth(), m_setup.getHeight());

		// Set the render target
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT };
		glDrawBuffers(m_setup.getNumOutputs(), buffers);

		if( clearBits )
		{
			glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
			glClear( clearBits );
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
