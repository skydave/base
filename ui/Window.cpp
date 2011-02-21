#ifdef _WINDOWS
#include "Window.h"
#include "Application.h"
#include <string>



namespace base
{

	Window::Window() : m_hwnd(0)
	{
	}
	HWND Window::getHandle()
	{
		return m_hwnd;
	}

	void Window::paint()
	{
	}



	//
	// shows the window
	//
	void Window::show()
	{
		ShowWindow( m_hwnd , SW_SHOW );
		UpdateWindow( m_hwnd );
		SetForegroundWindow( m_hwnd );
		SetFocus( m_hwnd );
	}

	//
	// closes the window
	//
	void Window::destroy()
	{
		DestroyWindow(m_hwnd);
	}

}
#endif








#ifdef linux

#include "GLWindow.h"
#include <string>


namespace base
{
	Window::Window() : m_hwnd(0)
	{
	}

	Window::~Window()
	{
	}

	HWND Window::getHandle()
	{
		return m_hwnd;
	}


	void Window::paint()
	{
	}

	//
	// shows the window
	//
	void Window::show()
	{
		Display *display = Application::getDisplay();
		XMapWindow( display, m_hwnd );
	}

	//
	// closes the window
	//
	void Window::destroy()
	{
		Display *display = Application::getDisplay();
		XUnmapWindow(display, m_hwnd);
		XDestroyWindow(display, m_hwnd);
	}



}

#endif
