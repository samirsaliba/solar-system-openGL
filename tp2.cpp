//g++ testev6.cpp -o testev6 -lGL -lGLU -lglut -lGLEW -lSOIL -lm
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <SOIL/SOIL.h>
#include <cmath>
#include <SFML/Audio.hpp>
#include <string.h>
#include "tp2.h"
using namespace std;

// CLASSES

// Classe camera
class Camera{   
  public:

  float eyeX, eyeY, eyeZ;
  float centerX, centerY, centerZ;
  float upX, upY, upZ;

  void setar(float eX, float eY, float eZ, float cX, float cY, float cZ, float uX, float uY, float uZ){
    eyeX = eX;
    eyeY = eY;
    eyeZ = eZ;

    centerX = cX;
    centerY = cY;
    centerZ = cZ;

    upX = uX;
    upY = uY;
    upZ = uZ;
  }

  void look(){
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
  }
};

const int quantidadeCameras = 12;
int cameraAtual = 0;
Camera *cam = (Camera*) malloc(quantidadeCameras*sizeof(Camera));

// Classe planeta

class Planeta{   
  public:

  string nome;
  GLuint textura;
  float distanciaSol;
  float raio;
  float anguloDeTranslacao = 0;
  float anguloDeRotacao = 0;
  float periodoRotacao;


  float velRotacao;
  float velTranslacao;
  float periodoTranslacao;

  int quantidadeFilhos = 1;
  Planeta *filhos = (Planeta*) malloc(quantidadeFilhos*sizeof(Camera));

  void solidSphere(int radius, int stacks, int columns){
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    // chama 01 glNormal para cada vértice.. poderia ser
    // GLU_FLAT (01 por face) ou GLU_NONE
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    // chama 01 glTexCoord por vértice
    gluQuadricTexture(quadObj, GL_TRUE);
    // cria os vértices de uma esfera
    gluSphere(quadObj, radius, stacks, columns);
    // limpa as variáveis que a GLU usou para criar
    // a esfera
    gluDeleteQuadric(quadObj);
  }

  void desenhaCubo(){
    glRotatef(anguloDeTranslacao, 0, 1, 0); //translacao
    glTranslatef(distanciaSol, 0, 0); //translacao

    glRotatef(anguloDeRotacao, 0, 1, 0); //rotacao
    
    glScalef(2*raio, 2*raio, 2*raio);
    
    //Frente
    glBegin(GL_POLYGON);
      glColor3f(   1.0,  0.0,  0.0 );
      glVertex3f( -0.5, -0.5, -0.5);       // P1
      glVertex3f( -0.5,  0.5, -0.5);       // P2
      glVertex3f(  0.5,  0.5, -0.5);       // P3
      glVertex3f(  0.5, -0.5, -0.5);       // P4
    glEnd();

    // Lado branco - TRASEIRA
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  0.0, 1.0 );
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();
     
    // Lado roxo - DIREITA
    glBegin(GL_POLYGON);
    glColor3f(  1.0,  0.0,  1.0 );
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();
     
    // Lado verde - ESQUERDA
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  1.0,  0.0 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
     
    // Lado azul - TOPO
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  0.0,  1.0 );
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();
     
    // Lado vermelho - BASE
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  0.0,  0.0 );
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    glRotatef(-anguloDeRotacao, 0, 1, 0);
    glScalef(1/raio, 1/raio, 1/raio);
  }

  void setar(float distSol, float raio2, float velocRotacao, float velocTranslacao, GLuint text){   
    distanciaSol = distSol;
    raio = raio2;
    velRotacao =  velocRotacao;
    velTranslacao = velocTranslacao;
    textura = text;

    periodoRotacao = (ajuste*multiploVelocidade) / velRotacao;
    periodoTranslacao = (ajuste*multiploVelocidade) / velTranslacao;
  }

  void atualizar(){
    anguloDeRotacao += velRotacao;
    anguloDeTranslacao += velTranslacao;
  }

  void desenhar(){
    glBindTexture(GL_TEXTURE_2D, textura);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);

    glRotatef(anguloDeTranslacao, 0, 1, 0); //translacao
    glTranslatef(distanciaSol, 0, 0); //translacao

    glRotatef(anguloDeRotacao, 0, 1, 0); //rotacao
    
    glScalef(raio, raio, raio);
    solidSphere(1, esferaLados, esferaLados);
    
    glRotatef(-anguloDeRotacao, 0, 1, 0);
    glScalef(1/raio, 1/raio, 1/raio);
    
    glDisable(GL_TEXTURE_2D);
  }

  void orbita(){
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glLineWidth(5);
    float a;
    for(int t=0; t<=360; t++){
      a = grausParaRadianos(t);
      glVertex3f(distanciaSol*cos(a), 0, distanciaSol*sin(a));
    }
    glEnd();
  }
};

