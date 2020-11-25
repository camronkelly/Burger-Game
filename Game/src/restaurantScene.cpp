#include "restaurantScene.h"
//#include <GLLight.h>
#include <Model.h>
#include <Inputs.h>
#include <parallax.h>
#include <player.h>
#include <skyBox.h>
#include <cmath>
#include <stdio.h>
#include <math.h>
#include <timer.h>
#include <stdarg.h>
#include <variables.h>
#include <sounds.h>
//#include <random>

sounds *restaurantsnds = new sounds();
Model *modelPot = new Model();
Inputs *restaurantKbMs = new Inputs();
parallax *restaurantPlx = new parallax();
player *resturantPly = new player();
skyBox *restaurantSky = new skyBox();
Model *man[3] = { new Model(), new Model(), new Model()};
Model *cursor = new Model();
Model *rock = new Model();
Model *gun = new Model();
parallax *transitionplx = new parallax();
timer *tim3 = new timer();


restaurantScene::restaurantScene(int *scorecounter)
{
    //ctor
    screenHeight= GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    score = scorecounter;
    spatulaCounter = rand()%(11)+10;
}

restaurantScene::~restaurantScene()
{
    //dtor
}

GLvoid restaurantScene::resetScene() {
    delete modelPot; modelPot = new Model();
    delete restaurantKbMs; restaurantKbMs = new Inputs();
    delete restaurantPlx; restaurantPlx = new parallax();
    delete transitionplx; transitionplx = new parallax();
    delete resturantPly; resturantPly = new player();
    delete restaurantSky; restaurantSky = new skyBox();
    delete cursor; cursor = new Model();
    delete rock; rock = new Model();
    delete gun; gun = new Model();
    delete tim3; tim3 = new timer();
    for (int i = 0; i < 3; i++) {
        delete man[i]; man[i] = new Model();
    }
    sceneDone = false;

}

GLvoid restaurantScene::buildFont(int fontsize) {
    HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	fontsize,							    // Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Comic Sans MS");					    // Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
    DeleteObject(font);
}

GLvoid restaurantScene::killFont() {
    glDeleteLists(base, 96);
}

GLvoid restaurantScene::glPrint(const char *fmt, ...) {
    char text[256];
    va_list ap;
    if (fmt == NULL)
            return;

    va_start(ap, fmt);
        vsprintf(text, fmt, ap);
    va_end(ap);

    glPushAttrib(GL_LIST_BIT);
    glListBase(base-32);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();
}

GLint restaurantScene::resumeGL() {
    srand(NULL);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

   // glEnable(GL_COLOR_MATERIAL);
    //GLLight SetLight(GL_LIGHT0);
    //GLLight Light(GL_LIGHT0);

    //modelTeapot->modelInit("images/player/player0.png",true);
    restaurantPlx->parallaxInit("images/secondstage.png");
    resturantPly->playerInit();
    restaurantSky->loadTextures();
}

GLint restaurantScene::initGL()
{
    srand(NULL);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

   // glEnable(GL_COLOR_MATERIAL);
    //GLLight SetLight(GL_LIGHT0);
    //GLLight Light(GL_LIGHT0);

    //modelTeapot->modelInit("images/player/player0.png",true);
    restaurantPlx->parallaxInit("images/secondstage.png");
    resturantPly->playerInit();
    restaurantSky->loadTextures();
    restaurantsnds->initSounds();
    buildFont(30);

    for(int i = 0; i < 3; i++) {
        man[i]->modelInit("images/spatula.png", true);
        man[i]->Zoom = -2.;
        man[i]->Ypos = .6;
        man[i]->set_scale(.25,.25);

    }
    man[0]->Xpos = -0.7;
    man[1]->Xpos = -0.1;
    man[2]->Xpos = 0.5;

    cursor->modelInit("images/cursor.png", true);

    rock->modelInit("images/fry.png", true);
    rock->Zoom = -2.0;
    rock->Xpos = 100.0;
    rock->Ypos = 100.0;
    rock->set_scale(.25,.25);

    gun->modelInit("images/GunOnly.png", true);
    gun->Ypos = -0.4;
    gun->Xpos = 0.0;
    gun->Zoom = -2.0;
    gun->set_scale(.25,.25);

    return true;
}


