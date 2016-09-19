#include <GL/glut.h>
#include "sphere.h"
#include <iostream>

//GLuint pass=0;
float shininess=18;
  GLfloat materialEmission[] = {0.0,0.0,0.0, 1.0};
  GLfloat materialSpecular[] = {0.3,0.3,0.3, 1.0};

colores colo = {
	{0.0f, 0.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{0.5f, 0.5f, 0.5f, 0.5f},
	{0.0f, 1.0f, 1.0f, 1.0f},
	{0.5f, 0.0f, 0.5f, 1.0f}
	};
TSphere::TSphere(GLfloat maxpos, GLfloat sp, GLfloat r, char c)
{
  compara=false;
  alto=false;
  radio=r;
  dibujo=true;
  speed=sp;
  this->setcolor(c);
  this->maxpos = maxpos;
  pos[0] = (random() % (GLuint)maxpos) - maxpos/2;
  pos[1] = (random() % (GLuint)maxpos) - maxpos/2;
  pos[2] = (random() % (GLuint)maxpos) - maxpos/2;
  dir[0] = random();
  dir[1] = random();
  dir[2] = random();
  GLfloat dirmod =sqrt(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2]);
  dir[0] /= dirmod;
  dir[1] /= dirmod;
  dir[2] /= dirmod;
  dir[0] *= speed;
  dir[1] *= speed;
  dir[2] *= speed;
}

TSphere::TSphere(GLfloat maxpos, GLfloat speed,GLfloat r)
{
  compara=false;
  alto=false;
  radio=r;
  dibujo=true;
  this->maxpos = maxpos;
  pos[0] = (random() % (GLuint)maxpos) - maxpos/2;
  pos[1] = (random() % (GLuint)maxpos) - maxpos/2;
  pos[2] = (random() % (GLuint)maxpos) - maxpos/2;
  dir[0] = random();
  dir[1] = random();
  dir[2] = random();
  GLfloat dirmod =sqrt(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2]);
  dir[0] /= dirmod;
  dir[1] /= dirmod;
  dir[2] /= dirmod;
  dir[0] *= speed;
  dir[1] *= speed;
  dir[2] *= speed;
}

TSphere::TSphere(GLfloat r,char c)
{
  compara=false;
  alto=false;
  radio=r;
  dibujo=true;
  this->setcolor(c);
}

TSphere::TSphere(GLfloat r)
{
  compara=false;
  alto=false;
  radio=r;
  dibujo=true;
}

TSphere::~TSphere()
{
}

void TSphere::setcolor(char C)
{
  tipo=C;
  if(C=='A'){
    color=colo.azul;
    tipopar='T';
  }
  if(C=='T'){
    color=colo.amarillo;
    tipopar='A'; 
  }
  if(C=='C'){
    color=colo.rojo;
    tipopar='G';
  }
  if(C=='G'){
    color=colo.verde;
    tipopar='C';
  }
  if(C=='H'){
     color=colo.plomo;
  }
  if(C=='E'){
     color=colo.morado;
  }	
}
void TSphere::stop(GLfloat x, GLfloat y, GLfloat z, GLfloat s)
{
  parada[0]=x;
  parada[1]=y;
  parada[2]=z;
  del=s;
  compara=true;
}

void TSphere::setpos(GLfloat x, GLfloat y, GLfloat z){
  if(!alto){
    pos[0]=x;
    pos[1]=y;
    pos[2]=z;
    if(compara)
      if((pos[0]>(parada[0]-del))&&
        (pos[1]>(parada[1]-del))&&
        (pos[2]>(parada[2]-del))&&
        (pos[0]<(parada[0]+del))&&
        (pos[1]<(parada[1]+del))&&
        (pos[2]<(parada[2]+del)))
        alto=true;
  }
}

void TSphere::setcompara(bool v){
  compara=v;
}

void TSphere::setparada(GLfloat x, GLfloat y, GLfloat z,  GLfloat s){
  parada[0]=x;
  parada[1]=y;
  parada[2]=z;
  del=s;
}

void TSphere::renderBitmapString(float x, float y, float z, void *font){
  //char *c;
  glRasterPos3f(x,y,z);
  glutBitmapCharacter(font, tipo);
}


void TSphere::setdibujo(bool v){
  dibujo=v;
}

void TSphere::setdir(){
  dir[0] = parada[0]-pos[0];
  dir[1] = parada[1]-pos[1];
  dir[2] = parada[2]-pos[2];
  GLfloat dirmod =sqrt(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2]);
  dir[0] /= dirmod;
  dir[1] /= dirmod;
  dir[2] /= dirmod;
  dir[0] *= speed;
  dir[1] *= speed;
  dir[2] *= speed;
}

char TSphere::gettipopar(){
  return tipopar;
}


void TSphere::test()
{
  if(!alto){
    pos[0] += dir[0];
    pos[1] += dir[1];
    pos[2] += dir[2];
    if(compara){
      if((pos[0]>(parada[0]-del))&&
        (pos[1]>(parada[1]-del))&&
        (pos[2]>(parada[2]-del))&&
        (pos[0]<(parada[0]+del))&&
        (pos[1]<(parada[1]+del))&&
        (pos[2]<(parada[2]+del))){
        alto=true;
        if(alto){
          pos[0] = parada[0];
          pos[1] = parada[1];
          pos[2] = parada[2];
	  compara=false;
        }
      }
    }  
    else{
      ((pos[0] < -maxpos) || (pos[0] > maxpos))?dir[0]*=-1:0;
      ((pos[1] < -maxpos) || (pos[1] > maxpos))?dir[1]*=-1:0;
      ((pos[2] < -maxpos) || (pos[2] > maxpos))?dir[2]*=-1:0;
    }
  }
}

void TSphere::direccion()
{
  if(!alto){
  std::cout<< pos[0]<<" "<< pos[1]<<" "<< pos[2]<<std::endl;
  std::cout<< dir[0]<<" "<< dir[1]<<" "<< dir[2]<<std::endl;
  }
}


void TSphere::setlugar(){
    pos[0] = parada[0];
    pos[1] = parada[1];
    pos[2] = parada[2];
}


void TSphere::link()
{
  
}


void TSphere::render(GLUquadric* g)
{
  if(dibujo){
    glPushMatrix();
    glTranslated(pos[0], pos[1], pos[2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess); // parametro de brillantez
    gluSphere (g, radio,20,20);
   // renderBitmapString(0.0,-0.4,0,GLUT_BITMAP_HELVETICA_18);
    glPopMatrix();
  }
}
//se usara para los puentes de hidrogeno
void TSphere::render(GLUquadric* g, GLfloat x1, GLfloat y1, GLfloat z1)
{
  if(dibujo){
    glPushMatrix();
    glTranslated(pos[0], pos[1], pos[2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess); // parametro de brillantez
    gluSphere (g, radio,20,20);  
    glPopMatrix();

    glBegin(GL_LINES);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 25);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, -y1, -z1);
    glEnd();
  }
}


bool TSphere::getparada()
{
  return alto;
}

GLfloat TSphere::getx()
{
  return pos[0];
}

GLfloat * TSphere::getPosv()
{
  return pos;
}
