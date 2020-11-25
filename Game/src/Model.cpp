#include "Model.h"
#include <textureLoader.h>
#include <iostream>
#include <cmath>
using namespace std;
Model::Model()
{
    //ctor
        RotateX =0.0; // Rotations
        RotateY =0.0;
        RotateZ =0.0;

        Zoom = -4.0;  //translations
        Xpos = -0.5;
        Ypos = -0.5;

        verticies[0].x=0.0;verticies[0].y=0.0;verticies[0].z=1.0;
        verticies[1].x=1.0;verticies[1].y=0.0;verticies[1].z=1.0;
        verticies[2].x=1.0;verticies[2].y=1.0;verticies[2].z=1.0;
        verticies[3].x=0.0;verticies[3].y=1.0;verticies[3].z=1.0;
        tex = new textureLoader();
        alreadyScaled = false;
}

Model::~Model()
{
    //dtor
    delete tex;
}
void Model::modelInit(char *fileName, bool trans)
{
   if(trans)
   {
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }

    tex->binder();
    tex->bindTexture(fileName);

}

void Model::drawModel()
{
    glColor3f(0.8,0.5,0.0);
    tex->binder();
    glTranslated(Xpos,Ypos,Zoom);
    glRotated(RotateX,1,0,0);
    glRotated(RotateY,0,1,0);
    glRotated(RotateZ,0,0,1);

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


  //  glutSolidTeapot(1.5);
}
void Model::set_scale(float x, float y)
{
    if (alreadyScaled == false) {
        verticies[1].x*=x;
        verticies[2].x*=x;
        verticies[2].y*=y;
        verticies[3].y*=y;
    }
    alreadyScaled = true;
}


bool Model::isCollided(Model* mdl)
{
    float newx = mdl->Xpos;
    float newy = mdl->Ypos;
    //cout << "new x " << newx << endl;
    //cout <<"new y " << newy << endl;
    //cout << "this x " << this->Xpos << endl;
    //cout <<"this y " << this->Ypos << endl;
    float newx2 = this->Xpos;
    float newy2 = this->Ypos;
    float xdistance = abs(newx-newx2);
    float ydistance = abs((newy)-newy2);


    //cout << mdl->Xpos << endl;;
    //if(xdistance < 50)
    //cout << "x " << xdistance << endl;
    //if(ydistance < 200)
    //cout << "y " << ydistance << endl;
    if (xdistance < .05 && ydistance < .2)
    {
        cout << "COLLISION" << endl;
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