Planeta sol;
Planeta mercurio;
  Planeta sateliteMercurio;
Planeta venus;
  Planeta sateliteVenus;
Planeta terra;
  Planeta lua;
Planeta marte;
  Planeta sateliteMarte;
Planeta jupiter;
  Planeta sateliteJupiter;
Planeta saturno;
  Planeta sateliteSaturno;
Planeta urano;
  Planeta sateliteUrano;
Planeta netuno;
  Planeta sateliteNetuno;
Planeta plutao;
  Planeta satelitePlutao;

//Métodos

void sons(){
  musicaDeFundo.openFromFile("sounds/background-music.wav");
  musicaDeFundo.setVolume(100);
  musicaDeFundo.setLoop(true);
}

void imprimir(int x, int y, const char *string){
  glPushMatrix ();
    glLoadIdentity ();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix ();
      glLoadIdentity();
      gluOrtho2D (0, 1000, 1000, 0);
      glColor3f (1,1,1);
      glRasterPos2f(x, y);
      int len = (int) strlen(string);
      for (int i = 0; i < len; i++){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
      }
    glPopMatrix ();
    glMatrixMode(GL_MODELVIEW);
  glPopMatrix ();
}

void floatParaString(char * string, int precisao, float valor){
  sprintf(string,"%.0f", valor);
  string[precisao] = '\0';
}

void setarCoordenadasDaCamera(Planeta planeta, Planeta satelite){
  double alpha = grausParaRadianos(planeta.anguloDeTranslacao);
  double beta = grausParaRadianos(satelite.anguloDeTranslacao);
  //Por algum motivo, a velocidade de translação do satélite fica diferente da velocidade da camera.
  //Por isso, tivemos que multiplicar o angulo da camera por um fator de ajuste para que as velocidades fiquem bem próximas.
  double xCamPos = (planeta.distanciaSol*cos(alpha) + (satelite.distanciaSol + zoomNoPlatena * satelite.raio)*cos(ajusteCamera*beta));
  double zCamPos = (planeta.distanciaSol*sin(-alpha) + (satelite.distanciaSol + zoomNoPlatena * satelite.raio)*sin(-ajusteCamera*beta));
  double xCamCenter = planeta.distanciaSol*cos(alpha);
  double zCamCenter = planeta.distanciaSol*sin(-alpha);
  cam[cameraAtual].setar(xCamPos, 0, zCamPos, xCamCenter, 0, zCamCenter, 0, 1, 0);
}

void configuraProjecao() 
{ 
  switch(cameraAtual){
    case 3:
      setarCoordenadasDaCamera(mercurio, sateliteMercurio);
    break;
    case 4:
      setarCoordenadasDaCamera(venus, sateliteVenus);
    break;
    case 5:
      setarCoordenadasDaCamera(terra, lua);
    break;
    case 6:
      setarCoordenadasDaCamera(marte, sateliteMarte);
    break;
    case 7:
      setarCoordenadasDaCamera(jupiter, sateliteJupiter);
    break;
    case 8:
      setarCoordenadasDaCamera(saturno, sateliteSaturno);
    break;
    case 9:
      setarCoordenadasDaCamera(urano, sateliteUrano);
    break;
    case 10:
      setarCoordenadasDaCamera(netuno, sateliteNetuno);
    break;
    case 11:
      setarCoordenadasDaCamera(plutao, satelitePlutao);
    break;
  }
  glLoadIdentity();
  cam[cameraAtual].look();
}

