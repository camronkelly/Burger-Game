#pragma once

#ifdef _WIN32
#  define WINDOWS_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <GLScene.h>
#include <mainMenu.h>
#include <landingScene.h>
#include <helpScreen.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>		// Header File For Windows
#include <restaurantScene.h>
#include <chaseScene.h>
#include <sounds.h>

using namespace std;


enum gScenes { landing, mainmenu, game, fps, help, chase };

gScenes currScene = landing;
gScenes prevScene = landing;
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

int i = 0;
restaurantScene *fpsScene = new restaurantScene(&i);
landingScene *landScene = new landingScene();
mainMenu *menuScene = new mainMenu();
helpScreen *helpScene = new helpScreen();
GLScene *marioScene = new GLScene(&i);
ChaseScene *finalScene = new ChaseScene(&i);
sounds *mainSound = new sounds();


/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE KILL GL WINDOW
/////////////////////////////////////////////////////////////////////////////////////////////////
GLvoid KillGLWindow()								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CREATE GL WINDOW
/////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle= WS_POPUP;			// must handle Gsync situations: Windows Style
		ShowCursor(FALSE);									// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	if (currScene == landing) {
        landScene->resizeGLScene(width, height);			// Set Up Our Perspective GL Screen
        if (!landScene->initGL())							// Initialize Our Newly Created GL Window
        {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
        }
	}
	else if (currScene == mainmenu) {
        menuScene->resizeGLScene(width, height);			// Set Up Our Perspective GL Screen
        if (!menuScene->initGL())							// Initialize Our Newly Created GL Window
        {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
        }
	}
	else if (currScene == help) {
        helpScene->resizeGLScene(width, height);			// Set Up Our Perspective GL Screen
        if (!helpScene->initGL())							// Initialize Our Newly Created GL Window
        {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
        }
	}
	else if (currScene == game) {
        marioScene->resizeGLScene(width, height);			// Set Up Our Perspective GL Screen
        if (!marioScene->initGL())							// Initialize Our Newly Created GL Window
        {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
        }
	}
	else if (currScene == fps) {
        fpsScene->resizeGLScene(width, height);			// Set Up Our Perspective GL Screen
        if (!fpsScene->initGL())							// Initialize Our Newly Created GL Window
        {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
        }
	}
	else if (currScene == chase) {
        finalScene->resizeGLScene(width, height);			// Set Up Our Perspective GL Screen
        if (!finalScene->initGL())							// Initialize Our Newly Created GL Window
        {
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
        }
	}

	return TRUE;									// Success
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE WINDOW PROCEDURE
/////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
    if (currScene == landing) {
        landScene->windMsg(hWnd,uMsg,wParam,lParam);
    }
    else if (currScene == mainmenu) {
        menuScene->windMsg(hWnd,uMsg,wParam,lParam);
    }
    else if (currScene == help) {
        helpScene->windMsg(hWnd,uMsg,wParam,lParam);
    }
    else if (currScene == game) {
        marioScene->windMsg(hWnd,uMsg,wParam,lParam);
    }
    else if (currScene == fps) {
        fpsScene->windMsg(hWnd,uMsg,wParam,lParam);
    }
    else if (currScene == chase) {
        finalScene->windMsg(hWnd,uMsg,wParam,lParam);
    }
    //Scene->windMsg(hWnd,uMsg,wParam,lParam);
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE

			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
                                                    // LoWord=Width, HiWord=Height
			//Scene->ReSizeGLScene(GetSystemMetrics(SM_CXSCREEN),HIWORD(lParam));

			//Scene->resizeGLScene(LOWORD(lParam),HIWORD(lParam));
			if (currScene == landing) {
                landScene->resizeGLScene(LOWORD(lParam),HIWORD(lParam));			// Set Up Our Perspective GL Screen
            }
            else if (currScene == mainmenu) {
                menuScene->resizeGLScene(LOWORD(lParam),HIWORD(lParam));	// Set Up Our Perspective GL Screen
            }
            else if (currScene == help) {
                helpScene->resizeGLScene(LOWORD(lParam),HIWORD(lParam));			// Set Up Our Perspective GL Screen
            }
            else if (currScene == game) {
                marioScene->resizeGLScene(LOWORD(lParam),HIWORD(lParam));			// Set Up Our Perspective GL Screen
            }
            else if (currScene == fps) {
                fpsScene->resizeGLScene(LOWORD(lParam),HIWORD(lParam));			// Set Up Our Perspective GL Screen
            }
            else if (currScene == chase) {
                finalScene->resizeGLScene(LOWORD(lParam),HIWORD(lParam));			// Set Up Our Perspective GL Screen
            }
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE WINMAIN
/////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop
	//currScene = landing;

	int	fullscreenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int	fullscreenHeight = GetSystemMetrics(SM_CYSCREEN);
    int screenHeight = 900;
    int screenWidth = 1600;
    mainSound->initSounds();

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}
	// Create Our OpenGL Window
	if (fullscreen) {
        if (!CreateGLWindow("Game Engine Lesson 01",fullscreenWidth,fullscreenHeight,256,fullscreen))
        {
            return 0;									// Quit If Window Was Not Created
        }
	}
	else {
        if (!CreateGLWindow("Game Engine Lesson 01",screenWidth,screenHeight,256,fullscreen))
        {
            return 0;									// Quit If Window Was Not Created
        }
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (keys[VK_ESCAPE] && (currScene == game || currScene == fps || currScene == chase))	// Active?  Was There A Quit Received?
			{
                //MessageBox escape = MessageBox(NULL, "Would you like to go to the Main Menu?", "Back to Main Menu",
			    if (MessageBox(NULL,"Would you like to Quit and go back to the Main Menu?", "Quit to Main Menu",MB_YESNO) == IDYES) {
                    keys[VK_ESCAPE]=FALSE;
                    prevScene = currScene;
                    marioScene->resetScene();
                    fpsScene->resetScene();
                    SwapBuffers(hDC);
                    menuScene->initGL(); //OTHERWISE JUST RESUMES GAME. WUT
                    menuScene->drawGLScene();
                    currScene = mainmenu;
                    SwapBuffers(hDC);
			    }
			    else {
                    keys[VK_ESCAPE]=FALSE;
			    }
			}
			else									// Not Time To Quit, Update Screen
			{
				if (currScene == landing) {
                    landScene->drawGLScene();
                    SwapBuffers(hDC);
                }
                else if (currScene == mainmenu) {
                    menuScene->drawGLScene();
                    SwapBuffers(hDC);
                }
                else if (currScene == help) {
                    helpScene->drawGLScene();
                    SwapBuffers(hDC);
                }
                /*else if (currScene == game) {
                    marioScene->drawGLScene();
                    if (marioScene->sceneDone) {
                        marioScene->resetScene();
                        prevScene = currScene;
                        currScene = chase;
                        finalScene->initGL();
                        //finalScene->drawGLScene();
                    }
                    SwapBuffers(hDC);
                }
                else if (currScene == chase) {
                    finalScene->drawGLScene();
                    if(finalScene->sceneDone) {
                        finalScene->resetScene();
                        prevScene = currScene;
                        currScene = fps;
                        fpsScene->initGL();
                        //fpsScene->drawGLScene();
                    }
                    SwapBuffers(hDC);
                }*/
                else if (currScene == game) {
                    marioScene->drawGLScene();
                    if (marioScene->sceneDone) {
                        //marioScene->resetScene();
                        prevScene = currScene;
                        currScene = fps;
                        fpsScene->initGL();
                        fpsScene->drawGLScene();
                    }
                    SwapBuffers(hDC);
                }
                else if (currScene == fps) {
                    fpsScene->drawGLScene();
                    if (fpsScene->sceneDone) {
                        //fpsScene->resetScene();
                        prevScene = currScene;
                        currScene = chase;
                        finalScene->initGL();
                        //finalScene->drawGLScene();
                    }
                    SwapBuffers(hDC);
                }
                else if (currScene == chase) {
                    finalScene->drawGLScene();
                    SwapBuffers(hDC);
                }
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (fullscreen) {
                    if (!CreateGLWindow("Game Engine Lesson 01",fullscreenWidth,fullscreenHeight,256,fullscreen))
                    {
                        return 0;									// Quit If Window Was Not Created
                    }
                }
                else {
                    if (!CreateGLWindow("Game Engine Lesson 01",screenWidth,screenHeight,256,fullscreen))
                    {
                        return 0;									// Quit If Window Was Not Created
                    }
                }
			}
			if (keys[VK_RETURN]) {
                keys[VK_RETURN] = FALSE;
                if (currScene == landing) {
                    prevScene == currScene;
                    cout << "Current in " << currScene << ", the Enter Key was pressed, going to main menu"<< endl;
                    menuScene->initGL();
                    menuScene->drawGLScene();
                    SwapBuffers(hDC);
                    currScene = mainmenu;
                    cout << currScene << endl;
                }
            }
            if (keys['N']) {                       //Draws game scene on 'H' if on main menu
                keys['N']=FALSE;
                cout << "Current in " << currScene << ", the N Key was pressed, starting a new game"<< endl;
                if (currScene == mainmenu) {
                    prevScene = currScene;
                    i = 0;
                    marioScene->initGL();
                    marioScene->drawGLScene();
                    SwapBuffers(hDC);
                    currScene = game;
                    cout << currScene << endl;
                }
            }
            if (keys['H']) {                       //Draws help scene on 'H' if on main menu
                keys['H'] = FALSE;
                cout << "Current in " << currScene << ", the H Key was pressed, changing to help scene"<< endl;
                    prevScene = currScene;
                    helpScene->initGL();
                    helpScene->drawGLScene();
                    SwapBuffers(hDC);
                    currScene = help;
                    cout << currScene << endl;
                    cout << prevScene << endl;
                }
            }
            if (keys['E']) {                        //Exits game on 'E' if on main menu
                cout << "Current in " << currScene << ", the letter E was pressed, exiting... " << endl;
                keys['E'] = FALSE;
                if (currScene == mainmenu) {
                    done = TRUE;
                }
            }
            if (keys[VK_ESCAPE] && currScene == help) {
                if (prevScene == mainmenu) {
                    cout << "Previous Scene from help was " << prevScene << endl;
                    cout << "Current in " << currScene << ", the Escape Key was pressed, exiting back to Main Menu" << endl;
                    prevScene = currScene;
                    keys[VK_ESCAPE] = FALSE;
                    menuScene->initGL();
                    menuScene->drawGLScene();
                    SwapBuffers(hDC);
                    currScene = mainmenu;
                    cout << currScene << endl;
                }
                else if (prevScene == game) {
                    cout << "Current in " << currScene << ", the Escape Key was pressed, exiting back to Main Menu" << endl;
                    keys[VK_ESCAPE] = FALSE;
                    SwapBuffers(hDC);
                    mainSound->pauseSounds(true);
                    marioScene->initGL();
                    marioScene->drawGLScene();
                    currScene = prevScene;
                    prevScene = help;
                    cout << currScene << endl;
                }
                else if (prevScene == fps) {
                    cout << "Current in " << currScene << ", the Escape Key was pressed, exiting back to Main Menu" << endl;
                    keys[VK_ESCAPE] = FALSE;
                    SwapBuffers(hDC);
                    fpsScene->resumeGL();
                    fpsScene->drawGLScene();
                    currScene = prevScene;
                    prevScene = help;
                    cout << currScene << endl;
                }
            }
		}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
