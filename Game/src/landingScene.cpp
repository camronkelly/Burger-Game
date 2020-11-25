#include "landingScene.h"
#include <GLLight.h>
#include <Inputs.h>
#include <parallax.h>
#include <timer.h>
Inputs *landingInput = new Inputs();
parallax *landingplx = new parallax();

landingScene::landingScene()
{
    //ctor
    screenHeight= GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
}

landingScene::~landingScene()
{
    //dtor
}

GLint landingScene::initGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

   // glEnable(GL_COLOR_MATERIAL);
    GLLight SetLight(GL_LIGHT0);
    GLLight Light(GL_LIGHT0);
    landingplx->parallaxInit("images/landing.png");

    return true;
}

GLint landingScene::drawGLScene()
{
   // srand(time(NULL));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

    glPushMatrix();
      glScaled(3.33,3.33,1.0);
         landingplx->drawSquare(screenWidth,screenHeight);
     glPopMatrix();
}

GLvoid landingScene::resizeGLScene(GLsizei width, GLsizei height)
{
   GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
   glViewport(0,0,width,height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0,aspectRatio,0.1,1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int landingScene::windMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
	{
	    case WM_KEYDOWN:
	        landingInput->wParam = wParam;
	        landingInput->keyEnv(landingplx, 0.0005);
            break;

	    case WM_KEYUP:								// Has A Key Been Released?
		{
			landingInput->wParam = wParam;
			landingInput->keyUP();
            break;								// Jump Back
		}

		case WM_LBUTTONDOWN:
        {
            landingInput->wParam = wParam;
            break;								// Jump Back
        }

   		case WM_RBUTTONDOWN:
        {
            landingInput->wParam = wParam;
            break;								// Jump Back
        }

          case WM_MBUTTONDOWN:
        {
            landingInput->wParam = wParam;
            break;								// Jump Back
        }

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            landingInput->mouseEventUp();
            break;								// Jump Back
        }

}
}