void mostradorTempo(){
  if(cameraAtual <= 2){
    imprimir(20, 50, "Tempo Decorrido:");
    imprimir(20, 75, "Valores Reais:");

    n = sprintf (buffer, "%.0f minuto(s) e %.0f segundo(s)", tempoMinutos, tempoSegundos);
    stringParaImprimir = (char*)buffer;
    imprimir(20, 100, stringParaImprimir);

    imprimir(20, 125, "Valores Represntados:");

    n = sprintf (buffer, "%.0f ano(s) e", tempoAnos);
    stringParaImprimir = (char*)buffer;
    imprimir(20, 150, stringParaImprimir);
    
    n = sprintf (buffer, "%.0f dias(s)", tempoDias);
    stringParaImprimir = (char*)buffer;
    imprimir(20, 175, stringParaImprimir);
  }
}

//Skybox
GLuint carregarTexturaSkybox(const char * nomeDoArquivo, int largura, int altura) {
    GLuint texture;
    unsigned char * dado;
    FILE* arquivo;

    arquivo = fopen( nomeDoArquivo, "rb" );
    if ( arquivo == NULL ) {
      return 0;
    }
    dado = (unsigned char *)malloc( largura * altura * 3 );
    fread( dado, largura * altura * 3, 1, arquivo );
    fclose( arquivo );

    glGenTextures( 1, &texture ); 
    glBindTexture( GL_TEXTURE_2D, texture ); 
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, largura, altura, GL_RGB, GL_UNSIGNED_BYTE, dado );    
    free(dado);
    return texture; 
} 

void iniciaSkybox(){
  skybox[SKY_LEFT] = carregarTexturaSkybox("image/stars.bmp", 1000, 1000);
  skybox[SKY_BACK] = carregarTexturaSkybox("image/stars.bmp", 1000, 1000);
  skybox[SKY_RIGTH] = carregarTexturaSkybox("image/stars.bmp", 1000, 1000);
  skybox[SKY_FRONT] = carregarTexturaSkybox("image/stars.bmp", 1000, 1000);
  skybox[SKY_UP] = carregarTexturaSkybox("image/stars.bmp", 1000, 1000);
  skybox[SKY_DOWN] = carregarTexturaSkybox("image/stars.bmp", 1000, 1000);
}

void deletaSkybox(){
  glDeleteTextures(6, &skybox[0]);
}

void desenharSkybox(){
  float x = 0;
  float y = 0;
  float z = 0;
  float largura  = 5000;
  float altura = 5000;
  float profundidade = 5000;
  x = x - largura  / 2;
  y = y - altura / 2;
  z = z - profundidade / 2;

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  // centraliza skybox
  glBindTexture(GL_TEXTURE_2D, skybox[SKY_BACK]);
  glBegin(GL_QUADS);      
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x + largura, y,  z);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x + largura, y + altura, z); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + altura, z);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,  z);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[SKY_FRONT]);
  glBegin(GL_QUADS);  
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z + profundidade);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + altura, z + profundidade);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x + largura, y + altura, z + profundidade); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x + largura, y,  z + profundidade);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[SKY_DOWN]);
  glBegin(GL_QUADS);      
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y,  z + profundidade);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x + largura, y,  z + profundidade); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x + largura, y,  z);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, skybox[SKY_UP]);
  glBegin(GL_QUADS);      
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x + largura, y + altura, z);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x + largura, y + altura, z + profundidade); 
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + altura, z + profundidade);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + altura, z);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[SKY_LEFT]);
  glBegin(GL_QUADS);      
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + altura, z); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + altura, z + profundidade); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,  z + profundidade);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z);     

  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[SKY_RIGTH]);
  glBegin(GL_QUADS);  
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x + largura, y,  z);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x + largura, y,  z + profundidade);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x + largura, y + altura, z + profundidade); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x + largura, y + altura, z);
  glEnd();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

}

