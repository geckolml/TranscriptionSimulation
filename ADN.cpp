//g++ ADN.cpp sphere.cpp -lGL -lGLU -lglut -lSDL2 -lfreeimage -std=c++11 -o Adn
#include <iostream>
#include <cmath>
#include <vector>
#include <FreeImage.h>
#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "sphere.h"
#define ESC 27
#define NUM_SPH     60
#define NUM_ADN     15
#define MAX 20
#define MAXMAR 41
using namespace std;
bool quit = false;
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 500;

constexpr int OPENGL_MAJOR_VERSION = 2;
constexpr int OPENGL_MINOR_VERSION = 1;

constexpr SDL_GLprofile OPENGL_PROFILE = SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE;

// Titulos
static char titulo[30]="Transcripcion del ADN"; // Titulo de la pantalla
static char letras[20]="Temperatura: ";//impresion en pantalla
static char disp_temperatura[10]="25.4"; // Valor de la temperatura en la pantalla

static char nucleotido[MAX]; // Hebra de nucleotidos ADN
static char comp_nucleotido[MAX]; // Hebra complementaria

static char marNucleotidos[MAX];//="ATGTGCGTGCGTAGTACGTC"; // mar de nucleotidos
static char mar[MAXMAR]; // mar de nucleotidos
static int NN;//numero de nucleotidos leidos
static int MNN;//numero de nucleotidos leidos para el mar

colores colore = {
	{0.0f, 0.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 1.0f, 1.0f},
	{0.5f, 0.0f, 0.5f, 1.0f}
	};
GLuint Estado=0;
GLuint Desna=0;//sirve para desnaturalizar la cadena al presionar 9
GLuint Rev=3;

GLfloat Longi=6;
GLfloat step;
GLfloat RadioM=1.5;
GLfloat Radio=RadioM;
GLfloat DAng;
GLfloat DesAng;
GLfloat py;
GLfloat pz;
GLfloat velgiro;
GLfloat paso;
GLfloat velhe;
GLfloat veltor;
GLfloat espacio=8;

