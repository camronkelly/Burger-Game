#include "GLScene.h"
#include <GLLight.h>
#include <Model.h>
#include <Inputs.h>
#include <parallax.h>
#include <player.h>
#include <skyBox.h>
#include <enemy.h>
#include <timer.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <variables.h>
#include <sounds.h>
#include <sidecar.h>

Model *modelTeapot = new Model();
Inputs *KbMs = new Inputs();
parallax *plx = new parallax();
player *ply = new player();
skyBox *sky = new skyBox;
enemy *ene;
timer *scenetim = new timer();
//timer *spawns = new timer();
sidecar *car = new sidecar();
sounds *snds = new sounds();

GLScene::GLScene(int* score)
{
    //ctor
    screenHeight= GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    sceneDone = false;
    this->score = score;
    ene = new enemy(score);
}

GLScene::~GLScene()
{
    //dtor
}
GLvoid GLScene::buildFont(int fontsize) {
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

GLvoid GLScene::killFont() {
    glDeleteLists(base, 96);
}

GLvoid GLScene::glPrint(const char *fmt, ...) {
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


GLvoid GLScene::resetScene() {

        delete modelTeapot; modelTeapot = new Model();
        delete KbMs; KbMs = new Inputs();
        delete plx; plx = new parallax();
        delete ply; ply = new player();
        delete sky; sky = new skyBox();
        delete ene; ene = new enemy(score);
        delete scenetim; scenetim = new timer();
        delete car; car = new sidecar();
        sceneDone = false;
}

GLint GLScene::initGL()
{
    scenetim->start();
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

   // glEnable(GL_COLOR_MATERIAL);
    GLLight SetLight(GL_LIGHT0);
    GLLight Light(GL_LIGHT0);

    modelTeapot->modelInit("images/player/player0.png",true);
    plx->parallaxInit("images/city_life.png");

    ply->playerInit();
    sky->loadTextures();
    ene->enemyInit("images/pixelfries.png");
    car->sidecarInit();
    snds->initSounds();
    buildFont(30);
    snds->playMusic("sounds/music.mp3", true);
    snds->isPlaying = true;
    //spawns->start();
    return true;
}

GLint GLScene::drawGLScene()
{
   // srand(time(NULL));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	  glLoadIdentity();									// Reset The Current Modelview Matrix
    glPushMatrix();
        glScaled(3.33,3.33,1.0);
        plx->drawSquare(screenWidth,screenHeight);
    glPopMatrix();


    bool running;
    if(ply->runTrigger > 0)
        running = true;
    else
        running = false;
    float speed = (screenWidth/screenHeight)/(screenWidth/3);

    if (ene->enemyCounter <= 0) {
    //if (*score >= 1) {
        ply->Xpos += speed*4;
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
        if (ply->Xpos >= .9) {
            sceneDone = true;
        }
    }
    else if (car->isCollided(ply)) {
        ply->runTrigger = 0;
        plx->scroll(running, "right", 0.0);
        ply->actionTrigger = 0;
        snds->pauseSounds(true);
        snds->playSound("sounds/death.mp3",false);
        glPushMatrix();
            buildFont(150);
            glColor3f(255.0f, 0.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRasterPos2f(-2.5f, 0.0f);                 // Position The Text On The Screen
            glPrint("YOU DIED A FAILURE", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
        glPushMatrix();
            buildFont(30);
            glColor3f(255.0f, 0.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRasterPos2f(-2.0f, -0.5f);                 // Position The Text On The Screen
            glPrint("Press Esc then 'Y' to return to main menu", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
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
        plx->scroll(running,"right",speed);
    }

    glPushMatrix();
        ply->actions(ply->actionTrigger);
    glPopMatrix();

    if(ply->runTrigger == 1){
        glPushMatrix();
            ene->enemyScroll(ply);

        glPopMatrix();
    }


    if(ply->runTrigger == 1 && ene->enemyCounter > 0){
        glPushMatrix();
            car->sidecarScroll(ply);
        glPopMatrix();
    }

}

GLvoid GLScene::resizeGLScene(GLsizei width, GLsizei height)
{
   GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
   glViewport(0,0,width,height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0,aspectRatio,0.1,1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int GLScene::windMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
	{

	    case WM_KEYDOWN:
	        KbMs->wParam = wParam;
	        KbMs->keyPressed(modelTeapot);
	        KbMs->keyEnv(plx, 0.0005);
	        KbMs->keyPressed(ply);
	        KbMs->keyPressed(sky);
	        KbMs->keySound(snds);

	    break;

	    case WM_KEYUP:								// Has A Key Been Released?
		{
			KbMs->wParam = wParam;
			KbMs->keyUP();
			KbMs->keyUp(ply);
		break;								// Jump Back
		}

		case WM_LBUTTONDOWN:
        {
            KbMs->wParam = wParam;
            KbMs->mouseEventDown(modelTeapot,LOWORD(lParam),HIWORD(lParam));
        break;								// Jump Back
        }

   		case WM_RBUTTONDOWN:
        {
            KbMs->wParam = wParam;
            KbMs->mouseEventDown(modelTeapot,LOWORD(lParam),HIWORD(lParam));
        break;								// Jump Back
        }

          case WM_MBUTTONDOWN:
        {
            KbMs->wParam = wParam;
            KbMs->mouseEventDown(modelTeapot,LOWORD(lParam),HIWORD(lParam));
        break;								// Jump Back
        }

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            KbMs->mouseEventUp();
        break;								// Jump Back
        }

        case WM_MOUSEMOVE:
        {
             KbMs->mouseMove(modelTeapot, LOWORD(lParam),HIWORD(lParam));
             KbMs->mouseMove(sky,LOWORD(lParam),HIWORD(lParam));
        break;								// Jump Back
        }

        case WM_MOUSEWHEEL:
        {
            KbMs->mouseWheel(modelTeapot,(double)GET_WHEEL_DELTA_WPARAM(wParam));
        break;								// Jump Back
        }
}
}