void hud(){   
  glDisable(GL_LIGHTING);
  mostradorTempo();
  switch(cameraAtual){
    case 3:
      imprimir(20, 800, "Mercurio:");
      imprimir(20, 825, "Raio: 2.440 km");
      imprimir(20, 850, "Gravidade: 3,7 m/s^2");
      imprimir(20, 875, "Distância do Sol: 57.910.000 km");
      imprimir(20, 900, "Duração do dia: 58d 15h 30m");
      imprimir(20, 925, "Area da superfície: 74.800.000 km²");
      imprimir(20, 950, "Período orbital: 88 dias");
    break;

    case 4:
      imprimir(20, 800, "Venus:");
      imprimir(20, 825, "Raio: 6.052 km");
      imprimir(20, 850, "Gravidade: 8,87 m/s^2");
      imprimir(20, 875, "Distância do Sol: 108.200.000 km");
      imprimir(20, 900, "Duração do dia: 116d 18h 0m");
      imprimir(20, 925, "Area da superfície: 460.200.000 km²");
      imprimir(20, 950, "Período orbital: 225 dias");
    break;

    case 5:
      imprimir(20, 800, "Terra:");
      imprimir(20, 825, "Raio: 6.371 km");
      imprimir(20, 850, "Gravidade: 9,807 m/s^2");
      imprimir(20, 875, "Distância do Sol: 149.600.000 km");
      imprimir(20, 900, "Duração do dia: 1 dia");
      imprimir(20, 925, "Area da superfície: 510.100.000 km²");
      imprimir(20, 950, "Período orbital: 365 dias");
    break;

    case 6:
      imprimir(20, 800, "Marte:");
      imprimir(20, 825, "Raio: 3.390 km");
      imprimir(20, 850, "Gravidade: 3,711 m/s^2");
      imprimir(20, 875, "Distância do Sol: 227.900.000 km");
      imprimir(20, 900, "Duração do dia: 1d 0h 37m");
      imprimir(20, 925, "Area da superfície: 144.800.000 km²");
      imprimir(20, 950, "Período orbital: 687 dias");
    break;

    case 7:
      imprimir(20, 800, "Jupiter:");
      imprimir(20, 825, "Raio: 69.911 km");
      imprimir(20, 850, "Gravidade: 24,79 m/s^2");
      imprimir(20, 875, "Distância do Sol: 778.500.000 km");
      imprimir(20, 900, "Duração do dia: 0d 9h 56m");
      imprimir(20, 925, "Area da superfície: 6,21796 x 10^10 km²");
      imprimir(20, 950, "Período orbital: 12 anos");
    break;

    case 8:
      imprimir(20, 800, "Saturno:");
      imprimir(20, 825, "Raio: 58.232 km");
      imprimir(20, 850, "Gravidade: 10,44 m/s^2");
      imprimir(20, 875, "Distância do Sol: 1,433 × 10^9 km");
      imprimir(20, 900, "Duração do dia: 0d 10h 42m");
      imprimir(20, 925, "Area da superfície: 4,27 × 10^10 km");
      imprimir(20, 950, "Período orbital: 29 anos");
    break;

    case 9:
      imprimir(20, 800, "Urano:");
      imprimir(20, 825, "Raio: 25.362 km");
      imprimir(20, 850, "Gravidade: 8,87 m/s^2");
      imprimir(20, 875, "Distância do Sol: 2,871 × 10^9 km");
      imprimir(20, 900, "Duração do dia: 0d 17h 14m");
      imprimir(20, 925, "Area da superfície: 8,083 × 10^9 km²");
      imprimir(20, 950, "Período orbital: 84 anos");
    break;

    case 10:
      imprimir(20, 800, "Netuno:");
      imprimir(20, 825, "Raio: 24.622 km");
      imprimir(20, 850, "Gravidade: 11,15 m/s^2");
      imprimir(20, 875, "Distância do Sol: 4,495 × 10^9 km");
      imprimir(20, 900, "Duração do dia: 0d 16h 6m");
      imprimir(20, 925, "Area da superfície:  7,618 × 10^9 km²");
      imprimir(20, 950, "Período orbital: 165 anos");
    break;

    case 11:
      imprimir(20, 800, "Plutao:");
      imprimir(20, 825, "Raio: 1.188 km");
      imprimir(20, 850, "Gravidade: 0,62 m/s^2");
      imprimir(20, 875, "Distância do Sol: ‎5,913 × 10^9 km");
      imprimir(20, 900, "Duração do dia: 6d 9h 17m ");
      imprimir(20, 925, "Area da superfície:  1,665 × 10^7 km²");
      imprimir(20, 950, "Período orbital: 248 anos");
    break;
  }
}