GLfloat LightAmbient[]=	{ 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat LightDiffuse[]=	{ 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat LightPosition[]= { 0.0f, -4.0f, 0.0f, 1.0f };
GLuint	fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	//Tipos de niebla
GLuint fogfilter=1;					//Usado para el modo de niebla

GLfloat	fogColor[4] = {0.5f,0.5f,0.5f,1.0f};
float mat_emission [] = {0.0,0.0,0.0,1.0};
GLUquadric *quadObj;

vector<TSphere> basesn;
vector<TSphere> complementos;
vector<TSphere> hidro;
vector<TSphere> marnucl;
TSphere helicasa(0.5,'E');
//TSphere * bases[NUM_SPH];					//Dibujara la esfera
float lx = 0.0, ly = -10.0; 				//Posiciones de la camara
float r=10.0;						//Distancia de la camara con respecto al centro de la tierra
float deltaMove = 0.0;				//Movimiento de la camara
float angle = 0.0; 					//Angulo de rotacion de la camara
float deltaAngle = 0.0; 				//Angulo de cambio
int isDragging = 0; 					//Mouse arrastrandose
int xDragStart = 0; 					//Guarda la posicion del mouse cuando se arrastra
GLuint texID[1];					//ID de la textura de la tierra
SDL_Window* displayWindow=NULL;				//Ventana principal
SDL_GLContext context;					//Aqui se dibuja
float velmar = 0.2; 	
bool init();

//bool loadTextures();

bool loadMedia();

void handleKeys( SDL_Event& e );

void calcNN(){
  int ix=0;
  for(int i=0; i<MAX && nucleotido[i]!='\0';i++){
    TSphere bas(0.2,nucleotido[i]);
    basesn.push_back (bas);
    ix++;
  }
  NN=ix;
  step=2*Longi/(NN-1);
  DAng=(Rev* M_PI)/(GLfloat)(NN-1);
  DesAng=DAng;
  veltor=(step*velmar)/(2*espacio);
  velgiro=0.05f;
  paso=(DAng*100.0);
  velhe=(step)/(paso);
}

void hidrogenos(){
  for(int i=0; i<NN;i++){
    TSphere bas(0.1,'H');
    hidro.push_back (bas);
  }
}

void complementario(){
  for(int i =0; i < NN;i++){
    TSphere bas(0.2);
    if(nucleotido[i]=='A'){
      bas.setcolor('T');
      comp_nucleotido[i]='T';
    }
    if(nucleotido[i]=='T'){
      bas.setcolor('A');
      comp_nucleotido[i]='A';
    }
    if(nucleotido[i]=='C'){
      bas.setcolor('G');
      comp_nucleotido[i]='G';
    }
    if(nucleotido[i]=='G'){
      bas.setcolor('C');
      comp_nucleotido[i]='C';
    }
    complementos.push_back (bas);
  }
}

void marInit(){
  int ix=0;
  for(int i=0; i<MAXMAR && mar[i]!='\0';i++){
    TSphere bas(espacio,velmar,0.2,mar[i]);
    marnucl.push_back (bas);
    ix++;
  }
  MNN=ix;
}


// Dibujar
bool init()
{
  srandom(time(NULL));
  bool success = true;
  FILE * fichero;
  fichero = fopen("cadenaADN.txt","rt");
  fgets(nucleotido,MAX,fichero);
  fclose(fichero);
  fichero = fopen("MarN.txt","rt");
  fgets(mar,MAXMAR,fichero);
  fclose(fichero);
  cout<<"========================================="<<endl;
  cout<<"Archivo cadenaADN.txt leído: "<<endl;
  cout<<"Cadena de Nucleotidos leidos: "<<endl;
  cout<<nucleotido<<endl;
  calcNN();
  cout<<"========================================="<<endl;
  cout<<"Iniciando Calculo de Complementario:"<<endl;	
  complementario();
  cout<<"Cadena de Nucleotidos Complementarios: "<<endl;
  cout<<comp_nucleotido<<endl;
  cout<<"========================================="<<endl;
  cout<<"Archivo MarN.txt leido: "<<endl;
  cout<<"Nucleotidos leidos para el mar de Nucleotidos: "<<endl;	
  cout<<mar<<endl;
  cout<<"========================================="<<endl;
  marInit();
  hidrogenos();
  //Inicializamos SDL, video y audio
  
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    std::cerr << "There was an error initing SDL2: " << SDL_GetError() << std::endl;
    success = false;
  }
  else
  {
    //Uso de OpenGL 2.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, OPENGL_PROFILE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //Creacion de la ventana
    displayWindow = SDL_CreateWindow("ADN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    if( displayWindow == NULL )
    {
      std::cerr << "There was an error creating the window: " << SDL_GetError() << std::endl;
      success = false;
    }
    else
    {
      //Creamos el lienzo donde se dibujara
      context = SDL_GL_CreateContext(displayWindow);
      if( context == NULL )
      {
        std::cerr << "There was an error creating OpenGL context: " << SDL_GetError() << std::endl;
        success = false;
      }
      else
      {
        const unsigned char *version = glGetString(GL_VERSION);
        if (version == NULL)
        {
          std::cerr << "There was an error with OpenGL configuration:" << std::endl;
          success = false;
        }	
      }
    }
  }
  return success;
}

bool loadMedia()
{
  bool success = true;
  //Se carga la musica
  return success;
}

void handleKeys( SDL_Event& e )
{
  //Si un boton es presionado
  if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
  {
    //Activamos el movimiento
    switch( e.key.keysym.sym )
    {
      case SDLK_UP: deltaMove = 1.0; break;
      case SDLK_DOWN: deltaMove = -1.0; break;
      //case SDLK_LEFT: mVelX -= DOT_VEL; break;
      //case SDLK_RIGHT: mVelX += DOT_VEL; break;
      case SDLK_c: Estado=1;break;
      case SDLK_ESCAPE: quit=true; break;
      case SDLK_MINUS: break;
      case SDLK_KP_MINUS: break;
      case SDLK_PLUS: break;
      case SDLK_KP_PLUS: break;
      
    }
  }
  //Si el boton es soltado 
  else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
  {
    //Desactivamos el movimiento
    switch( e.key.keysym.sym )
    {
      case SDLK_UP: deltaMove = 0.0; break;
      case SDLK_DOWN: deltaMove = 0.0; break;
      //case SDLK_LEFT: mVelX += DOT_VEL; break;
      //case SDLK_RIGHT: mVelX -= DOT_VEL; break;
      case SDLK_8: 
      if( Desna == 0 )
      {
        Desna=1;
      }
      else Desna=0; break;
      //Sirve para iniciar y pausar la musica
    }
  }
  else if( e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
  {
    //Tomamos la posicion del mouse
    int x, y;
    SDL_GetMouseState( &x, &y );
    switch( e.type )
    {
      case SDL_MOUSEMOTION:
      if (isDragging)
      {
        deltaAngle = (x - xDragStart) * 0.005;
        //actualizamos la posicon de la camara
        lx = sin(angle + deltaAngle)*r;
        ly = -cos(angle + deltaAngle)*r;
      }
      break;		
      case SDL_MOUSEBUTTONDOWN:
      isDragging = 1; //Empieza el desplazamiento
      xDragStart = x;
      break;	
      case SDL_MOUSEBUTTONUP:
      angle += deltaAngle; //actualizamos el angulo
      isDragging = 0;
      break;
    }	
  }
}


void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
  GLdouble fW, fH;
  fH = tan(fovY / 360 * M_PI) * zNear;
  fW = fH * aspect;
  glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void renderBitmapString(float x, float y, float z, void *font, char *string){
  char *c;
  glRasterPos3f(x,y,z);
  for(c=string; *c != '\0';c++) {
    glColor3f(1.0,1.0,1.0);
    glutBitmapCharacter(font, *c);
  }
}

bool Display_InitGL() {
  bool success = false;
  int j;
  
  GLenum error = GL_NO_ERROR;
  //glEnable(GL_TEXTURE_2D);	//habilitamos el uso de texturas
  glShadeModel( GL_SMOOTH);	//Habilitamos el smooth
  glClearColor(0, 0, 0, 0.5);	//Ponemos el background de color negro
  glClearDepth(1.0f);		//Definimos el Depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);	//Luz Ambiental
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);	//Luz Difusa
  glLightfv(GL_LIGHT1, GL_SPECULAR, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	//Posicion de la luz
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  quadObj = gluNewQuadric();
  gluQuadricNormals(quadObj, GLU_SMOOTH);
  
  glFogi(GL_FOG_MODE, fogMode[fogfilter]);		// Fog Mode
  glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
  glFogf(GL_FOG_DENSITY, 0.01f);
  glHint(GL_FOG_HINT, GL_DONT_CARE);			// Fog Hint Value
  glFogf(GL_FOG_START, 1.0f);				// Fog Start Depth
  glFogf(GL_FOG_END, 5.0f);				// Fog End Depth
  glEnable(GL_FOG);
  //glEnable(GL_COLOR_MATERIAL);
  glLineWidth(5.0);
  if( error != GL_NO_ERROR )
  {
    printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    success = true;
  }
  return success;
}
float * pos;
int Display_SetViewport(int width, int height) {
  /* Height / width ration */
  GLfloat ratio;
  /* Protect against a divide by zero */
  if (height == 0) {
    height = 1;
  }
  ratio = (GLfloat) width / (GLfloat) height;
  /* Setup our viewport. */
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  /* change to the projection matrix and set our viewing volume. */
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  /* Set our perspective */
  perspectiveGL(45.0f, ratio, 0.1f, 100.0f);
  /* Make sure we're chaning the model view and not the projection */
  glMatrixMode( GL_MODELVIEW);
  /* Reset The View */
  glLoadIdentity();
  return 1;
}

GLfloat AngInicial=0;

float torstep=1.5*Longi;

void MarRender(){
  for (int i=0;i<MNN;i++)
  {
    glPushMatrix();
    marnucl[i].render(quadObj);
    marnucl[i].test();
  }
}

GLfloat helipos=-Longi*1.0;

void Giro(void){
  int j;
  GLfloat Ang=0;
  GLfloat i=-Longi,ai=-Longi;
  //GLfloat apy= -sin(Ang)*Radio,apz = cos(Ang)*Radio;
  if(AngInicial<-2*M_PI)
    AngInicial=0.0;
  else
    AngInicial-=0.01;

  for(j=0;j<NN;j++){
    py = sin(Ang+AngInicial)*Radio;
    pz = cos(Ang+AngInicial)*Radio;
    Ang+=DesAng;
    basesn[j].setpos(i, py, pz);
    basesn[j].render(quadObj);

    complementos[j].setpos(i, -py, -pz);
    complementos[j].render(quadObj);

    hidro[j].setpos(i, 0.0, 0.0);
    hidro[j].render(quadObj,i, py, pz);
    if(j>0){
    glBegin(GL_LINES);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colore.celeste);
    glVertex3fv(basesn[j-1].getPosv());
    glVertex3fv(basesn[j].getPosv());
    glVertex3fv(complementos[j-1].getPosv());
    glVertex3fv(complementos[j].getPosv());
    glEnd();
    }
    ai=i;
   // apy=py;
   // apz=pz;

    i+=step;
  }
  MarRender();
}

void Helica(void){
  int j;
  GLfloat Ang=0;
  GLfloat i=-Longi,ai=-Longi;
  bool cont=true;
  if(AngInicial<-2*M_PI)
    AngInicial=0.0;
  else
    AngInicial-=0.01;

  for(j=0;j<NN;j++){
    if(i<helipos+0.5){
      hidro[j].setdibujo(false);
      basesn[j].stop(i, 0.0, Radio,0.01);
      marnucl[j].setparada(i, 0.0, Radio*0.4,0.6);
      complementos[j].stop(i, 0.0, -Radio,0.01);
      marnucl[2*NN-j-1].setparada(i, 0.0, -Radio*0.4,0.6);
    }
   
    
    py = sin(Ang+AngInicial)*Radio;
    pz = cos(Ang+AngInicial)*Radio;
    Ang+=DesAng;
    basesn[j].setpos(i, py, pz);
    cont=basesn[j].getparada();
    basesn[j].render(quadObj);

    complementos[j].setpos(i, -py, -pz);
    complementos[j].render(quadObj);

    hidro[j].setpos(i, 0.0, 0.0);
    hidro[j].render(quadObj,i, py, pz);
    if(j>0){
    glBegin(GL_LINES);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colore.celeste);
    glVertex3fv(basesn[j-1].getPosv());
    glVertex3fv(basesn[j].getPosv());
    glVertex3fv(complementos[j-1].getPosv());
    glVertex3fv(complementos[j].getPosv());
    glEnd();
    }
    ai=i;
    i+=step;
  }
if(cont==true)
 Estado=2;
MarRender();
helicasa.setpos(helipos, 0.0, 0.0);
helicasa.render(quadObj);
helipos+=velhe;
  
}

void Poli(void){
  int j;
  char c;
  GLfloat p;
  GLfloat i=-Longi,ai=-Longi;
  bool cont;
  
  for(j=0;j<NN;j++){
    cont=true;
    basesn[j].render(quadObj);
    p=basesn[j].getx();
    if((-torstep-0.12)<p&&(-torstep+0.2)>p){
      //if(-torstep>p){
      //  marnucl[j].setlugar();}
      if(false==marnucl[j].getparada()){
        marnucl[j].setdir();
        marnucl[j].setcompara(true);
      }
      
    }
    complementos[j].render(quadObj);
    if((torstep-0.2)<-p&&(torstep+0.12)>-p){
      //if(torstep<-p)
        //marnucl[NN+j].setlugar();
      if(false==marnucl[NN+j].getparada()){
        marnucl[NN+j].setdir();
        marnucl[NN+j].setcompara(true);
      }
      
    }
    if(j>0){
    glBegin(GL_LINES);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colore.celeste);
    glVertex3fv(basesn[j-1].getPosv());
    glVertex3fv(basesn[j].getPosv());
    glVertex3fv(complementos[j-1].getPosv());
    glVertex3fv(complementos[j].getPosv());
    glEnd();
    }
    i+=step;
  }

  MarRender();
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colore.morado);

