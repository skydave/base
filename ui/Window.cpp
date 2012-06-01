#ifdef _WINDOWS
#include "Window.h"
#include "Application.h"
#include <string>



namespace base
{

	Window::Window() : m_hwnd(0), m_keypress(0)
	{
	}

	void Window::setKeyPressCallback( KeyPressCallback keyPress )
	{
		m_keypress = keyPress;
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

	void Window::setCaption( std::string caption )
	{
		// convert std::wstring to std::string
		//m_caption = std::wstring( caption.begin(), caption.end() );
		m_caption = caption;
		SetWindowText( m_hwnd, m_caption.c_str());
	}

	void Window::setSize( int width, int height )
	{
		m_width = width;
		m_height = height;
	}


	std::string getRegisteredWndClass()
	{
		return "mf window class";
	}

	// will return the winappinstance
	HINSTANCE getWinAppInstance()
	{
		return GetModuleHandle(NULL);
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
