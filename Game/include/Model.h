#ifndef MODEL_H
#define MODEL_H

#include<windows.h>
#include<GL/glut.h>
#include <textureLoader.h>

typedef struct{
 float x;
 float y;
 float z;
}vec1;

class Model
{

    public:
        Model();
        virtual ~Model();
        void drawModel();
        void modelInit(char *,bool);

        double RotateX;
        double RotateY;
        double RotateZ;

        double Zoom;
        float Xpos;
        float Ypos;

        void set_scale(float, float);

        vec1 verticies[4];
        bool alreadyScaled;


        bool isCollided(Model* );

    protected:

    private:
        textureLoader* tex;
};

#endif // MODEL_H
