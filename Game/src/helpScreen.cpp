#include "helpScreen.h"
#include <GLLight.h>
#include <Inputs.h>
#include <parallax.h>
#include <timer.h>
Inputs *helpInput = new Inputs();
parallax *helpplx = new parallax();

helpScreen::helpScreen()
{
    //ctor
    screenHeight= GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
}

helpScreen::~helpScreen()
{
    //dtor
}

GLint helpScreen::initGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

   // glEnable(GL_COLOR_MATERIAL);
    GLLight SetLight(GL_LIGHT0);
    GLLight Light(GL_LIGHT0);

    helpplx->parallaxInit("images/help.png");

    return true;
}

GLint helpScreen::drawGLScene()
{
   // srand(time(NULL));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

    glPushMatrix();
      glScaled(3.33,3.33,1.0);
         helpplx->drawSquare(screenWidth,screenHeight);
     glPopMatrix();
}

GLvoid helpScreen::resizeGLScene(GLsizei width, GLsizei height)
{
   GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
   glViewport(0,0,width,height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0,aspectRatio,0.1,1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int helpScreen::windMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
	{
	    case WM_KEYDOWN:
	        helpInput->wParam = wParam;
	        helpInput->keyEnv(helpplx, 0.0005);
            break;

	    case WM_KEYUP:								// Has A Key Been Released?
		{
			helpInput->wParam = wParam;
			helpInput->keyUP();
            break;								// Jump Back
		}

		case WM_LBUTTONDOWN:
        {
            helpInput->wParam = wParam;
            break;								// Jump Back
        }

   		case WM_RBUTTONDOWN:
        {
            helpInput->wParam = wParam;
            break;								// Jump Back
        }

          case WM_MBUTTONDOWN:
        {
            helpInput->wParam = wParam;
            break;								// Jump Back
        }

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            helpInput->mouseEventUp();
            break;								// Jump Back
        }
    }
}
