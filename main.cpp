#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "customfunc.h"
#include "type.h"
#include "data.h"
#include "paint.h"
#include "object.h"
#include "world.h"


int tx = 0, ty =0;


namespace test1
{
	data d;
	object o;
	world w;
	
	void start()
	{
		using namespace mtypelist;
		using namespace dtypelist;
		using namespace otypelist;
		
		value vbuf;
		
		vbuf.type = _x;
		vbuf.v = 320;
		
		d.type = _pdata;
		d.add(&vbuf);
		d.add(_y,190);
		
		
		value inputvalue[128];
		unsigned ivnum = 0;
		
		inputvalue[ivnum].type = _x;
		inputvalue[ivnum++].v = 230;
		
		w.adddata(&d);
		w.setdata_first(_enter,0,inputvalue,1);
		
		
		//obj test
		w.iniobj(_bullet,&o);
		
		ivnum = 0;
		inputvalue[ivnum].type = _rx;
		inputvalue[ivnum++].v = 300;
		inputvalue[ivnum].type = _ry;
		inputvalue[ivnum++].v = 200;
		inputvalue[ivnum].type = _destx;
		inputvalue[ivnum++].v = 540;
		inputvalue[ivnum].type = _desty;
		inputvalue[ivnum++].v = 40;
		inputvalue[ivnum].type = _movetype;
		inputvalue[ivnum++].v = _destination;
		inputvalue[ivnum].type = _speed;
		inputvalue[ivnum++].v = 0.7;
		inputvalue[ivnum].type = _size;
		inputvalue[ivnum++].v = 11;
		inputvalue[ivnum].type = _boundtype;
		inputvalue[ivnum++].v = _circle;
		inputvalue[ivnum].type = _boundlimit;
		inputvalue[ivnum++].v = 1;
		o.setdata(_enter,_pdata,inputvalue,ivnum);
		
		
		ivnum = 0;
		inputvalue[ivnum].type = __type;
		inputvalue[ivnum++].v = _dmg;
		inputvalue[ivnum].type = _power;
		inputvalue[ivnum++].v = 10;
		o.setdata(_add,_beffect,inputvalue,ivnum);
		
		w.addobj(&o);
		
		
		
		w.iniobj(_unit,&o);
		
		ivnum = 0;
		inputvalue[ivnum].type = _rx;
		inputvalue[ivnum++].v = 50;
		inputvalue[ivnum].type = _ry;
		inputvalue[ivnum++].v = 300;
		inputvalue[ivnum].type = _destx;
		inputvalue[ivnum++].v = 500;
		inputvalue[ivnum].type = _desty;
		inputvalue[ivnum++].v = 100;
		inputvalue[ivnum].type = _movetype;
		inputvalue[ivnum++].v = _destination;
		inputvalue[ivnum].type = _speed;
		inputvalue[ivnum++].v = 1.2;
		inputvalue[ivnum].type = _size;
		inputvalue[ivnum++].v = 15;
		o.setdata(_enter,_pdata,inputvalue,ivnum);
		
		ivnum = 0;
		inputvalue[ivnum].type = _hp;
		inputvalue[ivnum++].v = 100;
		inputvalue[ivnum].type = _life;
		inputvalue[ivnum++].v = 100;
		o.setdata(_enter,_status,inputvalue,ivnum);
		
		w.addobj(&o);
		
		
	}
		
	
	void paint(HDC dc)
	{
		char str[128];
		
		using namespace mtypelist;
		getname(_msg,gettype(_msg,"y"),str);
		
		TextOutA(dc,0,0,str,strlen(str));
		
		using namespace mtypelist;
		using namespace dtypelist;
		data *dbuf = w.getdatapfirst(_pdata);
		sprintf(str,"%d",(int)dbuf->getvfirst(_x));
		TextOutA(dc,0,30,str,strlen(str));
		
		
		figure f(_circle,400,200,20,RGB(210,80,255));
		f.paint(dc);
		
		
		w.paint(dc);
	}
	
	void testproc()
	{
		w.proc();
	}
};



void testpaint(HDC dc)
{
	Ellipse(dc,tx-4,ty-4,tx+4,ty+4);
	tx+=2; ty+=2;
	test1::paint(dc);
}

void testproc()
{
	test1::testproc();
}

RECT crt; // crt
HDC hdc,hMemDC;
PAINTSTRUCT ps;
HBRUSH BGBrush,OldBrush;
HBITMAP hBitmap, OldBitmap;
HPEN hPen,OldPen;




clock_t fstart;
int fps;
int waittime = 0,tooktime = 0;


/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		case WM_CREATE :
			{
				srand(unsigned(time(NULL)));
				GetClientRect(hwnd,&crt);

				SetTimer(hwnd,0,1,NULL);
				fps = 60;
				
				
				test1::start();
			}
		break;
		
		case WM_TIMER :
			switch(wParam)
			{
				case 0 :
				{
					fstart = clock();
					
					//할거  
					InvalidateRect(hwnd,&crt,false); 
					testproc();
					//
					
					
					tooktime = clock() - fstart;
					waittime = (int)(1000/fps) - tooktime;
					if(waittime > 0)
					{
						Sleep(waittime);
					}
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
			
			BGBrush = CreateSolidBrush(RGB(40,255,40));		
			FillRect(hMemDC,&crt,BGBrush);
			DeleteObject(BGBrush);
			
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
