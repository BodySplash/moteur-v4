#include "CRenderer.h"
#include "d3dtools.h"

//constantes

void GameLoop();
const char* WINDOWCLASS = "Etherlink";
const char* WINDOWTITLE = "Etherlink Beta 1";
CRenderer game;
//IDirect3DSurface8 img;
LRESULT CALLBACK TheWindowProc ( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
{
	switch (uMsg)
	{

	case WM_LBUTTONDOWN:{
		game._render= 1;
		POINT mouse;
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		game.mouseMapper(mouse);
		
					  }
					  break;


	case WM_KEYDOWN:	//a key has been pressed
		{
			
			//if escape pressed, destroy window
			if ( wParam == VK_ESCAPE ) {
				DestroyWindow ( hWnd ) ;
				return (0);
			}
			else {
				game._render = 1;

				if (wParam == VK_UP) game.ptScreenAnchor.y+=15;
				if (wParam == VK_DOWN) game.ptScreenAnchor.y-=15;
				if (wParam == VK_LEFT) game.ptScreenAnchor.x+=30;
				if (wParam == VK_RIGHT) game.ptScreenAnchor.x-=30;
			}

		} break ;
	case WM_DESTROY : //window being destroyed
		{
			//quit

			PostQuitMessage ( 0 ) ;

			//message handled, return 0

			return ( 0 ) ;

		} break ;
	default:  //all other messages, send to default handler
		return ( DefWindowProc ( hWnd , uMsg , wParam , lParam ) ) ;
	}
}





int WINAPI WinMain ( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nShowCmd )
{
	//grab instance handle
	hInstance = hInstance ;

	//fill in window class
	WNDCLASSEX wc ;
	wc.cbClsExtra = 0 ;	//no extra class information
	wc.cbSize = sizeof ( WNDCLASSEX ) ; //size of structure
	wc.cbWndExtra = 0 ;	//no extra window information
	wc.hbrBackground = ( HBRUSH ) GetStockObject ( BLACK_BRUSH ) ;	//black brush
	wc.hCursor = NULL ;	//no cursor
	wc.hIcon = NULL ;	//no icon
	wc.hIconSm = NULL ;	//no small icon
	wc.hInstance = hInstance ;	//instance handle
	wc.lpfnWndProc = TheWindowProc ;	//window procedure
	wc.lpszClassName = WINDOWCLASS ;	//name of class
	wc.lpszMenuName = NULL ;	//no menu
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC ;	//class styles

	//register window class
	RegisterClassEx ( &wc ) ;

	//create window
	mainHwnd = CreateWindowEx ( 0 , WINDOWCLASS , WINDOWTITLE , WS_OVERLAPPEDWINDOW , 0 , 0 , 640 , 480 , NULL , NULL , hInstance , NULL ) ;

	//show the window
	ShowWindow ( mainHwnd , nShowCmd ) ;

	//initialization
	if (!game.initD3D())
		PostQuitMessage(0);

	MSG msg ;

	//message pump
	for ( ; ; ) 
	{
		//check for a message
		if ( PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ) )
		{
			//message exists

			//check for quit message
			if ( msg.message == WM_QUIT ) break ;

			//translate the message
			TranslateMessage ( &msg ) ;

			//dispatch the message
			DispatchMessage ( &msg ) ;
		}
		else 
			GameLoop();
	}

	//clean up
	game.clearD3D();

	//exit
	return ( msg.wParam ) ;
}

void GameLoop() {
	game.renderFrame(NULL, NULL);
}
