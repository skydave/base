

#include "GLExtensions.h"
#include "GLTemporaryContext.h"
#include <gltools\gl.h>


namespace base
{
	bool WGLisExtensionSupported(const char *extension)
	{
		const size_t extlen = strlen(extension);
		const char *supported = NULL;
 
		// Try To Use wglGetExtensionStringARB On Current DC, If Possible
		PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
 
		if (wglGetExtString)
			supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());
 
		// If That Failed, Try Standard Opengl Extensions String
		if (supported == NULL)
			supported = (char*)glGetString(GL_EXTENSIONS);
 
		// If That Failed Too, Must Be No Extensions Supported
		if (supported == NULL)
			return false;
 
		// Begin Examination At Start Of String, Increment By 1 On False Match
		for (const char* p = supported; ; p++)
		{
			// Advance p Up To The Next Possible Match
			p = strstr(p, extension);
 
			if (p == NULL)
			return false;                       // No Match
 
			// Make Sure That Match Is At The Start Of The String Or That
			// The Previous Char Is A Space, Or Else We Could Accidentally
			// Match "wglFunkywglExtension" With "wglExtension"
 
			// Also, Make Sure That The Following Character Is Space Or NULL
			// Or Else "wglExtensionTwo" Might Match "wglExtension"
			if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
				return true;                        // Match
		}
	}

	//
	// Returns the GL extensions for the current QGLContext. If there is no
    // current QGLContext, a default context will be created and the extensions
    // for that context will be returned instead.
	//
	// WARN: assumes a gl context is currently active
	GLExtensions::Extensions GLExtensions::glExtensions()
	{
		Extensions glExtensions = 0;

		if (WGLisExtensionSupported("GL_ARB_texture_rectangle"))
			glExtensions |= TextureRectangle;
		if (WGLisExtensionSupported("GL_ARB_multisample"))
			glExtensions |= SampleBuffers;
		if (WGLisExtensionSupported("GL_SGIS_generate_mipmap"))
			glExtensions |= GenerateMipmap;
		if (WGLisExtensionSupported("GL_ARB_texture_compression"))
			glExtensions |= TextureCompression;
		if (WGLisExtensionSupported("GL_EXT_texture_compression_s3tc"))
			glExtensions |= DDSTextureCompression;
		if (WGLisExtensionSupported("GL_OES_compressed_ETC1_RGB8_texture"))
			glExtensions |= ETC1TextureCompression;
		if (WGLisExtensionSupported("GL_IMG_texture_compression_pvrtc"))
			glExtensions |= PVRTCTextureCompression;
		if (WGLisExtensionSupported("GL_ARB_fragment_program"))
			glExtensions |= FragmentProgram;
		if (WGLisExtensionSupported("GL_ARB_fragment_shader"))
			glExtensions |= FragmentShader;
		if (WGLisExtensionSupported("GL_ARB_shader_objects"))
			glExtensions |= FragmentShader;
		if (WGLisExtensionSupported("GL_ARB_texture_mirrored_repeat"))
			glExtensions |= MirroredRepeat;
		if (WGLisExtensionSupported("GL_EXT_framebuffer_object"))
			glExtensions |= FramebufferObject;
		if (WGLisExtensionSupported("GL_EXT_stencil_two_side"))
			glExtensions |= StencilTwoSide;
		if (WGLisExtensionSupported("GL_EXT_stencil_wrap"))
			glExtensions |= StencilWrap;
		if (WGLisExtensionSupported("GL_EXT_packed_depth_stencil"))
			glExtensions |= PackedDepthStencil;
		if (WGLisExtensionSupported("GL_NV_float_buffer"))
			glExtensions |= NVFloatBuffer;
		if (WGLisExtensionSupported("GL_ARB_pixel_buffer_object"))
			glExtensions |= PixelBufferObject;
		if (WGLisExtensionSupported("GL_IMG_texture_format_BGRA8888") || WGLisExtensionSupported("GL_EXT_texture_format_BGRA8888"))
			glExtensions |= BGRATextureFormat;

		if (WGLisExtensionSupported("GL_ARB_framebuffer_object"))
		{
			// ARB_framebuffer_object also includes EXT_framebuffer_blit.
			glExtensions |= FramebufferObject;
			glExtensions |= FramebufferBlit;
		}

		if (WGLisExtensionSupported("GL_EXT_framebuffer_blit"))
			glExtensions |= FramebufferBlit;

		if (WGLisExtensionSupported("GL_ARB_texture_non_power_of_two"))
			glExtensions |= NPOTTextures;

		if (WGLisExtensionSupported("GL_EXT_bgra"))
			glExtensions |= BGRATextureFormat;

		//{
		//	GLboolean srgbCapableFramebuffers;
		//	glGetBooleanv(FRAMEBUFFER_SRGB_CAPABLE_EXT, &srgbCapableFramebuffers);
		//	if (srgbCapableFramebuffers)
		//	glExtensions |= SRGBFrameBuffer;
		//	// Clear possible error which is generated if
		//	// FRAMEBUFFER_SRGB_CAPABLE_EXT isn't supported.
		//	glGetError();
		//}


		return glExtensions;
	}

	//GLExtensions::Extensions GLExtensions::currentContextExtensions()
	//{
	//}

}