void desenhaCena(void){  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  glClearColor(0,0,0,0);
  glEnable(GL_DEPTH);
  desenharSkybox();
  hud();
  configuraProjecao();
  //  para a luz
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  luzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  luzDifusa);
  glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
  glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);
  //  para o material
  glMaterialfv(GL_FRONT, GL_AMBIENT,   materialAmbiente);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   materialDifuso);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  materialEspecular);
  glMaterialfv(GL_FRONT, GL_SHININESS, brilho);
  // luz ligada/desligada
  if (luzLigada){
    glEnable(GL_LIGHTING);
  }
  // desenhando os planetas
  glMaterialfv(GL_FRONT, GL_EMISSION,  emissaoSol);
  glPushMatrix();
    sol.desenhar();
    glMaterialfv(GL_FRONT, GL_EMISSION,  emissaoPlanetas);
    glPushMatrix();
      mercurio.desenhar();
    glPopMatrix();
    glPushMatrix();
      venus.desenhar();
    glPopMatrix();
    glPushMatrix();
      terra.desenhar();
      glPushMatrix();
        lua.desenhar();
      glPopMatrix();
    glPopMatrix();
    glPushMatrix();
      marte.desenhar();
      glPushMatrix();
        sateliteMarte.desenhaCubo();
      glPopMatrix();
    glPopMatrix();
    glPushMatrix();
      jupiter.desenhar();
    glPopMatrix();
    glPushMatrix();
      saturno.desenhar();
    glPopMatrix();
    glPushMatrix();
      urano.desenhar();
    glPopMatrix();
    glPushMatrix();
      netuno.desenhar();
    glPopMatrix();
    glPushMatrix();
      plutao.desenhar();
    glPopMatrix();
  glPopMatrix();
  glDisable(GL_DEPTH);
  // motrar orbitas
  if(mostrarOrbita){
    mercurio.orbita();
    venus.orbita();
    terra.orbita();
    marte.orbita();
    jupiter.orbita();
    saturno.orbita();
    urano.orbita();
    netuno.orbita();
    plutao.orbita();
  } 

  glutSwapBuffers();
}

