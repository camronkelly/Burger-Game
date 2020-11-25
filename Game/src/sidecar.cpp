    #include "sidecar.h"

#include <timer.h>
#include <textureLoader.h>
#include <iostream>
#include <cmath>

using namespace std;

timer *tim2 = new timer();
//timer *spawnRate = new timer();
textureLoader *sidecarText = new textureLoader();

sidecar::sidecar()
{
    //ctor
    verticies[0].x = 0.0;verticies[0].y = 0.0;verticies[0].z = -1;
    verticies[1].x = .3;verticies[1].y = 0.0;verticies[1].z = -1;
    verticies[2].x = .3;verticies[2].y = .3;verticies[2].z = -1;
    verticies[3].x = 0.0;verticies[3].y = .3;verticies[3].z = -1;
    Xpos = 0.9;
    Ypos = -0.4;

    runSpeed = .02;

}

sidecar::~sidecar()
{
    //dtor
}

void sidecar::drawsidecar()
{
    glColor3f(1.0,1.0,1.0);
    sidecarText->binder();
    glBegin(GL_QUADS);

    glTexCoord2f(0.0,1.0);
    glVertex3f(verticies[0].x,verticies[0].y,verticies[0].z);

    glTexCoord2f(1.0,1.0);
    glVertex3f(verticies[1].x,verticies[1].y,verticies[1].z);

    glTexCoord2f(1.0,0.0);
    glVertex3f(verticies[2].x,verticies[2].y,verticies[2].z);

    glTexCoord2f(0.0,0.0);
    glVertex3f(verticies[3].x,verticies[3].y,verticies[3].z);

    glEnd();
}

void sidecar::sidecarInit()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    tim2->start();
    sidecarText->binder();
    sidecarText->bindTexture("images/sidecar.png");

}

void sidecar::sidecarScroll(player* ply)
{
    if(tim2->getTicks()>10){
        //cout << "xpos "<< Xpos << endl;
        //cout << "ypos" << Ypos << endl;
        Xpos= Xpos - runSpeed;
        tim2->reset();
    }
    if((Xpos > -1.1 && Xpos < 1)){

        glTranslated(Xpos,Ypos,0);
        //cout << Ypos << endl;
        drawsidecar();
    }
    else{
        //cout << Ypos;
        Xpos = .8;
        }
    //if(isCollided(ply))
        //cout << isCollided(ply) << endl;
}

bool sidecar::isCollided(player* ply)
{
    float xdistance = ply->Xpos-Xpos;
    float ydistance = abs(ply->Ypos-Ypos);

    //cout << ply->Xpos << endl;;
    //cout << xdistance << endl;
    //cout << ydistance << endl;
    if (xdistance < .27 && ydistance < .2 && xdistance > -.02 )
    {
        //cout << "COLLISION" << endl;
            //*score += 100;
            return true;
    }
    /*
        float distance = sqrt(pow(abs(ply->Xpos)-abs(Xpos),2)+pow(abs(ply->Ypos)-abs(Ypos),2));

        if (distance < 1.4142135375){
            cout << "COLLISION" << endl;
            return false;
            }
            */
        return false;
}

