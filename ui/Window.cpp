#ifdef _WINDOWS
#include "Window.h"
#include "Application.h"
#include <string>



namespace base
{

	Window::Window() : m_hwnd(0), m_needsRepaint(true)
	{
	}
	HWND Window::getHandle()
	{
		return m_hwnd;
	}
	bool Window::needsRepaint()
	{
		return m_needsRepaint;
	}
	void Window::update()
	{
		m_needsRepaint = true;
	}
	void Window::paint()
	{
		m_needsRepaint = false;
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
		//Display *display = Application::getDisplay();
		//XUnmapWindow(display, m_hwnd);
		//XDestroyWindow(display, m_hwnd);
	}

}
#endif








#ifdef linux

#include "GLWindow.h"
#include <string>


namespace base
{
	Window::Window() : m_hwnd(0), m_needsRepaint(true)
	{
	}

	Window::~Window()
	{
	}

	HWND Window::getHandle()
	{
		return m_hwnd;
	}
	bool Window::needsRepaint()
	{
		return m_needsRepaint;
	}
	void Window::update()
	{
		m_needsRepaint = true;
	}

	void Window::paint()
	{
		m_needsRepaint = false;
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
