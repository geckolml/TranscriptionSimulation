#include <GL/glut.h>
#include "sphere.h"
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
TSphere::TSphere(GLfloat maxpos, GLfloat speed, GLfloat r, char c)
{
  compara=false;
  alto=false;
  radio=r;
  dibujo=true;
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

void TSphere::setdibujo(bool v){
  dibujo=v;
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


GLfloat * TSphere::getPosv()
{
  return pos;
}
