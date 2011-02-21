#include "Application.h"
#include <iostream>

namespace base
{
	#ifdef _WINDOWS

	std::map< HWND, Window* > Application::m_windowRegister;
	bool Application::m_quit = false;
	EventInfo Application::m_eventInfo;

	Application::Application()
	{
	}

	int Application::exec()
	{
		// Main message loop:
		while( !m_quit ) 
		{
			// update eventinfo
			updateEventInfo();

			MSG msg;
			while( PeekMessage( &msg,NULL,0,0,PM_REMOVE) )
			{
				if (msg.message == WM_QUIT)
				{
					m_quit = true;
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// repaint all windows which need repainting
			for( WindowRegister::iterator it = m_windowRegister.begin(); it != m_windowRegister.end(); ++it)
			{
				Window *w = it->second;
				w->paint();
			}
		};
		return 0;
	}

	//
	// stops the app
	//
	void Application::quit()
	{
		PostQuitMessage(0);
	}

	void Application::registerWindow( Window *window )
	{
		m_windowRegister[window->getHandle()] = window;
	}

	void Application::unregisterWindow( Window *window )
	{
		m_windowRegister.erase( window->getHandle() );
	}

	Window *Application::getRegisteredWindow( HWND hwnd )
	{
		std::map< HWND, Window* >::iterator it = m_windowRegister.find( hwnd );
		if( it != m_windowRegister.end() )
			return it->second;
		return 0;
	}

	EventInfo &Application::eventInfo()
	{
		return m_eventInfo;
	}

	void Application::updateEventInfo()
	{
		POINT   p;
		int     i;

		m_eventInfo.keyb.state[KEY_LEFT]     = GetAsyncKeyState( VK_LEFT );
		m_eventInfo.keyb.state[KEY_RIGHT]    = GetAsyncKeyState( VK_RIGHT );
		m_eventInfo.keyb.state[KEY_UP]       = GetAsyncKeyState( VK_UP );
		m_eventInfo.keyb.state[KEY_PGUP]     = GetAsyncKeyState( VK_PRIOR );
		m_eventInfo.keyb.state[KEY_PGDOWN]   = GetAsyncKeyState( VK_NEXT );
		m_eventInfo.keyb.state[KEY_DOWN]     = GetAsyncKeyState( VK_DOWN );

		m_eventInfo.keyb.state[KEY_ESCAPE]   = GetAsyncKeyState( VK_ESCAPE );
		m_eventInfo.keyb.state[KEY_RETURN]   = GetAsyncKeyState( VK_RETURN );

		m_eventInfo.keyb.state[KEY_SPACE]    = GetAsyncKeyState( VK_SPACE );
		m_eventInfo.keyb.state[KEY_RSHIFT]   = GetAsyncKeyState( VK_RSHIFT );
		m_eventInfo.keyb.state[KEY_RCONTROL] = GetAsyncKeyState( VK_RCONTROL );
		m_eventInfo.keyb.state[KEY_LSHIFT]   = GetAsyncKeyState( VK_LSHIFT );
		m_eventInfo.keyb.state[KEY_LCONTROL] = GetAsyncKeyState( VK_LCONTROL );
		m_eventInfo.keyb.state[KEY_1]        = GetAsyncKeyState( '1' );
		m_eventInfo.keyb.state[KEY_2]        = GetAsyncKeyState( '2' );
		m_eventInfo.keyb.state[KEY_3]        = GetAsyncKeyState( '3' );
		m_eventInfo.keyb.state[KEY_4]        = GetAsyncKeyState( '4' );
		m_eventInfo.keyb.state[KEY_5]        = GetAsyncKeyState( '5' );
		m_eventInfo.keyb.state[KEY_6]        = GetAsyncKeyState( '6' );
		m_eventInfo.keyb.state[KEY_7]        = GetAsyncKeyState( '7' );
		m_eventInfo.keyb.state[KEY_8]        = GetAsyncKeyState( '8' );
		m_eventInfo.keyb.state[KEY_9]        = GetAsyncKeyState( '9' );
		m_eventInfo.keyb.state[KEY_0]        = GetAsyncKeyState( '0' );
		for( i=KEY_A; i<=KEY_Z; i++ )
			m_eventInfo.keyb.state[i] = GetAsyncKeyState( 'A'+i-KEY_A );

		//-------
		GetCursorPos( &p );

		m_eventInfo.mouse.ox = m_eventInfo.mouse.x;
		m_eventInfo.mouse.oy = m_eventInfo.mouse.y;
		m_eventInfo.mouse.x = p.x;
		m_eventInfo.mouse.y = p.y;
		m_eventInfo.mouse.dx =  m_eventInfo.mouse.x - m_eventInfo.mouse.ox;
		m_eventInfo.mouse.dy =  m_eventInfo.mouse.y - m_eventInfo.mouse.oy;

		m_eventInfo.mouse.obuttons[0] = m_eventInfo.mouse.buttons[0];
		m_eventInfo.mouse.obuttons[1] = m_eventInfo.mouse.buttons[1];
		m_eventInfo.mouse.buttons[0] = GetAsyncKeyState(VK_LBUTTON);
		m_eventInfo.mouse.buttons[1] = GetAsyncKeyState(VK_RBUTTON);
		m_eventInfo.mouse.buttons[2] = GetAsyncKeyState(VK_MBUTTON);

		m_eventInfo.mouse.dbuttons[0] = m_eventInfo.mouse.buttons[0] - m_eventInfo.mouse.obuttons[0];
		m_eventInfo.mouse.dbuttons[1] = m_eventInfo.mouse.buttons[1] - m_eventInfo.mouse.obuttons[1];
	}

	//
	// WinAPI message handler
	//
	LRESULT CALLBACK Application::windowHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		Window *w = getRegisteredWindow( hWnd );

		switch( uMsg )
		{
		case WM_CLOSE:								
			{
				w->destroy();
				return 0;
			}break;
		case WM_DESTROY:
			{
				unregisterWindow(w);

				// we assume we are in gui mode all the time (gui mode means, when there is no window the app closes)
				if(1)
					if( m_windowRegister.empty() )
						PostQuitMessage(0);
				return 0;
			}break;
		case WM_KEYDOWN: // a key has been pressed
		{
			// update central eventinfo
			EventInfo &ei = Application::eventInfo();
			int conv = 0;
			switch( wParam )
			{
				case VK_LEFT:     conv = KEY_LEFT;     break;
				case VK_RIGHT:    conv = KEY_RIGHT;    break;
				case VK_UP:       conv = KEY_UP;       break;
				case VK_PRIOR:    conv = KEY_PGUP;     break;
				case VK_NEXT:     conv = KEY_PGDOWN;   break;
				case VK_DOWN:     conv = KEY_DOWN;     break;
				case VK_SPACE:    conv = KEY_SPACE;    break;
				case VK_RSHIFT:   conv = KEY_RSHIFT;   break;
				case VK_RCONTROL: conv = KEY_RCONTROL; break;
				case VK_LSHIFT:   conv = KEY_LSHIFT;   break;
				case VK_LCONTROL: conv = KEY_LCONTROL; break;
				case VK_ESCAPE: conv = KEY_ESCAPE; break;
				case VK_RETURN: conv = KEY_RETURN; break;
			}
        
			for( int i=KEY_A; i<=KEY_Z; i++ )
			{
				if( wParam==(WPARAM)('A'+i-KEY_A) )
					conv = i;
				if( wParam==(WPARAM)('a'+i-KEY_A) )
					conv = i;
			}

			ei.keyb.press[conv] = 1;
			return( DefWindowProc(hWnd,uMsg,wParam,lParam) );
		}break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
		}break;
		case WM_MOUSEMOVE :
		{
			/*
			int xPos = GET_X_LPARAM(lParam); 
			int yPos = GET_Y_LPARAM(lParam);

			m_eventInfo.mouse.ox = m_eventInfo.mouse.x;
			m_eventInfo.mouse.oy = m_eventInfo.mouse.y;
			m_eventInfo.mouse.x = xPos;
			m_eventInfo.mouse.y = yPos;
			m_eventInfo.mouse.dx =  m_eventInfo.mouse.x - m_eventInfo.mouse.ox;
			m_eventInfo.mouse.dy =  m_eventInfo.mouse.y - m_eventInfo.mouse.oy;
			//w->event();
			return( DefWindowProc(hWnd,uMsg,wParam,lParam) );
			*/
		}break;
		default:
			return DefWindowProc( hWnd, uMsg, wParam, lParam );   
		}


		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
#endif

#ifdef linux

	std::map< HWND, Window* > Application::m_windowRegister;
	bool Application::m_quit = false;
	Display *Application::m_display = 0;
	EventInfo Application::m_eventInfo;

	Atom Application::m_deleteWindowAtom = 0;

	Application::Application()
	{
	}

	Application::~Application()
	{
		if(m_display)
		{
			XCloseDisplay(m_display);
			m_display = 0;
		}
	}

	int Application::exec()
	{
		Display *display = getDisplay();
		XEvent event;

		if( !display )
			return 1;

		while( !m_quit )
		{
			// update eventinfo
			updateEventInfo();

			while( XPending(display) )
			{
				XNextEvent( display, &event );

				// handle event
				switch( event.type )
				{
					case ClientMessage:
					{
						Atom clientMsg = (Atom)event.xclient.data.l[0];
						if(clientMsg == m_deleteWindowAtom)
						{
							// destroy window
							getRegisteredWindow( event.xclient.window )->destroy();
						}
					}break;
					case DestroyNotify:
					{
						Window *w = getRegisteredWindow( event.xdestroywindow.window );
						unregisterWindow(w);

						// we assume we are in gui mode all the time (gui mode means, when there is no window the app closes)
						if(1)
							if( m_windowRegister.empty() )
								m_quit = true;
					}break;
					case KeyPress:
					{
						switch( XKeycodeToKeysym( display, event.xkey.keycode, 0 ) )
						{
						case XK_Up:
							m_eventInfo.keyb.press[KEY_UP] = 1;break;
						case XK_Down:
							m_eventInfo.keyb.press[KEY_DOWN] = 1;break;
						case XK_Left:
							m_eventInfo.keyb.press[KEY_LEFT] = 1;break;
						case XK_Right:
							m_eventInfo.keyb.press[KEY_RIGHT] = 1;break;
						case XK_Escape:
							m_eventInfo.keyb.press[KEY_ESCAPE] = 1;break;
						};
					}break;
				};
			}

			// repaint all windows which need repainting
			for( WindowRegister::iterator it = m_windowRegister.begin(); it != m_windowRegister.end(); ++it)
			{
				Window *w = it->second;
				w->paint();
			}
		}
		return 0;
	}

	void Application::updateEventInfo()
	{
		Display *display = getDisplay();
		HWND root_return;
		HWND child_return;
		int root_x_return;
		int root_y_return;
		int win_x_return;
		int win_y_return;
		unsigned int mask_return;

		int defaultScreen = DefaultScreen(display);
		HWND rootWindow = RootWindow( display, defaultScreen );

		bool result = XQueryPointer( display, rootWindow, &root_return, &child_return,  &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);

		m_eventInfo.keyb.press[KEY_LEFT]     = 0;
		m_eventInfo.keyb.press[KEY_RIGHT]    = 0;
		m_eventInfo.keyb.press[KEY_UP]       = 0;
		m_eventInfo.keyb.press[KEY_DOWN]     = 0;
		m_eventInfo.keyb.press[KEY_PGUP]     = 0;

		if (result)
		{
			m_eventInfo.mouse.ox = m_eventInfo.mouse.x;
			m_eventInfo.mouse.oy = m_eventInfo.mouse.y;
			m_eventInfo.mouse.x = win_x_return;
			m_eventInfo.mouse.y = win_y_return;
			m_eventInfo.mouse.dx =  m_eventInfo.mouse.x - m_eventInfo.mouse.ox;
			m_eventInfo.mouse.dy =  m_eventInfo.mouse.y - m_eventInfo.mouse.oy;

			m_eventInfo.mouse.obuttons[0] = m_eventInfo.mouse.buttons[0];
			m_eventInfo.mouse.obuttons[1] = m_eventInfo.mouse.buttons[1];
			m_eventInfo.mouse.buttons[0] = (mask_return & 256)?1:0;
			m_eventInfo.mouse.buttons[2] = (mask_return & 512)?1:0;
			m_eventInfo.mouse.buttons[1] = (mask_return & 1024)?1:0;

			m_eventInfo.mouse.dbuttons[0] = m_eventInfo.mouse.buttons[0] - m_eventInfo.mouse.obuttons[0];
			m_eventInfo.mouse.dbuttons[1] = m_eventInfo.mouse.buttons[1] - m_eventInfo.mouse.obuttons[1];
		}
	}

	Display *Application::getDisplay()
	{
		if(!m_display)
		{
			m_display = XOpenDisplay(NULL);
			m_deleteWindowAtom = XInternAtom( m_display, "WM_DELETE_WINDOW", false); // this means we want to handle window deletion on our own
		}
		return m_display;
	}

	void Application::registerWindow( Window *window )
	{
		m_windowRegister[window->getHandle()] = window;

		// we use this place to tell the window which protocols we are supporting (e.g which events we want to handle etc.)
		XSetWMProtocols( m_display, window->getHandle(), &m_deleteWindowAtom, 1);
		XFlush( m_display );
	}

	void Application::unregisterWindow( Window *window )
	{
		m_windowRegister.erase( window->getHandle() );
	}

	Window *Application::getRegisteredWindow( HWND hwnd )
	{
		std::map< HWND, Window* >::iterator it = m_windowRegister.find( hwnd );
		if( it != m_windowRegister.end() )
			return it->second;
		return 0;
	}

	EventInfo &Application::eventInfo()
	{
		return m_eventInfo;
	}

	//
	// stops the app
	//
	void Application::quit()
	{
		m_quit = true;
	}

#endif

}





