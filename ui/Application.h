//
// atm window focus is not taken into account -> all window events will update a central eventinfo structure
// a keypress event in one window will change eventinfo centrally but only the window which received the event
// will get its event method called
//
#pragma once

#include <map>


#ifdef _WINDOWS
#include <windows.h>

#include "Window.h"

#include "EventInfo.h"

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
		static EventInfo                           &eventInfo();
	private:
		void                                  updateEventInfo();
		static bool                                      m_quit; // indicates if the app has to be closed
		static WindowRegister                  m_windowRegister;
		static EventInfo                            m_eventInfo;

		
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
