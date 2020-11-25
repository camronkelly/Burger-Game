#ifndef LANDINGSCENE_H
#define LANDINGSCENE_H
#include <windows.h>

#include<gl/gl.h>
#include<iostream>

using namespace std;


class landingScene
{
    public:
        landingScene();
        virtual ~landingScene();
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

#endif // LANDINGSCENE_H
