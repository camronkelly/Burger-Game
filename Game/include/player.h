#ifndef PLAYER_H
#define PLAYER_H

#include<Model.h>
#include<gl/gl.h>
#include<textureLoader.h>

typedef struct
{
   float x;
   float y;
   float z;
}vec;

class player:public Model
{
    public:
        player();
        virtual ~player();

        float scalesize[3]={1.0,1.0,1.0};
        vec verticies[4];

        int runspeed;
        float jumpspeed;

        int actionTrigger;
        int runTrigger;
        int jumpTrigger;

        bool inAir;
        float startY;

        float jumpLoc= 0;

        void drawPlayer();
        void playerInit();
        void actions(int&);

        bool goingup;
        bool goingdown;

        void set_scale(float x, float y);

    protected:

    private:
};

#endif // PLAYER_H