void inicializa(void){
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  iniciaSkybox();
  musicaDeFundo.play();

  texturaSol = SOIL_load_OGL_texture(
    "image/sol.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaSol == 0){
    cout << "Problema ao carregar SOL.jpg: " << SOIL_last_result() << endl;
  }

  texturaMercurio = SOIL_load_OGL_texture(
    "image/mercurio.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaMercurio == 0) {
    cout << "Problema ao carregar MERCURIO.jpg: " << SOIL_last_result() << endl;
  }

  texturaVenus = SOIL_load_OGL_texture(
    "image/venus.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaVenus == 0){
    cout << "Problema ao carregar VENUS.jpg: " << SOIL_last_result() << endl;
  }

  texturaTerra = SOIL_load_OGL_texture(
    "image/terra.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaTerra == 0){
    cout << "Problema ao carregar TERRA.jpg: " << SOIL_last_result() << endl;
  }

  texturaLua= SOIL_load_OGL_texture(
    "image/lua.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaLua== 0){
    cout << "Problema ao carregar LUA.jpg: " << SOIL_last_result() << endl;
  }

  texturaMarte = SOIL_load_OGL_texture(
    "image/marte.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaMarte == 0){
    cout << "Problema ao carregar MARTE.jpg: " << SOIL_last_result() << endl;
  }

  texturaJupiter = SOIL_load_OGL_texture(
    "image/jupiter.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaJupiter== 0){
    cout << "Problema ao carregar JUPITER.jpg: " << SOIL_last_result() << endl;
  }

  texturaSaturno = SOIL_load_OGL_texture(
    "image/saturno.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaSaturno == 0){
    cout << "Problema ao carregar SATURNO.jpg: " << SOIL_last_result() << endl;
  }

  texturaUrano = SOIL_load_OGL_texture(
    "image/urano.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaUrano == 0){
    cout << "Problema ao carregar URANO.jpg: " << SOIL_last_result() << endl;
  }

  texturaNetuno = SOIL_load_OGL_texture(
    "image/netuno.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaNetuno == 0){
    cout << "Problema ao carregar NETUNO.jpg: " << SOIL_last_result() << endl;
  }

  texturaPlutao = SOIL_load_OGL_texture(
    "image/plutao.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaPlutao == 0){
    cout << "Problema ao carregar PLUTAO.jpg: " << SOIL_last_result() << endl;
  }

  texturaLua = SOIL_load_OGL_texture(
    "image/lua.jpg",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texturaLua == 0){
    cout << "Problema ao carregar LUA.jpg: " << SOIL_last_result() << endl;
  }

  sol.setar(solSol, raioSol, velRotSol, velTransSol, texturaSol);
  mercurio.setar(solMercurio, raioMercurio, velRotMercurio, velTransMercurio, texturaMercurio);
    sateliteMercurio.setar(distSateliteMercurio, raioSateliteMercurio, 0, 0, semTextura);
  venus.setar(solVenus, raioVenus, velRotVenus, velTransVenus, texturaVenus);
    sateliteVenus.setar(distSateliteVenus, raioSateliteVenus, 0, 0, semTextura);
  terra.setar(solTerra, raioTerra, velRotTerra, velTransTerra, texturaTerra);
    lua.setar(luaTerra, raioLua, velRotLua, velTransLua, texturaLua);
  marte.setar(solMarte, raioMarte, velRotMarte, velTransMarte, texturaMarte);
    sateliteMarte.setar(distSateliteMarte, raioSateliteMarte, velRotSatMarte, velTransSatMarte, semTextura);
  jupiter.setar(solJupiter, raioJupiter, velRotJupiter, velTransJupiter, texturaJupiter);
    sateliteJupiter.setar(distSateliteJupiter, raioSateliteJupiter, 0, 0, semTextura);
  saturno.setar(solSaturno, raioSaturno, velRotSaturno, velTransSaturno, texturaSaturno);
    sateliteSaturno.setar(distSateliteSaturno, raioSateliteSaturno, 0, 0, semTextura);
  urano.setar(solUrano, raioUrano, velRotUrano, velTransUrano, texturaUrano);
    sateliteUrano.setar(distSateliteUrano, raioSateliteUrano, 0, 0, semTextura);
  netuno.setar(solNetuno, raioNetuno, velRotNetuno, velTransNetuno, texturaNetuno);
    sateliteNetuno.setar(distSateliteNetuno, raioSateliteNetuno, 0, 0, semTextura);
  plutao.setar(solPlutao, raioPlutao, velRotPlutao, velTransPlutao, texturaPlutao);
    satelitePlutao.setar(distSatelitePlutao, raioSatelitePlutao, 0, 0, semTextura);

  cam[0].setar(0, 2000, 0, 0, 0, 0, 1, 0, 0);
  cam[1].setar(0, 700, 700, 0, 0, 0, 0, 1, 0);
  cam[2].setar(0, 0, 2000, 0, 0, 0, 0, 1, 0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void redimensiona(int largura, int altura){
  float razaoAspectoJanela = ((float)largura)/altura;
  float razaoAspectoMundo = ((float) LARGURA_DO_MUNDO)/ ALTURA_DO_MUNDO;
  // se a janela está menos larga do que o mundo (16:9)...
  if (razaoAspectoJanela < razaoAspectoMundo) {
    // vamos colocar barras verticais (acima e abaixo)
    float hViewport = largura / razaoAspectoMundo;
    float yViewport = (altura - hViewport)/2;
    glViewport(0, yViewport, largura, hViewport);
  }
  // se a janela está mais larga (achatada) do que o mundo (16:9)...
  else if (razaoAspectoJanela > razaoAspectoMundo) {
    // vamos colocar barras horizontais (esquerda e direita)
    float wViewport = ((float)altura) * razaoAspectoMundo;
    float xViewport = (largura - wViewport)/2;
    glViewport(xViewport, 0, wViewport, altura);
  }else{
    glViewport(0, 0, largura, altura);
  }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, largura/altura, 1, 10000);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void atualiza(int idx){
  sol.atualizar();
  mercurio.atualizar();
  venus.atualizar();
  terra.atualizar();
    lua.atualizar();
  marte.atualizar();
    sateliteMarte.atualizar();
  jupiter.atualizar();
  saturno.atualizar();
  urano.atualizar();
  netuno.atualizar();
  plutao.atualizar();
 
  T1++;
  T2++;
  tempoSegundos= T1/FPS;
  tempoDias = multiploVelocidade*T2/FPS;

  if(tempoSegundos==60){
    tempoSegundos = 0;
    tempoMinutos++;
    T1 = 0;
  }

  if(tempoDias == 365){
    tempoDias = 0;
    tempoAnos++;
    T2 = 0;
  }

  glutPostRedisplay();
  glutTimerFunc(1000/FPS, atualiza, 0); // 25ms, 40 frames por segundo
}

void teclado(unsigned char key, int x, int y){
  switch(key){
    //ESC
    case 27:
      exit(0);
    break;
    break;
    case 'o':
    case 'O':
      mostrarOrbita = !mostrarOrbita;
    break;
  
    case 'c':
    case 'C':
      if(cameraAtual < 2){
        cameraAtual++;
      }else{
        cameraAtual = 0;    
      }
    break;

    case '1':
      zoomNoPlatena = 5;
      cameraAtual = 3;
    break;

    case '2':
      zoomNoPlatena = 5;
      cameraAtual = 4;
    break;

    case '3':
      zoomNoPlatena = 5;
      cameraAtual = 5;
    break;

    case '4':
      zoomNoPlatena = 5;
      cameraAtual = 6;
    break;

    case '5':
      zoomNoPlatena = 5;
      cameraAtual = 7;
    break;

    case '6':
      zoomNoPlatena = 5;
      cameraAtual = 8;
    break;

    case '7':
      zoomNoPlatena = 5;
      cameraAtual = 9;
    break;

    case '8':
      zoomNoPlatena = 5;
      cameraAtual = 10;
    break;
    case '9':
      zoomNoPlatena = 5;
      cameraAtual = 11;
    break;

    case 'n':
    case 'N':
      if(planetaAtual == 0){  
        break;  }
      else{ 
        planetaAtual--;   
      }
    break;

    case 'm':
    case 'M':
      if(planetaAtual < 10){
        planetaAtual++;
      }else{
        planetaAtual = 0;
      }
    break;

    case 'l':
    case 'L':
      luzLigada = !luzLigada;
    break;
  }
}

void specialKeyboardInput(int key, int x, int y){
  switch(key){
    case GLUT_KEY_LEFT:
    if(cameraAtual == 1 || cameraAtual ==2){  
      cam[cameraAtual].centerX -= 25;
    }else if(cameraAtual == 0){
      cam[cameraAtual].centerZ -= 25;
    }
    break;

    case GLUT_KEY_RIGHT:
    if(cameraAtual == 1 || cameraAtual ==2){  
      cam[cameraAtual].centerX += 25;
    }else if(cameraAtual == 0){
      cam[cameraAtual].centerZ += 25;
    }
    break;

    case GLUT_KEY_UP:
    if(cameraAtual == 1 || cameraAtual ==2){  
      cam[cameraAtual].centerY += 25;
    }else if(cameraAtual == 0){
      cam[cameraAtual].centerX += 25;
    }
    zoomNoPlatena -= 0.2;
    break;

    case GLUT_KEY_DOWN:
    if(cameraAtual == 1 || cameraAtual ==2){  
      cam[cameraAtual].centerY -= 25;
    }else if(cameraAtual == 0){
      cam[cameraAtual].centerX -= 25;
    }
    zoomNoPlatena += 0.2;
    break;
  }
  
}

// Rotina principal
int main(int argc, char **argv){
  //  configuração inicial da janela do GLUT
  glutInit(&argc, argv);
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Sistema Solar");
  sons();

  //  callbacks
  glutDisplayFunc(desenhaCena);
  glutReshapeFunc(redimensiona);
  glutKeyboardFunc(teclado);
  glutSpecialFunc(specialKeyboardInput);
  glutTimerFunc(1000/FPS, atualiza, 0);
  inicializa();

  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH);

  // Entra em loop e nunca sai
  glutMainLoop();
  deletaSkybox();
  return 0;
}