glPushMatrix();
  glTranslated(-torstep,0,Radio);
  glRotatef(90,0.0,1.0,0.0);
  glutSolidTorus(0.4, 1.0, 28, 28);
glPopMatrix();
glPushMatrix();
  glTranslated(torstep,0,-Radio);
  glRotatef(90,0.0,1.0,0.0);
  glutSolidTorus(0.4, 1.0, 28, 28);
glPopMatrix();
torstep-=veltor;
  

}
void ADN(void){

  GLfloat Ang=0;
  glMaterialf(GL_FRONT,GL_SHININESS,0.5);
  int j;
  float i=-Longi,ai=-Longi;
  float apy= -sin(Ang)*Radio,apz = cos(Ang)*Radio;
   if(Desna==0&&Radio==RadioM)
      DesAng+=0.001;
   else if(Desna==1)
      DesAng-=0.001;
   
   if(DesAng<0)
      DesAng=0;
   if(DesAng>DAng)
      DesAng=DAng;

   if(DesAng==0&&Desna==1)
      Radio+=0.01;
   if(DesAng==0&&Desna==0)
      Radio-=0.01;
   
   if(Radio<RadioM)
      Radio=RadioM;
   if(Radio>2*RadioM)
      Radio=2*RadioM;
 
    pz = cos(Ang)*Radio;
for(j=0;j<NN;){
    py = -sin(Ang)*Radio;
    pz = cos(Ang)*Radio;
    Ang+=DesAng;
    glPushMatrix();
    
    glTranslated(i, py, pz);
basesn[j].render(quadObj);

//bases[j++]->render(quadObj);
    //gluSphere (quadObj, 0.2,8,8); // Esfera Nativa de Open GL
    glPopMatrix();
    glPushMatrix();
    
    glTranslated(i, -py, -pz);
complementos[j].render(quadObj);
//bases[j++]->render(quadObj);
    //gluSphere (quadObj, 0.2,20,20); // Esfera Nativa de Open GL
    glPopMatrix();
    glBegin(GL_LINES);
//glLineWidth(20.0);
j++;
if(Radio==RadioM){
glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colore.blanco);
glVertex3f(i, py, pz);
glVertex3f(i, -py, -pz);
}
glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colore.celeste);
glVertex3f(ai, apy, apz);
glVertex3f(i, py, pz);
glVertex3f(ai, -apy, -apz);
glVertex3f(i, -py, -pz);
ai=i;
apy=py;
apz=pz;
glEnd();
	/*glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);*/
    i+=step;
}

