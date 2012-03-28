

#pragma once

#include <util/Flags.h>


namespace base
{
	// GL extension definitions
	struct GLExtensions
	{
		enum Extension
		{
			TextureRectangle        = 0x00000001,
			SampleBuffers           = 0x00000002,
			GenerateMipmap          = 0x00000004,
			TextureCompression      = 0x00000008,
			FragmentProgram         = 0x00000010,
			MirroredRepeat          = 0x00000020,
			FramebufferObject       = 0x00000040,
			StencilTwoSide          = 0x00000080,
			StencilWrap             = 0x00000100,
			PackedDepthStencil      = 0x00000200,
			NVFloatBuffer           = 0x00000400,
			PixelBufferObject       = 0x00000800,
			FramebufferBlit         = 0x00001000,
			NPOTTextures            = 0x00002000,
			BGRATextureFormat       = 0x00004000,
			DDSTextureCompression   = 0x00008000,
			ETC1TextureCompression  = 0x00010000,
			PVRTCTextureCompression = 0x00020000,
			FragmentShader          = 0x00040000,
			ElementIndexUint        = 0x00080000,
			Depth24                 = 0x00100000,
			SRGBFrameBuffer         = 0x00200000
		};
		typedef Flags<Extension> Extensions;

		static Extensions glExtensions();
		//static Extensions currentContextExtensions();
	};
}