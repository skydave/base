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
		typedef std::map< HWND, Window* > WindowRegister;
		Application();
		int                                              exec(); // starts the main application loop

		static void            registerWindow( Window *window );
		static void          unregisterWindow( Window *window );
		static Window         *getRegisteredWindow( HWND hwnd );
		static LRESULT CALLBACK windowHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	private:
		static bool                                      m_quit; // indicates if the app has to be closed
		static WindowRegister                  m_windowRegister;

		
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
		typedef std::map< HWND, Window* > WindowRegister;
		Application();
		~Application();
		int                                              exec(); // starts the main application loop

		static Display                            *getDisplay(); //
		static void            registerWindow( Window *window );
		static void          unregisterWindow( Window *window );
		static Window         *getRegisteredWindow( HWND hwnd );
		//static LRESULT CALLBACK windowHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	private:
		static bool                                      m_quit; // indicates if the app has to be closed
		static WindowRegister                  m_windowRegister;
		static Display                               *m_display; // display server

		static Atom                          m_deleteWindowAtom;


	};
}

#endif
