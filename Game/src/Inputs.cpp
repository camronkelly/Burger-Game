#include "Inputs.h"
#include <player.h>

Inputs::Inputs()
{
    //ctor
   prev_Mouse_X =0;
   prev_Mouse_Y =0;
   Mouse_Translate=0;
   Mouse_Roatate=0;
}

Inputs::~Inputs()
{
    //dtor
}
void Inputs::keyEnv(parallax* plx, float speed)
{
    switch(wParam)
    {
        case VK_LEFT:
            //plx->Xmin -=speed;
            //plx->Xmax -=speed;
            break;

        case VK_RIGHT:
            //plx->Xmin +=speed;
            //plx->Xmax +=speed;
            break;

         case VK_UP:
            //plx->Ymin -=speed;
            //plx->Ymax -=speed;
            break;

        case VK_DOWN:
            //plx->Ymin +=speed;
            //plx->Ymax +=speed;
            break;

    }
}


void Inputs::keyPressed(Model* Mdl)
{
    switch(wParam)
    {
    case VK_LEFT:
        Mdl->RotateX +=1.0;
        break;

    case VK_RIGHT:
        Mdl->RotateX -=1.0;
        break;

    case VK_DOWN:
        Mdl->RotateY -=1.0;
        break;

    case VK_UP:
        Mdl->RotateY +=1.0;
        break;

    case VK_ADD:
        Mdl->Zoom +=1.0;
        break;

    case VK_SUBTRACT:
        Mdl->Zoom -=1.0;
        break;
    }
}

void Inputs::keyUp(player* ply)
{
    if(ply->inAir == true)
        ply->actionTrigger = 2;
    if(ply->inAir == false && ply->runTrigger == 1){
    if(ply->jumpTrigger == 1){ply->jumpTrigger = 0;}
    //if(ply->jumpTrigger == 0){ply->actionTrigger = 1;}
    ply->actionTrigger =1;
    ply->Ypos = ply->startY;
    }
    switch(wParam)
    {
       default:break;
    }
}


void Inputs::keyPressed(player* ply)
{
    switch(wParam)
    {
     case VK_LEFT:
       //ply->actionTrigger =0;
        break;

    case VK_RIGHT:
       ply->actionTrigger =1;
       ply->runTrigger = 1;
        break;

    case VK_DOWN:

        break;

    case VK_UP:
        //ply->actionTrigger = 2;
        if(ply->runTrigger == 1){ply->actionTrigger = 2;}
        if(ply->runTrigger == 1){ply->jumpTrigger = 1;
        ply->inAir = true;}
        break;

    case VK_ADD:

        break;

    case VK_SUBTRACT:

        break;
    }
}


void Inputs::keyUP()
{
  switch (wParam)
            {
                default:
                break;
            }
}

void Inputs::mouseEventDown(Model *Model, double x,double y)
{
        prev_Mouse_X =x;
        prev_Mouse_Y =y;

   switch (wParam)
            {
                case MK_LBUTTON:
                        Mouse_Roatate = true;
                    break;

                case MK_RBUTTON:
                     Mouse_Translate =true;
                    break;
                case MK_MBUTTON:

                    break;

                default:
                    break;
            }
}

 void Inputs::mouseEventUp()
 {
    Mouse_Translate =false;
    Mouse_Roatate =false;
 }

void Inputs::mouseWheel(Model *Model,double Delta)
{
    Model->Zoom += Delta/100;
}

void Inputs::mouseMove(Model *Model,double x,double y)
{
      if(Mouse_Translate)
      {
       Model->Xpos += (x-prev_Mouse_X)/100;
       Model->Ypos -= (y-prev_Mouse_Y)/100;

       prev_Mouse_X =x;
       prev_Mouse_Y =y;
      }

      if(Mouse_Roatate)
      {
        Model->RotateY += (x-prev_Mouse_X)/3;
        Model->RotateX += (y-prev_Mouse_Y)/3;

        prev_Mouse_X =x;
        prev_Mouse_Y =y;
      }
}
void Inputs::keyPressed(skyBox* sky)
{
    switch(wParam)
    {
     case VK_LEFT:
       sky->RotateY += 0.5;
        break;

    case VK_RIGHT:
       sky->RotateY -= 0.5;
        break;

    case VK_DOWN:
         sky->RotateX += 0.5;
        break;

    case VK_UP:
         sky->RotateX -= 0.5;
        break;
    }
}

void Inputs::mouseMove(skyBox* sky, double x, double y)
{

        sky->RotateY += (x-prev_Mouse_X)/3;
        sky->RotateX += (y-prev_Mouse_Y)/3;

        prev_Mouse_X =x;
        prev_Mouse_Y =y;

}

void Inputs::keySound(sounds* snds)
{
     switch(wParam)
    {
      case VK_SPACE:
          snds->playSound("sounds/shooting.mp3", false);

      break;
      case VK_TAB:
          if (snds->isPlaying == false) {
            snds->playMusic("sounds/music.mp3", true);
            snds->isPlaying = true;
          }
          else {
            snds->pauseSounds(true);
            //snds->stopAllSounds();
            snds->isPlaying = false;

          }
      break;
      case VK_UP:
        snds->playSound("sounds/jumping.mp3", false);
        break;
    }

}
