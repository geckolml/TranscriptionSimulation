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
GLfloat blanco[4];
GLfloat plomo[4];//puente de hidrogeo
GLfloat celeste[4];
GLfloat morado[4];//timina
}colores;

class TSphere
{
private:
  bool dibujo;
  bool alto;
  bool compara;
  TSphere * par;
  GLfloat radio,del;
  GLfloat maxpos;
  GLfloat pos[3];
  GLfloat parada[3];
  GLfloat dir[3];
  GLfloat speed;
  GLfloat* color;
  char colorpar;
public:
  TSphere(GLfloat maxpos, GLfloat speed,GLfloat r);
  TSphere(GLfloat maxpos, GLfloat speed, GLfloat r,char c);
  TSphere(GLfloat r,char c);
  TSphere(GLfloat r);
  ~TSphere();
  void test();
  void renderBitmapString(float x, float y, float z, void *font, char *string);
  void setdibujo(bool v);
  void setcolor(char c);
  void setpos(GLfloat x, GLfloat y, GLfloat z);
  void setparada(GLfloat x, GLfloat y, GLfloat z, GLfloat s);
  void setdir();
  void setlugar();
  void setcompara(bool v);
  void stop(GLfloat x, GLfloat y, GLfloat z, GLfloat s);
  void link();
  void render(GLUquadric* g);
  void render(GLUquadric* g, GLfloat x1, GLfloat y1, GLfloat z1);
  //void modifySpeed(float inc);
  float * getPosv();
  float getx();
  bool getparada();
  char getcolorpar();
};
#endif
