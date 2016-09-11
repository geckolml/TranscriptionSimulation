#include <GL/glut.h>
#include "sphere.h"
//GLuint pass=0;
float shininess=12;
  GLfloat materialEmission[] = {0.0,0.0,0.0, 1.0};
  GLfloat materialSpecular[] = {0.3,0.3,0.3, 1.0};

colores colo = {
	{0.0, 0.0, 1.0, 1.0},
	{1.0, 0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 1.0},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 1.0f, 1.0f},
	{0.5f, 0.0f, 0.5f, 1.0f}
	};
TSphere::TSphere(GLfloat maxpos, GLfloat speed, GLfloat r, char c)
{
  radio=r;
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
  radio=r;
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
  radio=r;
  this->setcolor(c);
}

TSphere::~TSphere()
{
}

void TSphere::setcolor(char C)
{
  if(C=='A'){
      color=colo.azul;
  }
  if(C=='T'){
      color=colo.amarillo;
  }
  if(C=='C'){
     color=colo.rojo;
  }
  if(C=='G'){
     color=colo.verde;
  }
  if(C=='H'){
     color=colo.blanco;
  }
}

void TSphere::test()
{
  ((pos[0] < -maxpos) || (pos[0] > maxpos))?dir[0]*=-1:0;
  ((pos[1] < -maxpos) || (pos[1] > maxpos))?dir[1]*=-1:0;
  ((pos[2] < -maxpos) || (pos[2] > maxpos))?dir[2]*=-1:0;
  pos[0] += dir[0];
  pos[1] += dir[1];
  pos[2] += dir[2];
}

void TSphere::link()
{
  ((pos[0] < -maxpos) || (pos[0] > maxpos))?dir[0]*=-1:0;
  ((pos[1] < -maxpos) || (pos[1] > maxpos))?dir[1]*=-1:0;
  ((pos[2] < -maxpos) || (pos[2] > maxpos))?dir[2]*=-1:0;
  pos[0] += dir[0];
  pos[1] += dir[1];
  pos[2] += dir[2];
}

void TSphere::unlink()
{
  ((pos[0] < -maxpos) || (pos[0] > maxpos))?dir[0]*=-1:0;
  ((pos[1] < -maxpos) || (pos[1] > maxpos))?dir[1]*=-1:0;
  ((pos[2] < -maxpos) || (pos[2] > maxpos))?dir[2]*=-1:0;
  pos[0] += dir[0];
  pos[1] += dir[1];
  pos[2] += dir[2];
}

void TSphere::render(GLUquadric* g)
{
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess); // parametro de brillantez
  gluSphere (g, radio,20,20);     
}

GLfloat * TSphere::getPosv()
{
  return pos;
}
