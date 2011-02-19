#include "Application.h"
#include <iostream>

namespace base
{
	#ifdef _WINDOWS

	std::map< HWND, Window* > Application::m_windowRegister;
	bool Application::m_quit = false;

	Application::Application()
	{
	}

	int Application::exec()
	{
		// Main message loop:
		while( !m_quit ) 
		{
			MSG msg;
			if( PeekMessage( &msg,NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// repaint all windows which need repainting
			for( WindowRegister::iterator it = m_windowRegister.begin(); it != m_windowRegister.end(); ++it)
			{
				Window *w = it->second;

				if(w->needsRepaint())
					w->paint();
			}
		};
		return 0;
	}

	void Application::registerWindow( Window *window )
	{
		m_windowRegister[window->getHandle()] = window;
	}

	Window *Application::getRegisteredWindow( HWND hwnd )
	{
		std::map< HWND, Window* >::iterator it = m_windowRegister.find( hwnd );
		if( it != m_windowRegister.end() )
			return it->second;
		return 0;
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
				//PostQuitMessage(0);
				m_quit = true;
				return 0;
			}break;
		case WM_KEYDOWN: // a key has been pressed
			switch( wParam )
			{
			case VK_ESCAPE:
				//PostQuitMessage(0);
				m_quit = true;
				break;
			case 0 :
				break;
			case VK_RETURN:
				{
				}break;
			case VK_SPACE:
				{
				}break;
			case VK_UP :
				break;
			case VK_DOWN :
				break;
			case VK_LEFT :
				{
				}break;
			case VK_RIGHT :
				{
				}break;
			case VK_NUMPAD4 :
				break;
			case VK_NUMPAD6 :
				break;
			case VK_NUMPAD5 :
				break;
			case VK_NUMPAD8 :
				break;
			case VK_NUMPAD9 :
				break;
			case VK_NUMPAD3 :
				break;
			case VK_NUMPAD1 :
				break;
			case VK_ADD :
				break;
			case VK_SUBTRACT :
				break;
			case 70 :// KEY : 'f'
				break;
			case 82 :// KEY : 'r'
				break;
			case 68 :// KEY : 'd'
				{
				}break;
			case 86 :// KEY : 'v'
				break;
			case 71 :// KEY : 'g'
				break;
			case 49 : // 1 key
				break;
			case 50 : // 2 key
				break;
			case 51 : // 3 key
				break;
			case 52 : // 4 key
				break;
			default:
				return DefWindowProc( hWnd, uMsg, wParam, lParam );
				break;
			}
			break;

		case WM_MOUSEMOVE :
			{
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
			//queryState();
			//done = intro_do();

			//wininfo.events.keyb.state[KEY_LEFT]     = 0;
			//wininfo.events.keyb.state[KEY_RIGHT]    = 0;
			//wininfo.events.keyb.state[KEY_UP]       = 0;
			//wininfo.events.keyb.state[KEY_DOWN]     = 0;
			//wininfo.events.keyb.state[KEY_PGUP]     = 0;

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
				};
			}

			// repaint all windows which need repainting
			for( WindowRegister::iterator it = m_windowRegister.begin(); it != m_windowRegister.end(); ++it)
			{
				Window *w = it->second;

				if(w->needsRepaint())
					w->paint();
			}
		}
		return 0;
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

		//Atom wm_delete_window = XInternAtom( m_display, "WM_DELETE_WINDOW", false); // this means we want to handle window deletion on our own
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

#endif

}





