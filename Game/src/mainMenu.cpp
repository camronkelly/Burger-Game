#include "mainMenu.h"
#include <GLLight.h>
#include <Inputs.h>
#include <parallax.h>
#include <timer.h>
Inputs *menuInput = new Inputs();
parallax *menuplx = new parallax();

mainMenu::mainMenu()
{
    //ctor
    screenHeight= GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
}

mainMenu::~mainMenu()
{
    //dtor
}

GLint mainMenu::initGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

   // glEnable(GL_COLOR_MATERIAL);
    GLLight SetLight(GL_LIGHT0);
    GLLight Light(GL_LIGHT0);

    menuplx->parallaxInit("images/mainmenu.png");

    return true;
}

GLint mainMenu::drawGLScene()
{
   // srand(time(NULL));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

    glPushMatrix();
      glScaled(3.33,3.33,1.0);
         menuplx->drawSquare(screenWidth,screenHeight);
     glPopMatrix();


}

GLvoid mainMenu::resizeGLScene(GLsizei width, GLsizei height)
{
   GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
   glViewport(0,0,width,height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0,aspectRatio,0.1,1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int mainMenu::windMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
	{
	    case WM_KEYDOWN:
	        menuInput->wParam = wParam;
	        menuInput->keyEnv(menuplx, 0.0005);
            break;

	    case WM_KEYUP:								// Has A Key Been Released?
		{
			menuInput->wParam = wParam;
			menuInput->keyUP();
            break;								// Jump Back
		}

		case WM_LBUTTONDOWN:
        {
            menuInput->wParam = wParam;
            break;								// Jump Back
        }

   		case WM_RBUTTONDOWN:
        {
            menuInput->wParam = wParam;
            break;								// Jump Back
        }

          case WM_MBUTTONDOWN:
        {
            menuInput->wParam = wParam;
            break;								// Jump Back
        }

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            menuInput->mouseEventUp();
            break;								// Jump Back
        }

}
}
