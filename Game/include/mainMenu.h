#ifndef MAINMENU_H
#define MAINMENU_H
#include <windows.h>

#include<gl/gl.h>
#include<iostream>

using namespace std;

class mainMenu
{
    public:
        mainMenu();
        virtual ~mainMenu();
        GLint initGL();
        GLint drawGLScene();
        GLvoid resizeGLScene(GLsizei, GLsizei);

        int windMsg(HWND,UINT, WPARAM,LPARAM);


        WPARAM wParam;
        float screenHeight;
        float screenWidth;
    protected:

    private:
};

#endif // MAINMENU_H
