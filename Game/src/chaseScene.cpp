#include "ChaseScene.h"
//#include <GLLight.h>
#include <parallax.h>
#include <player.h>
#include <enemy.h>
#include <sounds.h>
#include <Model.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <stdarg.h>
#include <variables.h>
#include <Inputs.h>

Inputs *finalKbMs = new Inputs();
sounds *finalsound = new sounds();
parallax *finalBackground = new parallax();
player *burgerman = new player();
enemy *bikeboi;
//textureLoader biketextures[2];
timer *bikertim = new timer();
int bikerticks;
Model* leftFry = new Model();
timer * failTimer = new timer();

ChaseScene::ChaseScene(int* scorecounter)
{
    //ctor
    score = scorecounter;
    screenHeight= GetSystemMetrics(SM_CYSCREEN);
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    bikeboi = new enemy(score);
    timboi = new timer();
}

ChaseScene::~ChaseScene()
{
    //dtor
}
GLvoid ChaseScene::buildFont(int fontsize) {
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

GLvoid ChaseScene::killFont() {
    glDeleteLists(base, 96);
}

GLvoid ChaseScene::glPrint(const char *fmt, ...) {
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


GLvoid ChaseScene::resetScene() {
        delete finalBackground; finalBackground = new parallax();
        delete burgerman; burgerman = new player();
        delete bikeboi; bikeboi = new enemy(score);
}

GLint ChaseScene::initGL()
{
    srand(time(NULL));
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    buildFont(30);
   // glEnable(GL_COLOR_MATERIAL);
    //GLLight SetLight(GL_LIGHT0);
    //GLLight Light(GL_LIGHT0);

    finalBackground->parallaxInit("images/finalbackground.png");

    //biketextures[0].bindTexture("images/sprite_0.png");
    //biketextures[1].bindTexture("images/sprite_1.png");
    timboi->start();
    bikertim->start();
    bikerticks = (rand() % 2000) + 500;
    finalsound->initSounds();
    burgerman->playerInit();
    burgerman->Xpos = 0.0;
    burgerman->Ypos = -0.35;
    burgerman->Zoom = -4.0;
    burgerman->set_scale(2.0, 2.0);

    bikeboi->enemyInit("images/sprite_0.png");
    bikeboi->Xpos = -.2;
    bikeboi->Ypos = 0.1;
    bikeboi->Zoom = -4.0;
    bikeboi->set_scale(2.0, 2.0);

    leftFry->modelInit("images/fry.png", true);

    leftFry->Xpos  = 100.0;
    leftFry->Ypos  = 0.0;
    leftFry->Zoom  = -2.0;
    leftFry->set_scale(0.2, 0.2);


    //spawns->start();
    return true;
}

GLint ChaseScene::drawGLScene()
{
   // srand(time(NULL));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

    glPushMatrix();
        glScaled(3.33,3.33,1.0);
        finalBackground->drawSquare(screenWidth,screenHeight);
    glPopMatrix();
    finalBackground->scroll(true,"up",0.01);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -5.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glDisable(GL_LIGHTING);
        glRasterPos2f(2.2f, 1.8f);                 // Position The Text On The Screen
        char buffer [100];
        char *intToStr = itoa(*score, buffer, 10);
        string counter = string(intToStr);
        string hello = "Ammunition " + counter;
        glPrint(hello.c_str(), cnt1);	// Print GL Text To The Screen
    glPopMatrix();
    glEnable(GL_LIGHTING);


    if(*score <= -1 && bikeboi->health > 0 ) {
        finalsound->pauseSounds(true);
        finalsound->playSound("sounds/death.mp3", false);
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
            glPrint("Press Esc to then 'Y' return to main menu", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
        failTimer->start();
        if(failTimer->getTicks() > 3000)
            sceneDone = true;
    }
    if(bikeboi->health <= 0 ) {
        glPushMatrix();
            buildFont(100);
            glColor3f(0.0f, 255.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -2.0f);
            glRasterPos2f(-.5f, 0.1f);                         // Position The Text On The Screen
            glPrint("YOU WON", cnt1);	// Print GL Text To The Screen
        glPopMatrix();
        glPushMatrix();
            buildFont(50);
            glColor3f(0.0f, 255.0f, 0.0f);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRasterPos2f(-.5f, -0.3f);                 // Position The Text On The Screen
            glPrint("Press Esc to then 'Y' return to main menu", cnt1);	// Print GL Text To The Screen
        glPopMatrix();

        failTimer->start();
        if(failTimer->getTicks() > 3000)
            sceneDone = true;
    }
   glPushMatrix();
        if (movingLeft && burgerman->Xpos > -0.5) {
            burgerman->Xpos -= 0.01;
        } else if (movingRight && burgerman->Xpos < 0.3) {
            burgerman->Xpos += 0.01;
        }
    int a = 3;
        burgerman->actions(a);
    glPopMatrix();

glPushMatrix();
        if(shotMoving) {
            leftFry->Ypos += 0.02;
        }
        if(leftFry->Ypos > .4) {
            shotMoving = false;
            leftFry->Ypos = 1000.0;
        }
        if(bikeboi->isCollided(leftFry))
        {
            shotMoving = false;
            leftFry->Ypos = 1000.0;
        }
        leftFry->drawModel();
    glPopMatrix();



    glPushMatrix();
        if (bikerLeft && bikeboi->Xpos > -0.5) {
            bikeboi->Xpos -= 0.005;
        } else if (bikerRight && bikeboi->Xpos < 0.3) {
            bikeboi->Xpos += 0.005;
        }
        if (bikertim->getTicks() > bikerticks) {
            if (bikerLeft) {
                bikerRight = true;
                bikerLeft = false;
            } else {
                bikerLeft = true;
                bikerRight = false;
            }
            bikerticks = (rand() % 1300) + 100;
            bikertim->reset();
        }
        glTranslated(bikeboi->Xpos, bikeboi->Ypos, 0);
        if(bikeboi->health > 0)
            bikeboi->drawEnemy();
    glPopMatrix();

}

GLvoid ChaseScene::resizeGLScene(GLsizei width, GLsizei height)
{
   GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
   glViewport(0,0,width,height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0,aspectRatio,0.1,1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int ChaseScene::windMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
	{

	    case WM_KEYDOWN:
	        finalKbMs->wParam = wParam;
	        if (wParam == VK_LEFT) {
                movingLeft = true;
                movingRight = false;
	        } else if (wParam == VK_RIGHT) {
                movingRight = true;
                movingLeft = false;
	        } else if (wParam == VK_SPACE & !shotMoving && *score >-1) {
                leftFry->Xpos = burgerman->Xpos;

                leftFry->Ypos =  burgerman->Ypos;
                if(*score >0)
                    shotMoving = true;
                *score = *score - 1;
	        }
	        finalKbMs->keySound(finalsound);
	    break;

	    case WM_KEYUP:								// Has A Key Been Released?
		{
		    if (wParam == VK_LEFT) {
		        movingLeft = false;
		    } else if (wParam == VK_RIGHT) {
		        movingRight = false;
		    }
		break;								// Jump Back
		}

		case WM_LBUTTONDOWN:
        {
        break;								// Jump Back
        }

   		case WM_RBUTTONDOWN:
        {
        break;								// Jump Back
        }

          case WM_MBUTTONDOWN:
        {
        break;								// Jump Back
        }

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
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
