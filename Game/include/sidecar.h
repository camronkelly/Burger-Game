#ifndef SIDECAR_H
#define SIDECAR_H

#include<Model.h>
#include<gl/gl.h>
#include<textureLoader.h>
#include <string.h>
#include <iostream>
#include <player.h>

using namespace std;

typedef struct
{
   float x;
   float y;
   float z;
}vec3;

class sidecar:public Model{
    public:
        sidecar();
        virtual ~sidecar();

        float scalesize[3]={1.0,1.0,1.0};
        vec3 verticies[4];

        float runSpeed;
        float xpos;
        float ypos;
        void drawsidecar();
        //void deleteEnemy();
        void sidecarInit();
        void sidecarScroll(player* ply);
        bool isCollided(player*);


    protected:

    private:
};

#endif // SIDECAR_H
