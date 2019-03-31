#define LARGURA_DO_MUNDO 1000
#define ALTURA_DO_MUNDO 1000
#define radianoParaGraus(radianos) (radianos * (180.000000 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.000000)

//  Alguns parâmetros do modelo de iluminação: FONTE DE LUZ
const GLfloat luzAmbiente[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat luzDifusa[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat luzEspecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat luzPosicao[] = { 0, 0, 0, 1.0f };
bool luzLigada = true;
static bool localViewer = true;

//  Alguns parâmetros do modelo de iluminação: MATERIAL
const GLfloat materialAmbiente[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat materialDifuso[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat materialEspecular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat brilho[] = { 10.0f };

const GLfloat emissaoSol[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat emissaoPlanetas[]   = { 0, 0, 0, 1.0f };

//  Quantas subdivisões latitudinais/longitudinais da esfera
static int esferaLados = 200;

//  Para a orbita e impressoes na tela
bool mostrarOrbita = false;
int planetaAtual = 0;
static char stringBuffer[10];
static long fonte = (long)GLUT_BITMAP_8_BY_13;

//  Para o ZOOM
bool ZOOM = false;
const int zoomSize = 200;
const int zoomDistance = 10;

//ajuste da terceira camera
const float ajusteCamera = 1.015;

//  Tratando 1 dia da vida real = 1 segundo na representacao
//  ou seja, a terra roda 360 graus em 1 segundo
const int FPS = 40;
const float ajuste = 360/FPS;

//Tempo
int T1;
int T2;
float tempoSegundos;
float tempoMinutos;
float tempoDias;
float tempoAnos;

//Texturas
GLuint texturaSol;
GLuint texturaMercurio;
GLuint texturaVenus;
GLuint texturaTerra;
  GLuint texturaLua;
GLuint texturaMarte;
GLuint texturaJupiter;
GLuint texturaSaturno;
GLuint texturaUrano;
GLuint texturaNetuno;
GLuint texturaPlutao;
GLuint semTextura;
GLuint texturaIntro;

// Planetas
const int distanciaPlanetas = 35;
const int multiploVelocidade = 10;

int calculaDistancias(int distanciaAnterior, int raioPlanetaAnterior, int raioPlanetaAtual){
  return distanciaAnterior + raioPlanetaAnterior + distanciaPlanetas + raioPlanetaAtual;
}

const float raioSol = 300;
const float solSol = 0;
const float velRotSol = (multiploVelocidade * ajuste) / 24.47;
const float velTransSol = 0 * multiploVelocidade;

const float raioMercurio = 10;
const float solMercurio = calculaDistancias(0, raioSol, raioMercurio);
const float velRotMercurio = (multiploVelocidade * ajuste) / 58.7;
const float velTransMercurio = (multiploVelocidade * ajuste) / 89.76; 

const float raioVenus = 25;
const float solVenus = calculaDistancias(solMercurio, raioMercurio, raioVenus);
const float velRotVenus = (multiploVelocidade * ajuste) / 243;
const float velTransVenus = (multiploVelocidade * ajuste) / 224.68;

const float raioTerra = 25;
const float solTerra = calculaDistancias(solVenus, raioVenus, raioTerra);
const float velRotTerra = (multiploVelocidade * ajuste) / 1;
const float velTransTerra = (multiploVelocidade * ajuste) / 365.26;    //0.024657534 * multiploVelocidade;

const float raioMarte = 15;
const float solMarte = calculaDistancias(solTerra, raioTerra, raioMarte);
const float velRotMarte = (multiploVelocidade * ajuste) / 1026;
const float velTransMarte = (multiploVelocidade * ajuste) / 686.98;

const float raioJupiter = 70;
const float solJupiter = calculaDistancias(solMarte, raioMarte, raioJupiter);
const float velRotJupiter = (multiploVelocidade * ajuste) / 0.41;
const float velTransJupiter = (multiploVelocidade * ajuste) / 4316.49; 

const float raioSaturno = 65;
const float solSaturno = calculaDistancias(solJupiter, raioJupiter, raioSaturno);
const float velRotSaturno = (multiploVelocidade * ajuste) / 0.425;
const float velTransSaturno = (multiploVelocidade * ajuste) / 10751.44;

const float raioUrano = 45;
const float solUrano = calculaDistancias(solSaturno, raioSaturno, raioUrano);
const float velRotUrano = (multiploVelocidade * ajuste) / 0.746;
const float velTransUrano = (multiploVelocidade * ajuste) / 30685.55;

const float raioNetuno = 40;
const float solNetuno = calculaDistancias(solUrano, raioUrano, raioNetuno);
const float velRotNetuno = (multiploVelocidade * ajuste) / 0.796;
const float velTransNetuno = (multiploVelocidade * ajuste) / 60155.65;

const float raioPlutao = 5;
const float solPlutao = calculaDistancias(solNetuno, raioNetuno, raioPlutao);
const float velRotPlutao = (multiploVelocidade * ajuste) / 6.39;
const float velTransPlutao = (multiploVelocidade * ajuste) / 90410.5;

// Satelites
float zoomNoPlatena = 5;

const float raioSateliteMercurio = raioMercurio / 7;
const float distSateliteMercurio = 2 * raioMercurio;

const float raioSateliteVenus = raioVenus / 7;
const float distSateliteVenus = 2 * raioVenus;

const float raioLua = raioTerra / 7;
const float luaTerra = 2 * raioTerra;
const float velRotLua = (multiploVelocidade * ajuste) / 27.3333;
const float velTransLua = (multiploVelocidade * ajuste) / 27.3333;

const float raioSateliteMarte = raioMarte / 7;
const float distSateliteMarte = 2 * raioMarte;
const float velRotSatMarte = (multiploVelocidade * ajuste) / 15.3333;
const float velTransSatMarte = (multiploVelocidade * ajuste) / 10.3333;

const float raioSateliteJupiter = raioJupiter / 7;
const float distSateliteJupiter = 2 * raioJupiter;

const float raioSateliteSaturno = raioSaturno / 7;
const float distSateliteSaturno = 2 * raioSaturno;

const float raioSateliteUrano = raioUrano / 7;
const float distSateliteUrano = 2 * raioUrano;

const float raioSateliteNetuno = raioNetuno / 7;
const float distSateliteNetuno = 2 * raioNetuno;

const float raioSatelitePlutao = raioPlutao / 7;
const float distSatelitePlutao = 2 * raioPlutao;

//variasveis para impressao de texto
char buffer [50];
int n;
char *stringParaImprimir;

// Skybox
enum { SKY_LEFT=0, SKY_BACK, SKY_RIGTH, SKY_FRONT, SKY_UP, SKY_DOWN };
unsigned int skybox[6];

//Sons
sf::Music musicaDeFundo;

//Métodos
void imprimir(int x, int y, const char *string);
void floatParaString(char * string, int precisao, float valor);
void sons();
void configuraProjecao();
void hud();
GLuint carregarTexturaSkybox(const char * nomeDoArquivo, int largura, int altura);
void iniciaSkybox();
void deletaSkybox();
void desenharSkybox();
void desenhaCena(void);
void inicializa(void);
void redimensiona(int largura, int altura);
void atualiza(int idx);
void teclado(unsigned char key, int x, int y);
void specialKeyboardInput(int key, int x, int y);