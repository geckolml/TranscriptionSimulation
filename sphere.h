/////////////////////////////////////////////////////////
// Fichero sphere.h
/////////////////////////////////////////////////////////
#ifndef sphere_h
#define sphere_h
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <GL/glut.h>
typedef struct colores{
GLfloat azul[4];//adenina
GLfloat rojo[4];//citosina
GLfloat amarillo[4];//timina
GLfloat verde[4];//guanina
GLfloat blanco[4];//timina
GLfloat celeste[4];//guanina
GLfloat morado[4];//timina
}colores;

class TSphere
{
private:
  TSphere * par;
  GLfloat radio;
  GLfloat maxpos;
  GLfloat pos[3];
  GLfloat dir[3];
  GLfloat speed;
  GLfloat* color;
public:
  TSphere(GLfloat maxpos, GLfloat speed,GLfloat r);
  TSphere(GLfloat maxpos, GLfloat speed, GLfloat r,char c);
  TSphere(GLfloat r,char c);
  ~TSphere();
  void test();
  void setcolor(char c);
  void link();
  void unlink();
  void render(GLUquadric* g);
  //void modifySpeed(float inc);
  float * getPosv();
};
#endif
