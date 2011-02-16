#pragma once

#include <map>


#ifdef _WINDOWS
#include <windows.h>

#include "Window.h"

namespace base
{


	class Application
	{
	public:
		Application();
		int                                              exec(); // starts the main application loop

		static void            registerWindow( Window *window );
		static Window         *getRegisteredWindow( HWND hwnd );
		static LRESULT CALLBACK windowHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	private:
		static bool                                      m_quit; // indicates if the app has to be closed
		static std::map< HWND, Window* >       m_windowRegister;

		
	};
}

#endif

#ifdef linux

#include <X11/Xlib.h>
#include "Window.h"

namespace base
{


	class Application
	{
	public:
		Application();
		int                                              exec(); // starts the main application loop

		static Display                            *getDisplay(); //
		static void            registerWindow( Window *window );
		static Window         *getRegisteredWindow( HWND hwnd );
		//static LRESULT CALLBACK windowHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	private:
		static bool                                      m_quit; // indicates if the app has to be closed
		static std::map< HWND, Window* >       m_windowRegister;
		static Display                               *m_display; // display server


	};
}

#endif