MarRender();

glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colore.morado);
if(Radio==2*RadioM){
glPushMatrix();
  glTranslated(-torstep,0,Radio);
  glRotatef(90,0.0,1.0,0.0);
  glutSolidTorus(0.25, 0.75, 28, 28);
glPopMatrix();
glPushMatrix();
  glTranslated(torstep,0,-Radio);
  glRotatef(90,0.0,1.0,0.0);
  glutSolidTorus(0.25, 0.75, 28, 28);
glPopMatrix();
torstep+=0.03;
}

  
}

void Display_Render(SDL_Window* displayWindow) {
  /* Clear The Screen And The Depth Buffer */
  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 0.75);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(lx, ly, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  //glBindTexture(GL_TEXTURE_2D, texID[0]);
  //gluSphere (quadObj, 4,40,40);
  switch(Estado){
  case 0: Giro(); break;
  case 1: Helica(); break;
  case 2: Poli(); break;
  }
  //ADN();
  SDL_GL_SwapWindow(displayWindow);//Actualiza el dibujo
}

void close()
{
  //Destroy window
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(displayWindow);
  displayWindow = NULL;
  SDL_Quit();	
}

void update(void) 
{
  if (deltaMove) { // update camera position
    r += -(float)deltaMove * 0.1;
    lx = sin(angle)*r;
    ly = -cos(angle)*r;
  }
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  if( !init() )
  {
    printf( "Failed to initialize!\n" );
  }
  else
  {
    if( !loadMedia() )
    {
      printf( "Failed to load media!\n" );
    }
    else{
      SDL_GL_MakeCurrent(displayWindow, context);
      quit=Display_InitGL();
      Display_SetViewport(SCREEN_WIDTH, SCREEN_HEIGHT);
      //Event handler
      SDL_Event e;
      while( !quit )
      {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
          //User requests quit
          if( e.type == SDL_QUIT )
          {
            quit = true;
          }
          handleKeys(e );
        }
        update();
        Display_Render(displayWindow);
      }
	
    }
  }
  close();
  return 0;
}