GLint restaurantScene::drawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
    glPushMatrix();
        glScaled(3.33,3.33,1.0);
        restaurantPlx->drawSquare(screenWidth,screenHeight);
    glPopMatrix();

       //restaurantPlx->scroll(true,"right",0.005);
    if(*score <= -1) {
        restaurantsnds->pauseSounds(true);
        restaurantsnds->playSound("sounds/death.mp3",false);
        glPushMatrix();
            buildFont(100);
            glColor3f(255.0f, 0.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -2.0f);
            glRasterPos2f(-.5f, 0.1f);                         // Position The Text On The Screen
            glPrint("OUT OF AMMO", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
        glPushMatrix();
            buildFont(100);
            glColor3f(255.0f, 0.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -2.0f);
            glRasterPos2f(-.5f, -0.15f);                         // Position The Text On The Screen
            glPrint("YOU ARE A FAILURE", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
        glPushMatrix();
            buildFont(50);
            glColor3f(255.0f, 0.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRasterPos2f(-.5f, -0.8f);                 // Position The Text On The Screen
            glPrint("Press Esc then 'Y' to return to main menu", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
    }
    else if(man[0]->Ypos <= -.4 ||man[1]->Ypos <= -.4 || man[2]->Ypos <= -.4 )
    {
        restaurantsnds->pauseSounds(true);
        restaurantsnds->playSound("sounds/death.mp3",false);
        glPushMatrix();
            buildFont(100);
            glColor3f(255.0f, 0.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -2.0f);
            glRasterPos2f(-.5f, -0.15f);                         // Position The Text On The Screen
            glPrint("YOU ARE A FAILURE", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
        glPushMatrix();
            buildFont(50);
            glColor3f(255.0f, 0.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRasterPos2f(-.5f, -0.8f);                 // Position The Text On The Screen
            glPrint("Press Esc to then 'Y' return to main menu", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
    }
    else if (spatulaCounter <= 0) {
        transitionplx->parallaxInit("images/transition.png");
        glPushMatrix();
            glScaled(3.33,3.33,1.0);
            transitionplx->drawSquare(screenWidth,screenHeight);
        glPopMatrix();
        tim3->start();
        if(tim3->getTicks() > 1000) {
            sceneDone = true;
        }
    }
    else {
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, -5.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glRasterPos2f(2.2f, 1.8f);                 // Position The Text On The Screen
            char buffer [100];
            char *intToStr = itoa(*score, buffer, 10);
            string counter = string(intToStr);
            string currentScore = "Ammunition: " + counter;
            glPrint(currentScore.c_str(), cnt1);	// Print GL Text To The Screen
        glPopMatrix();
        glEnable(GL_LIGHTING);


        for (int i = 0; i < 3; i++) {
            man[i]->Ypos -= rand() % 1000 * 0.000005;
            glPushMatrix();
                if(man[i]->isCollided(rock)){
                    man[i]->Ypos = .6;
                    shotMoving = false;
                    spatulaCounter--;
                    cout << spatulaCounter << endl;
                    rock->Ypos = 1000.0;
                }
                man[i]->drawModel();
            glPopMatrix();
        }
        glPushMatrix();
            //glScalef(0.2, 0.2, 1);
            //glTranslated(0, 0, rock->Zoom);
            if (shotMoving) {
                rock->Ypos += 0.01;
            }
            if (rock->Ypos > .4) {
                shotMoving = false;
                rock->Ypos = 1000.0;
            }
            rock->drawModel();
        glPopMatrix();
        //cout << rock->Ypos - man[1]->Ypos << endl;
        //man[0]->isCollided(rock);

        glPushMatrix();
            glScaled(1.0, 1.0, 1.0);
            if (gunMoveLeft && (gun->Xpos > -.75)) {
                gun->Xpos -= 0.01;
            } else if (gunMoveRight && (gun->Xpos < .5)) {
                gun->Xpos += 0.01;
            }
            //glScalef(0.2, 0.2, 1);
            //glTranslated(0, gun->Ypos, gun->Zoom);
            gun->drawModel();
        glPopMatrix();
    }



/*
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glScaled(200,200,200);
        restaurantSky->drawBox();
        glEnable(GL_LIGHTING);
    glPopMatrix();
*/
/*
    glPushMatrix();
        //glTranslated(0,0,modelTeapot->Zoom);
        resturantPly->actions(resturantPly->actionTrigger);
    glPopMatrix();
*/

/*
    glPushMatrix();
        glScaled(1, 1, 1);
        glTranslated(0, 0, cursor->Zoom);
        //cursor->drawModel();
    glPopMatrix();
*/
}

GLvoid restaurantScene::resizeGLScene(GLsizei width, GLsizei height)
{
   GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
   glViewport(0,0,width,height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0,aspectRatio,0.1,1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int restaurantScene::windMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
	{

	    case WM_KEYDOWN:
	        restaurantKbMs->wParam = wParam;
	        if (wParam == VK_LEFT) {
                gunMoveLeft = true;
	        } else if (wParam == VK_RIGHT) {
	            gunMoveRight = true;
	        } else if (wParam == VK_SPACE && !shotMoving) {
                rock->Xpos = gun->Xpos;
                rock->Ypos = gun->Ypos;
                shotMoving = true;
                *score = *score - 1;
            }
            restaurantKbMs->keySound(restaurantsnds);
	    break;

	    case WM_KEYUP:								// Has A Key Been Released?
		{
	        if (wParam == VK_LEFT) {
                gunMoveLeft = false;
	        } else if (wParam == VK_RIGHT) {
	            gunMoveRight = false;
	        }
		      break;								// Jump Back
		}

		case WM_LBUTTONDOWN:
        {
        break;								// Jump Back
        }

   		case WM_RBUTTONDOWN:
        {
          restaurantKbMs->wParam = wParam;
          restaurantKbMs->mouseEventDown(modelPot,LOWORD(lParam),HIWORD(lParam));
        break;								// Jump Back
        }

          case WM_MBUTTONDOWN:
        {
          restaurantKbMs->wParam = wParam;
          restaurantKbMs->mouseEventDown(modelPot,LOWORD(lParam),HIWORD(lParam));
        break;								// Jump Back
        }

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            //restaurantKbMs->mouseEventUp();
        break;								// Jump Back
        }

        case WM_MOUSEMOVE:
        {
        break;								// Jump Back
        }

        case WM_MOUSEWHEEL:
        {
        break;								// Jump Back
        }
}
}
