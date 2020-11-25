#include <player.h>
#include <timer.h>
#include <textureLoader.h>
#include <iostream>
#include <string>

using namespace std;

timer *T = new timer();
timer *landT = new timer();
timer *airT = new timer();
textureLoader runText[8];
textureLoader stand[2];
textureLoader jump[17];
textureLoader rTT2[2];

player::player()
{
    //ctor
    verticies[0].x = 0.0;verticies[0].y = 0.0;verticies[0].z = -1.0;
    verticies[1].x = .10;verticies[1].y = 0.0;verticies[1].z = -1.0;
    verticies[2].x = .10;verticies[2].y = .10;verticies[2].z = -1.0;
    verticies[3].x = 0.0;verticies[3].y = .10;verticies[3].z = -1.0;

    runspeed  =0;
    jumpspeed =0.015;
    actionTrigger =0;
    runTrigger = 0;
    jumpTrigger = 0;
    jumpLoc = 0;
    Xpos = -.3;
    Ypos = -.3;
    startY = -.3;
    inAir = false;
}

player::~player()
{
    //dtor
}
void player::drawPlayer()
{
  //  glColor3f(1.0,0.0,0.0);
   // glPushMatrix();
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

 //   glPopMatrix();
}

void player::playerInit()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    T->start();
    landT->start();
    airT->start();
    stand[0].bindTexture("images/burger/sprite_0.png");


    runText[0].bindTexture("images/burger/sprite_1.png");
    runText[1].bindTexture("images/burger/sprite_2.png");
    runText[2].bindTexture("images/burger/sprite_3.png");
    runText[3].bindTexture("images/burger/sprite_4.png");
    runText[4].bindTexture("images/burger/sprite_5.png");
    runText[5].bindTexture("images/burger/sprite_6.png");
    runText[6].bindTexture("images/burger/sprite_7.png");
    runText[7].bindTexture("images/burger/sprite_8.png");

    jump[0].bindTexture("images/burger/jump/sprite_07.png");

    rTT2[0].bindTexture("images/topBurger0.png");
    rTT2[1].bindTexture("images/topBurger1.png");

}

void player::actions(int &action)
{


   switch(action){
   case 0:
       glPushMatrix();
       glTranslated(Xpos,Ypos,0);
       stand[0].binder();
      //  glutSolidTeapot(1.5);
       drawPlayer();
       glPopMatrix();
       break;

   case 1:
        inAir = false;
       //T->start();
       airT->reset();
       airT->pause();
       //landT->reset();

       glPushMatrix();
       glTranslated(Xpos,startY,0);
        //Ypos = startY;
       if(T->getTicks()>50){

        runspeed++;
        runspeed = runspeed %8;
        T->reset();
       }

       runText[runspeed].binder();
       drawPlayer();

       glPopMatrix();
       break;
   case 2:

            if(Ypos < jumpLoc && !goingdown){
                Ypos+=jumpspeed;
                goingup = true;
            }
            else if(Ypos >= jumpLoc && goingup)
            {
                goingup = false;
                goingdown = true;

            }
            else if(goingdown)
            {
                Ypos-=jumpspeed;
            }


   /*
            if (airT->getTicks()>700){
                actionTrigger = 1;
                landT->reset();
            }
                landT->pause();
                airT->resume();

            glPushMatrix();

            //jumpLoc is 0, why?

            //if(T->getTicks()>10){

                if(airT->getTicks() <= 350)
                {
                    Ypos+=(jumpLoc-startY)/350*10;
                }
                if(airT->getTicks() > 350)
                {
                    Ypos -=(jumpLoc-startY)/350*10;
                }
                /*
                jumpspeed++;
                jumpspeed = runspeed %10;
`               */
            if(Ypos < startY){
                Ypos = startY;
                goingdown = false;
                actionTrigger = 1;
            }
            jump[0].binder();

            glPushMatrix();
            glTranslated(Xpos,Ypos,0);


            drawPlayer();
            glPopMatrix();

          break;
   case 3:
       glPushMatrix();
       glTranslated(Xpos,Ypos,0);
        //Ypos = startY;
       if(T->getTicks()>200){

        runspeed++;
        runspeed = runspeed %2;
        T->reset();
       }

       rTT2[runspeed].binder();
       drawPlayer();

       glPopMatrix();
       break;
   }

}
void player::set_scale(float x, float y)
{

    verticies[1].x*=x;
    verticies[2].x*=x;
    verticies[2].y*=y;
    verticies[3].y*=y;
}
