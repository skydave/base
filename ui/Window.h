#pragma once

#ifdef _WINDOWS
namespace base
{
	class Window
	{
	public:
		Window() : m_hwnd(NULL)
		{
		}
		HWND getHandle()
		{
			return m_hwnd;
		}
	protected:
		HWND                                            m_hwnd; // window handle
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
		Window() : m_hwnd(0)
		{
		}
		HWND getHandle()
		{
			return m_hwnd;
		}
	protected:
		HWND                                            m_hwnd; // window handle
		char                                    m_wndclass[11]; // window class and title :)
	};
}
#endif
