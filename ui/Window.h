#pragma once

#ifdef _WINDOWS

#include <windows.h>

namespace base
{
	class Window
	{
	public:

		Window();
		HWND                                       getHandle();
		bool                                    needsRepaint();
		void                                          update();
		void                                            show(); // shows the window

		virtual void                                   paint();
		virtual void                                 destroy(); // closes and destroys the window

	protected:
		HWND                                            m_hwnd; // window handle
		bool                                    m_needsRepaint; // dirty flag
	};
}
#endif

#ifdef linux
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace base
{
	typedef Window HWND;

	class Window
	{
	public:
		Window();
		virtual ~Window();
		HWND                                       getHandle();
		bool                                    needsRepaint();
		void                                          update();
		void                                            show(); // shows the window

		virtual void                                   paint();
		virtual void                                 destroy(); // closes and destroys the window
	protected:
		HWND                                            m_hwnd; // window handle
		char                                    m_wndclass[11]; // window class and title :)
		bool                                    m_needsRepaint; // dirty flag
	};
}
#endif
