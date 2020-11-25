#ifndef ENEMY_H
#define ENEMY_H

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
}vec2;

class enemy:public Model{
    public:
        enemy(int*);
        virtual ~enemy();

        float scalesize[3]={1.0,1.0,1.0};
        vec2 verticies[4];

        float runSpeed;
        float xpos;
        float ypos;
        void drawEnemy();
        void deleteEnemy();
        void enemyInit(char* file);
        void enemyScroll(player* ply);
        bool isCollided(player*);
        bool isCollided(Model*);
        void set_scale(float x, float y);
        int  enemyCounter;
        int health;
    protected:

    private:
        int* score;
};

#endif // ENEMY_H
