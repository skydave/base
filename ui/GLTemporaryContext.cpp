//
// Purpose of GLTemporaryContext is to allow execution of gl functions when no gl context is available
// e.g. checking for available gl extensions which we only can do with a gl context
//

#include "GLTemporaryContext.h"

namespace base
{
	GLTemporaryContext::GLTemporaryContext()
	{
		std::string windowClassName = getRegisteredWndClass();

		dmy_id = CreateWindow(windowClassName.c_str(),
								0, 0, 0, 0, 1, 1,
								0, 0, getWinAppInstance(), 0);

		dmy_pdc = GetDC(dmy_id);
		PIXELFORMATDESCRIPTOR dmy_pfd;
		memset(&dmy_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		dmy_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		dmy_pfd.nVersion = 1;
		dmy_pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		dmy_pfd.iPixelType = PFD_TYPE_RGBA;
		//if (!directRendering)
		//	dmy_pfd.dwFlags |= PFD_GENERIC_FORMAT;

		int dmy_pf = ChoosePixelFormat(dmy_pdc, &dmy_pfd);
		SetPixelFormat(dmy_pdc, dmy_pf, &dmy_pfd);
		dmy_rc = wglCreateContext(dmy_pdc);
		old_dc = wglGetCurrentDC();
		old_context = wglGetCurrentContext();
		wglMakeCurrent(dmy_pdc, dmy_rc);
	}

	GLTemporaryContext::~GLTemporaryContext()
	{
		wglMakeCurrent(dmy_pdc, 0);
		wglDeleteContext(dmy_rc);
		ReleaseDC(dmy_id, dmy_pdc);
		DestroyWindow(dmy_id);
		if (old_dc && old_context)
			wglMakeCurrent(old_dc, old_context);
	}
}