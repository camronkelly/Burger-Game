#ifndef CHASESCENE_H
#define CHASESCENE_H

#include <windows.h>

#include<gl/gl.h>
#include<iostream>
#include <timer.h>

using namespace std;

class ChaseScene
{
    public:
        ChaseScene(int* scorecounter);
        virtual ~ChaseScene();
        GLint initGL();
        GLint drawGLScene();
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

        //Other Params
        WPARAM wParam;
        float screenHeight;
        float screenWidth;
        bool  sceneDone;
        bool victory = false;
    protected:

    private:
        int* score;
        bool shotMoving = false;
        bool movingLeft;
        bool movingRight;
        bool bikerLeft;
        bool bikerRight;
        int enemyrun;
        timer *timboi = new timer();
};

#endif // CHASESCENE_H
