#ifndef RESTAURANTSCENE_H
#define RESTAURANTSCENE_H

#include <windows.h>

#include<gl/gl.h>
#include<iostream>

using namespace std;

class restaurantScene
{
    public:
        restaurantScene(int *);
        virtual ~restaurantScene();
        GLint initGL();
        GLint drawGLScene();
        GLint resumeGL();
        GLvoid resizeGLScene(GLsizei, GLsizei);
        GLvoid resetScene();

        int windMsg(HWND,UINT, WPARAM,LPARAM);

        //Font
        GLvoid buildFont(int);
        GLvoid killFont();
        GLvoid glPrint(const char *, ...);
        GLuint base;
        GLfloat cnt1;
        GLfloat cnt2;

        WPARAM wParam;
        float screenHeight;
        float screenWidth;
        int spatulaCounter;
        bool sceneDone;
    protected:

    private:
        bool gunMoveLeft = false;
        bool gunMoveRight = false;
        bool shotMoving = false;
        int *score;
};

#endif // RESTAURANTSCENE_H
