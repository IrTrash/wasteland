#include <windows.h>
#include <stdlib.h>
#include <time.h>


#include "data.h"
#include "object.h"
#include "world.h"


int tx = 0, ty =0;
void testpaint(HDC dc)
{
	Ellipse(dc,tx-4,ty-4,tx+4,ty+4);
}


RECT crt; // crt
HDC hdc,hMemDC;
PAINTSTRUCT ps;
HBRUSH BGBrush,OldBrush;
HBITMAP hBitmap, OldBitmap;
HPEN hPen,OldPen;

int	fcount = 0;
clock_t fstart;
int starttime1 = 0, starttime2 = 0 ,tooktime1 = 0, tooktime2 = 0;



/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		case WM_CREATE :
			{
				srand(unsigned(time(NULL)));
				GetClientRect(hwnd,&crt);
				SetTimer(hwnd, 0 , 1, NULL);
				starttime1 = clock();
				starttime2 = starttime1;
			}
		break;
		
		case WM_TIMER :
			switch(wParam)
			{
				case 0 :
					int currenttime = clock();
					tooktime1 = currenttime - starttime1;
					tooktime2 = currenttime - starttime2;
					
					
					if(tooktime1 >= _clock)
					{
						starttime1 = clock();
						//자료 처리 
						tx++;
						ty++;
		 				
					}
					
					if(tooktime2 >= int(1000/_fps))
					{
						starttime2 = clock();
						//출력 처리
						InvalidateRect(hwnd,&crt,false); 
					}
					
				break;

			}			
		break;
		
		
		case WM_PAINT :
		{
			hdc = BeginPaint(hwnd,&ps);
			hMemDC = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
			OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); //HBITMAP(SelectObject(hMemDC, hBitmap)); 이것도 됨.
			
			//출력할거
			testpaint(hMemDC);
			
			BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY);
			DeleteObject(SelectObject(hMemDC, OldBitmap));
			DeleteDC(hMemDC);
			EndPaint(hwnd, &ps); 
			
		}
		break;
		
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
