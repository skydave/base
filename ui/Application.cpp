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

#ifdef _LINUX
#error "APPLICATION MAIN LOOP FOR LINUX HAS TO BE IMPLEMENTED"
#endif

}





