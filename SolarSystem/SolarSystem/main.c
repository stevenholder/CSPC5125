#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#ifndef M_PI
#define M_PI 3.14159265
#endif


struct planet_struct {
	GLfloat size;
	GLfloat translation;
	GLfloat translationSpeed;
	GLfloat rotation;
	GLfloat rotationSpeed;
	GLfloat tilt;
	GLfloat distance;
	GLfloat inclination;
	struct list_element * satellites;
	GLfloat position[3];
	int material;
	int color;
};

typedef struct planet_struct planet;

struct list_element {
	planet * thePlanet;
	struct list_element *next;
};

typedef struct list_element linkedElement;

static linkedElement *planetList = 0;

GLfloat colors[8][4] = {{0.0, 0.0, 0.0, 1.0},{0.0, 0.0, 1.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 1.0, 1.0}, {1.0, 0.0, 0.0, 1.0},{1.0, 0.0, 1.0, 1.0},{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0}};
GLfloat materials[24][4][4] = {{{0.0215, 0.1745, 0.0215, 1.0}, {0.07568, 0.61424, 0.07568, 1.0}, {0.633, 0.727811, 0.633, 1.0}, {0.6, 0.6, 0.6, 0.6}},
	{{0.135, 0.2225, 0.1575, 1.0}, {0.54, 0.89, 0.63, 1.0}, {0.316228, 0.316228, 0.316228, 1.0}, {0.1, 0.1, 0.1, 0.1}},
	{{0.05375, 0.05, 0.06625, 1.0}, {0.18275, 0.17, 0.22525, 1.0}, {0.332741, 0.328634, 0.346435, 1.0}, {0.3, 0.3, 0.3, 0.3}},
	{{0.25, 0.20725, 0.20725, 1.0}, {1, 0.829, 0.829, 1.0}, {0.296648, 0.296648, 0.296648, 1.0}, {0.088, 0.088, 0.088, 0.088}},
	{{0.1745, 0.01175, 0.01175, 1.0}, {0.61424, 0.04136, 0.04136, 1.0},{0.727811, 0.626959, 0.626959, 1.0}, {0.6, 0.6, 0.6, 0.6}},
	{{0.1, 0.18725, 0.1745, 1.0}, {0.396, 0.74151, 0.69102, 1.0}, {0.297254, 0.30829, 0.306678, 1.0}, {0.1, 0.1, 0.1, 0.1}},
	{{0.329412, 0.223529, 0.027451, 1.0}, {0.780392, 0.568627, 0.113725, 1.0}, {0.992157, 0.941176, 0.807843, 1.0}, {0.21794872, 0.21794872, 0.21794872, 0.21794872}},
	{{0.2125, 0.1275, 0.054, 1.0}, {0.714, 0.4284, 0.18144, 1.}, {0.393548, 0.271906, 0.166721, 1.0}, {0.2, 0.2, 0.2, 0.2}},
	{{0.25, 0.25, 0.25, 1.0}, {0.4, 0.4, 0.4, 1.0}, {0.774597, 0.774597, 0.774597, 1.0}, {0.6, 0.6, 0.6, 0.6}},
	{{0.19125, 0.0735, 0.0225, 1.0}, {0.7038, 0.27048, 0.0828, 1.0}, {0.256777, 0.137622, 0.086014, 1.0}, {0.1, 0.1, 0.1, 0.1}},
	{{0.24725, 0.1995, 0.0745, 1.0}, {0.75164, 0.60648, 0.22648, 1.0}, {0.628281, 0.555802, 0.366065, 1.0}, {0.4, 0.4, 0.4, 0.4}},
	{{0.19225, 0.19225, 0.19225, 1.0}, {0.50754, 0.50754, 0.50754, 1.0}, {0.508273, 0.508273, 0.508273, 1.0}, {0.4, 0.4, 0.4, 0.4}},
	{{0.0, 0.0, 0.0, 1.0}, {0.01, 0.01, 0.01, 1.0}, {0.50, 0.50, 0.50, 1.0}, {0.25, 0.25, 0.25, 0.25}},
	{{0.0, 0.1, 0.06, 1.0}, {0.0, 0.50980392, 0.50980392, 1.0}, {0.50196078, 0.50196078, 0.50196078, 1.0}, {0.25, 0.25, 0.25, 0.25}},
	{{0.0, 0.0, 0.0, 1.0}, {0.1, 0.35, 0.1, 1.0}, {0.45, 0.55, 0.45, 1.0}, {0.25, 0.25, 0.25, 0.25}},
	{{0.0, 0.0, 0.0, 1.0}, {0.5, 0.0, 0.0, 1.0}, {0.7, 0.6, 0.6, 1.0}, {0.25, 0.25, 0.25, 0.25}},
	{{0.0, 0.0, 0.0, 1.0}, {0.55, 0.55, 0.55, 1.0}, {0.70, 0.70, 0.70, 1.0}, {0.25, 0.25, 0.25, 0.25}},
	{{0.0, 0.0, 0.0, 1.0}, {0.5, 0.5, 0.0, 1.0}, {0.60, 0.60, 0.50, 1.0}, {0.25, 0.25, 0.25, 0.25}},
	{{0.02, 0.02, 0.02, 1.0}, {0.01, 0.01, 0.01, 1.0}, {0.4, 0.4, 0.4, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
	{{0.0, 0.05, 0.05, 1.0}, {0.4, 0.5, 0.5, 1.0}, {0.04, 0.7, 0.7, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
	{{0.0, 0.05, 0.0, 1.0}, {0.4, 0.5, 0.4, 1.0}, {0.04, 0.7, 0.04, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
	{{0.05, 0.0, 0.0, 1.0}, {0.5, 0.4, 0.4, 1.0}, {0.7, 0.04, 0.04, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
	{{0.05, 0.05, 0.05, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.7, 0.7, 0.7, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}},
	{{0.05, 0.05, 0.0, 1.0}, {0.5, 0.5, 0.4, 1.0}, {0.7, 0.7, 0.04, 1.0}, {0.078125, 0.078125, 0.078125, 0.078125}}};
/**
 *  First column:  emerald, jade, obsidian, pearl, ruby, turquoise
 *  2nd column:  brass, bronze, chrome, copper, gold, silver
 *  3rd column:  black, cyan, green, red, white, yellow plastic
 *  4th column:  black, cyan, green, red, white, yellow rubber
 */

#define CSU_EMERALD 0
#define CSU_JADE 1
#define CSU_OBSIDIAN 2
#define CSU_PEARL 3
#define CSU_RUBY 4
#define CSU_TURQUOISE 5
#define CSU_BRASS 6
#define CSU_BRONZE 7
#define CSU_CHROME 8
#define CSU_COPPER 9
#define CSU_GOLD 10
#define CSU_SILVER 11
#define CSU_BLACK_PLASTIC 12
#define CSU_CYAN_PLASTIC 13
#define CSU_GREEN_PLASTIC 14
#define CSU_RED_PLASTIC 15
#define CSU_WHITE_PLASTIC 16
#define CSU_YELLOW_PLASTIC 17
#define CSU_BLACK_RUBBER 18
#define CSU_CYAN_RUBBER 19
#define CSU_GREEN_RUBBER 20
#define CSU_RED_RUBBER 21
#define CSU_WHITE_RUBBER 22
#define CSU_YELLOW_RUBBER 23

#define CSU_BLACK 0
#define CSU_BLUE 1
#define CSU_GREEN 2
#define CSU_CYAN 3
#define CSU_RED 4
#define CSU_PURPLE 5
#define CSU_YELLOW 6
#define CSU_WHITE 7


#define CSU_LEFTVIEW 0
#define CSU_FRONTVIEW 3
#define CSU_TOPVIEW 2
#define CSU_PERSPECTIVEVIEW 1


static GLfloat spin = 0.0;
static GLint ww = 500;
static GLint wh = 500;
static GLint savedww, savedwh;
static GLfloat rotationX = 0.0;
static GLfloat rotationY = 0.0;
static GLfloat rotationZ = 0.0;
static GLint transparency = 0;
static GLint lightingEnabled = 1;
static GLint wireframe = 1;
static GLenum mode = GL_SMOOTH;
static GLenum polygonMode = GL_FILL;
static GLint fullscreen = 0;
static GLdouble left, right, bottom, top;
static GLdouble near = -5.0, far = 5.0;
static GLdouble zoom = 10;
static GLint lastX = 0, lastY = 0;
static int mouseButton, currentModifiers;
static int activeView = CSU_LEFTVIEW;
static GLdouble theta = 45.0, phi = 45.0;
static GLfloat eyeX, eyeY, eyeZ;
static GLdouble radius;
static int animate = 0;

// Sun's data
static GLfloat sunSize = 2.0;
static GLfloat sunRotation = 0.0;
static GLfloat sunRotationSpeed = 0.1 * 10;
static GLfloat sunTilt = 7.25;
// Mercury's data
static GLfloat mercurySize = 0.38251;
static GLfloat mercuryTranslation = 0.0;
static GLfloat mercuryTranslationSpeed = 4.152019/10;
static GLfloat mercuryRotation = 0.0;
static GLfloat mercuryRotationSpeed = 0.23310 * 10;
static GLfloat mercuryTilt = 0.01;
static GLfloat mercuryDistance = 4.0;
static GLfloat mercuryInclination = 7.00487;
// Venus' data
static GLfloat venusSize = 0.94885;
static GLfloat venusTranslation = 40.0;
static GLfloat venusTranslationSpeed = 1.6254949/10;
static GLfloat venusRotation = 0.0;
static GLfloat venusRotationSpeed = -1.0 * 10;
static GLfloat venusTilt = 177.36;
static GLfloat venusDistance = 7.0;
static GLfloat venusInclination = 3.39471;
// Earth's data
static GLfloat earthSize = 1.0;
static GLfloat earthTranslation = 80.0;
static GLfloat earthTranslationSpeed = 0.99998260/10;
static GLfloat earthRotation = 0.0;
static GLfloat earthRotationSpeed = 0.41038 * 10;//0.0041038 * 10;
static GLfloat earthTilt = 23.45;
static GLfloat earthDistance = 11.0;
static GLfloat earthInclination = 0.00005;
// Moon's Data
static GLfloat moonSize = 0.27287;
static GLfloat moonTranslation = 80.0;
static GLfloat moonTranslationSpeed = 13.368/10;
static GLfloat moonRotation = 0.0;
static GLfloat moonRotationSpeed = 0.00013013 * 10;//0.0000013013 * 10;
static GLfloat moonTilt = 0.0;
static GLfloat moonDistance = 1.5;
static GLfloat moonInclination = 5.16;
// Mars' Data
static GLfloat marsSize = 0.5318;
static GLfloat marsTranslation = 120.0;
static GLfloat marsTranslationSpeed = 0.53167519/10;
static GLfloat marsRotation = 0.0;
static GLfloat marsRotationSpeed = 0.42219 * 10;//0.0042219 * 10;
static GLfloat marsTilt = 25.19;
static GLfloat marsDistance = 14.0;
static GLfloat marsInclination = 1.85061;
// Phobos' Data
static GLfloat phobosSize = 0.0174;
static GLfloat phobosTranslation = 0.0;
static GLfloat phobosTranslationSpeed = 11.40/10;// Divided by 100 also
static GLfloat phobosRotation = 0.0;
static GLfloat phobosRotationSpeed = 0.000000152 * 10;//0.00000000152 * 10;
static GLfloat phobosTilt = 0.046;
static GLfloat phobosDistance = 1.5;
static GLfloat phobosInclination = 1.075;
// Deimos' Data
static GLfloat deimosSize = 0.0097;
static GLfloat deimosTranslation = 180.0;
static GLfloat deimosTranslationSpeed = 2.894/10;// Divided by 10 also
static GLfloat deimosRotation = 0.0;
static GLfloat deimosRotationSpeed = 0.0000006011 * 10;//0.000000006011 * 10;
static GLfloat deimosTilt = 0.897;
static GLfloat deimosDistance = 2.7633;
static GLfloat deimosInclination = 1.793;
//Jupiter's Data
static GLfloat jupiterSize = 10.863;
static GLfloat jupiterTranslation = 160.0;
static GLfloat jupiterTranslationSpeed = 0.084298445/10;
static GLfloat jupiterRotation = 0.0;
static GLfloat jupiterRotationSpeed = 0.17017 * 10;//0.0017017 * 10;
static GLfloat jupiterTilt = 3.13;
static GLfloat jupiterDistance = 28.0;
static GLfloat jupiterInclination = 1.30530;
// Io's Data 
static GLfloat ioSize = 0.28608;
static GLfloat ioTranslation = 90.0;
static GLfloat ioTranslationSpeed = 206.5/10;
static GLfloat ioRotation = 0.0;
static GLfloat ioRotationSpeed = 0.0000008425 * 10;//0.000000008425 * 10;
static GLfloat ioTilt = 0.0;
static GLfloat ioDistance = 3.049;
static GLfloat ioInclination = 0.036;
// Europa's Data
static GLfloat europaSize = 0.24512;
static GLfloat europaTranslation = 180.0;
static GLfloat europaTranslationSpeed = 102.9/10;
static GLfloat europaRotation = 0.0;
static GLfloat europaRotationSpeed = 0.000001691 * 10;//0.00000001691 * 10;
static GLfloat europaTilt = 0.016;
static GLfloat europaDistance = 4.851;
static GLfloat europaInclination = 0.466;
// Ganymede's Data
static GLfloat ganymedeSize = 0.41323;
static GLfloat ganymedeTranslation = 270.0;
static GLfloat ganymedeTranslationSpeed = 51.05/10;
static GLfloat ganymedeRotation = 0.0;
static GLfloat ganymedeRotationSpeed = 0.000003408 * 10;//0.00000003408 * 10;
static GLfloat ganymedeTilt = 0.068;
static GLfloat ganymedeDistance = 7.737;
static GLfloat ganymedeInclination = 0.177;
// Callisto's Data
static GLfloat callistoSize = 0.37853;
static GLfloat callistoTranslation = 0.0;
static GLfloat callistoTranslationSpeed = 21.88/10;
static GLfloat callistoRotation = 0.0;
static GLfloat callistoRotationSpeed = 0.000007949 * 10;//0.00000007949 * 10;
static GLfloat callistoTilt = 0.356;
static GLfloat callistoDistance = 13.609;
static GLfloat callistoInclination = 0.192;
//Saturn's Data
static GLfloat saturnSize = 9.001;
static GLfloat saturnTranslation = 0.0;
static GLfloat saturnTranslationSpeed = 0.033958742/10;
static GLfloat saturnRotation = 0.0;
static GLfloat saturnRotationSpeed = 0.1800 * 10;//0.001800 * 10;
static GLfloat saturnTilt = 26.73;
static GLfloat saturnDistance = 52.0;
static GLfloat saturnInclination = 2.48446;
// Titan's Data  
static GLfloat titanSize = 0.40448;
static GLfloat titanTranslation = 90.0;
static GLfloat titanTranslationSpeed = 22.90/10;
static GLfloat titanRotation = 0.0;
static GLfloat titanRotationSpeed = 0.000007597 * 10;//0.00000007597 * 10;
static GLfloat titanTilt = 0.6;
static GLfloat titanDistance = 10.659;
static GLfloat titanInclination = 0.28;
// Iapetus's Data
static GLfloat iapetusSize = 0.1154;
static GLfloat iapetusTranslation = 180.0;
static GLfloat iapetusTranslationSpeed = 4.604/10;
static GLfloat iapetusRotation = 0.0;
static GLfloat iapetusRotationSpeed = 0.00003778 * 10;//0.0000003778 * 10;
static GLfloat iapetusTilt = 14.968;
static GLfloat iapetusDistance = 31.063;
static GLfloat iapetusInclination = 7.489;
// Rhea's Data
static GLfloat rheaSize = 0.1201;
static GLfloat rheaTranslation = 270.0;
static GLfloat rheaTranslationSpeed = 80.84/10;
static GLfloat rheaRotation = 0.0;
static GLfloat rheaRotationSpeed = 0.000002152 * 10;//0.00000002152 * 10;
static GLfloat rheaTilt = 0.029;
static GLfloat rheaDistance = 4.5979;
static GLfloat rheaInclination = 0.331;
//Uranus' Data
static GLfloat uranusSize = 3.9680;
static GLfloat uranusTranslation = 240.0;
static GLfloat uranusTranslationSpeed = 0.011902375/10;
static GLfloat uranusRotation = 0.0;
static GLfloat uranusRotationSpeed = 0.27 * 10;//0.0027 * 10;
static GLfloat uranusTilt = 97.77;
static GLfloat uranusDistance = 65.0;
static GLfloat uranusInclination = 0.76986;
//Neptune' Data
static GLfloat neptuneSize = 3.8559;
static GLfloat neptuneTranslation = 280.0;
static GLfloat neptuneTranslationSpeed = 0.0060682808/10;
static GLfloat neptuneRotation = 0.0;
static GLfloat neptuneRotationSpeed = 0.316 * 10;//0.00316 * 10;
static GLfloat neptuneTilt = 28.32;
static GLfloat neptuneDistance = 75.0;//77.677723
static GLfloat neptuneInclination = 1.76917;
//Pluto' Data
static GLfloat plutoSize = 0.1842;
static GLfloat plutoTranslation = 320.0;
static GLfloat plutoTranslationSpeed = 0.0040335486/10;
static GLfloat plutoRotation = 0.0;
static GLfloat plutoRotationSpeed = 2.6282 * 10;//0.026282 * 10;
static GLfloat plutoTilt = 122.53;
static GLfloat plutoDistance = 80.0;//101.99379
static GLfloat plutoInclination = 17.14175;



int mSet; /* starting point of set of materials */
int cSet; /* stating point of set of colors */
int persAngle = 60;


void appendElement(linkedElement * theList, planet * newPlanet)
{
	if(theList->next)
	{
		appendElement(theList->next, newPlanet);
	}
	else
	{
		theList->next = (linkedElement *)malloc(sizeof(linkedElement));
		theList->next->next = 0;
		theList->next->thePlanet = newPlanet;
	}
}

planet * createPlanet(GLfloat size)
{
	planet * newPlanet;
	newPlanet = (planet *)malloc(sizeof(planet));
	newPlanet->size = size;
	newPlanet->satellites = 0;
	return newPlanet;
}

planet * getPlanet(linkedElement * theList, int number)
{
	if(number == 0)
	{
		return theList->thePlanet;
	}
	else
	{
		if(theList->next)
		{
			return getPlanet(theList->next, number - 1);
		}
		else
		{
			return 0;
		}
	}
}

/*
 * Get planet's position
 */
void getPosition(planet *aPlanet, GLfloat position[])
{
	position[0] = aPlanet->position[0];
	position[1] = aPlanet->position[1];
	position[2] = aPlanet->position[2];
}

/** You may use the predefined constants above */
void setMaterial(int matNum)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, materials[matNum][0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[matNum][1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materials[matNum][2]);
    glMaterialf(GL_FRONT, GL_SHININESS, materials[matNum][3][0] * 128.0);
}

void setActiveViewport(int x, int y)
{
	lastX = x;
	lastY = y;
	if(x > ww/2)
	{
		if(y > wh/2)
		{
			activeView = CSU_PERSPECTIVEVIEW;
		}
		else
		{
			activeView = CSU_FRONTVIEW;
		}
	}
	else
	{
		if(y > wh/2)
		{
			activeView = CSU_LEFTVIEW;
		}
		else
		{
			activeView = CSU_TOPVIEW;
		}
	}
}

void movePlanet(planet *thePlanet)
{
	thePlanet->translation += thePlanet->translationSpeed;
	if(thePlanet->translation >= 360.0)
	{
		thePlanet->translation = thePlanet->translation - 360.0;
	}
	thePlanet->rotation += thePlanet->rotationSpeed;
	if(thePlanet->rotation >= 360.0)
	{
		thePlanet->rotation = thePlanet->rotation - 360.0;
	}
	
}

void movePlanets(linkedElement * theList)
{
	linkedElement * pList = theList;
	while(pList)
	{
		movePlanet(pList->thePlanet);
		pList = pList->next;
	}
}

void spinDisplay(void)
{
	movePlanets(planetList);
	glutPostRedisplay();
}

void animateSystem(int value)
{
	if(animate)
	{
		spinDisplay();
		glutTimerFunc(33, animateSystem, -1);
	}
}

void adjustOrtho()
{
	if (ww <= wh)
	{
		left = -zoom;
		right = zoom;
		bottom = (-zoom*(GLfloat)wh/(GLfloat)ww);
		top = (zoom*(GLfloat)wh/(GLfloat)ww);
		near = -zoom;
		far = 2*zoom;
	}
	else
	{
		left = (-zoom*(GLfloat)ww/(GLfloat)wh);
		right = (zoom*(GLfloat)ww/(GLfloat)wh);
		bottom = -zoom;
		top = zoom;
		near = -2*zoom;
		far = 2*zoom;
	}
	radius = sqrt(pow(zoom, 2) + pow(zoom, 2) + pow(zoom, 2));
	eyeX = radius * cos(theta/180.0 * M_PI) * sin(phi/180.0 * M_PI);
	eyeY = radius * sin(theta/180.0 * M_PI) * sin(phi/180.0 * M_PI);
	eyeZ = radius * cos(theta/180.0 * M_PI);
}

void reshape(int w, int h)
{
	ww = w;
	wh = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	adjustOrtho();
	glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
	GLfloat tempPos[3];
	
	mouseButton = button;
	setActiveViewport(x, y);
	currentModifiers = glutGetModifiers();
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{
				/*
				getPosition(getPlanet(planetList,3), tempPos);
				printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
				eyeX = tempPos[0];
				eyeY = tempPos[1];
				eyeZ = tempPos[2];
				glutPostRedisplay();\
				*/
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
				break;
			break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN)
				break;
			break;
		default:
			break;
	}
}

void drawPlanetList(linkedElement *theList);


void drawPlanet(planet *thePlanet)
{
	GLfloat m[16];
	
	glRotatef(thePlanet->translation, 0.0, 1.0, 0.0);
	glTranslatef(thePlanet->distance, 0.0, 0.0);
	glRotatef(thePlanet->tilt, 1.0, 0.0, 0.0);
	glRotatef(thePlanet->rotation, 0.0, 1.0, 0.0);
	glPushMatrix();
	if(thePlanet->satellites)
	{
		drawPlanetList(thePlanet->satellites);
	}
	glPopMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	setMaterial(thePlanet->material);
	glColor4fv(colors[thePlanet->color]);
	glutSolidSphere(thePlanet->size, 12, 12);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	thePlanet->position[0] = m[12];
	thePlanet->position[1] = m[13];
	thePlanet->position[2] = m[14];
}

void drawPlanetList(linkedElement *theList)
{
	linkedElement * pList = theList;
	while(pList)
	{
		glPushMatrix();
		drawPlanet(pList->thePlanet);
		glPopMatrix();
		pList = pList->next;
	}
}

void drawSolarSystem()
{
	if(lightingEnabled == 0)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);
	
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	
	glShadeModel(mode);
	glPushMatrix();
	
	drawPlanetList(planetList);
	glPopMatrix();
}

void buildSolarSystem()
{
	planet * tempPlanet, * tempSatellite;
	
	planetList = (linkedElement *)malloc(sizeof(linkedElement));
	planetList->next = 0;
	tempPlanet = createPlanet(sunSize);
	tempPlanet->rotation = sunRotation;
	tempPlanet->rotationSpeed = sunRotationSpeed;
	tempPlanet->translation = 0.0;
	tempPlanet->translationSpeed = 0.0;
	tempPlanet->tilt = sunTilt;
	tempPlanet->distance = 0.0;
	tempPlanet->material = CSU_GOLD;
	tempPlanet->color = CSU_YELLOW;
	planetList->thePlanet = tempPlanet;
	
	tempPlanet = createPlanet(mercurySize);
	tempPlanet->rotation = mercuryRotation;
	tempPlanet->rotationSpeed = mercuryRotationSpeed;
	tempPlanet->translation = mercuryTranslation;
	tempPlanet->translationSpeed = mercuryTranslationSpeed;
	tempPlanet->distance = mercuryDistance;
	tempPlanet->inclination = mercuryInclination;
	tempPlanet->material = CSU_BRONZE;
	tempPlanet->color = CSU_RED;
	tempPlanet->tilt = mercuryTilt;
	
	appendElement(planetList, tempPlanet);
	
	tempPlanet = createPlanet(venusSize);
	tempPlanet->rotation = venusRotation;
	tempPlanet->rotationSpeed = venusRotationSpeed;
	tempPlanet->translation = venusTranslation;
	tempPlanet->translationSpeed = venusTranslationSpeed;
	tempPlanet->distance = venusDistance;
	tempPlanet->inclination = venusInclination;
	tempPlanet->material = CSU_PEARL;
	tempPlanet->color = CSU_WHITE;
	tempPlanet->tilt = venusTilt;
	
	appendElement(planetList, tempPlanet);
	
	tempPlanet = createPlanet(earthSize);
	tempPlanet->rotation = earthRotation;
	tempPlanet->rotationSpeed = earthRotationSpeed;
	tempPlanet->translation = earthTranslation;
	tempPlanet->translationSpeed = earthTranslationSpeed;
	tempPlanet->distance = earthDistance;
	tempPlanet->inclination = earthInclination;
	tempPlanet->material = CSU_TURQUOISE;
	tempPlanet->color = CSU_CYAN;
	tempPlanet->tilt = earthTilt;
	
	tempSatellite = createPlanet(moonSize);
	tempSatellite->rotation = moonRotation;
	tempSatellite->rotationSpeed = moonRotationSpeed;
	tempSatellite->translation = moonTranslation;
	tempSatellite->translationSpeed = moonTranslationSpeed;
	tempSatellite->distance = moonDistance;
	tempSatellite->inclination = moonInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = moonTilt;
	
	// The next line creates the starting point of a linked list for the satellites of the current planet
	// and sets the first satellite
	// If you were to add another satellite you would use AppendElement(tempPlanet->satellites, tempSatellite)
	tempPlanet->satellites = (linkedElement *)malloc(sizeof(linkedElement));
	tempPlanet->satellites->thePlanet = tempSatellite;
	tempPlanet->satellites->next = 0;
	
	appendElement(planetList, tempPlanet);
	
	//Start
	//Mars
	
	tempPlanet = createPlanet(marsSize);
	tempPlanet->rotation = marsRotation;
	tempPlanet->rotationSpeed = marsRotationSpeed;
	tempPlanet->translation = marsTranslation;
	tempPlanet->translationSpeed = marsTranslationSpeed;
	tempPlanet->distance = marsDistance;
	tempPlanet->inclination = marsInclination;
	tempPlanet->material = CSU_RUBY;
	tempPlanet->color = CSU_RED;
	tempPlanet->tilt = marsTilt;
	
	//Phobos Deimos
	
	tempSatellite = createPlanet(phobosSize);
	tempSatellite->rotation = phobosRotation;
	tempSatellite->rotationSpeed = phobosRotationSpeed;
	tempSatellite->translation = phobosTranslation;
	tempSatellite->translationSpeed = phobosTranslationSpeed;
	tempSatellite->distance = phobosDistance;
	tempSatellite->inclination = phobosInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = phobosTilt;
	
	tempPlanet->satellites = (linkedElement *)malloc(sizeof(linkedElement));
	tempPlanet->satellites->thePlanet = tempSatellite;
	tempPlanet->satellites->next = 0;
	
	tempSatellite = createPlanet(deimosSize);
	tempSatellite->rotation = deimosRotation;
	tempSatellite->rotationSpeed = deimosRotationSpeed;
	tempSatellite->translation = deimosTranslation;
	tempSatellite->translationSpeed = deimosTranslationSpeed;
	tempSatellite->distance = deimosDistance;
	tempSatellite->inclination = deimosInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = deimosTilt;
	
	appendElement(tempPlanet->satellites, tempSatellite);
	
	appendElement(planetList, tempPlanet);
	
	//Asteroid Belt
	
	//Jupiter
	
	tempPlanet = createPlanet(jupiterSize);
	tempPlanet->rotation = jupiterRotation;
	tempPlanet->rotationSpeed = jupiterRotationSpeed;
	tempPlanet->translation = jupiterTranslation;
	tempPlanet->translationSpeed = jupiterTranslationSpeed;
	tempPlanet->distance = jupiterDistance;
	tempPlanet->inclination = jupiterInclination;
	tempPlanet->material = CSU_BRONZE;
	tempPlanet->color = CSU_WHITE;
	tempPlanet->tilt = jupiterTilt;
	
	//io europa ganymede callisto
	tempSatellite = createPlanet(ioSize);
	tempSatellite->rotation = ioRotation;
	tempSatellite->rotationSpeed = ioRotationSpeed;
	tempSatellite->translation = ioTranslation;
	tempSatellite->translationSpeed = ioTranslationSpeed;
	tempSatellite->distance = ioDistance;
	tempSatellite->inclination = ioInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = ioTilt;
	
	tempPlanet->satellites = (linkedElement *)malloc(sizeof(linkedElement));
	tempPlanet->satellites->thePlanet = tempSatellite;
	tempPlanet->satellites->next = 0;
	
	tempSatellite = createPlanet(europaSize);
	tempSatellite->rotation = europaRotation;
	tempSatellite->rotationSpeed = europaRotationSpeed;
	tempSatellite->translation = europaTranslation;
	tempSatellite->translationSpeed = europaTranslationSpeed;
	tempSatellite->distance = europaDistance;
	tempSatellite->inclination = europaInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = europaTilt;
	
	appendElement(tempPlanet->satellites, tempSatellite);
	
	tempSatellite = createPlanet(ganymedeSize);
	tempSatellite->rotation = ganymedeRotation;
	tempSatellite->rotationSpeed = ganymedeRotationSpeed;
	tempSatellite->translation = ganymedeTranslation;
	tempSatellite->translationSpeed = ganymedeTranslationSpeed;
	tempSatellite->distance = ganymedeDistance;
	tempSatellite->inclination = ganymedeInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = ganymedeTilt;
	
	appendElement(tempPlanet->satellites, tempSatellite);
	
	tempSatellite = createPlanet(callistoSize);
	tempSatellite->rotation = callistoRotation;
	tempSatellite->rotationSpeed = callistoRotationSpeed;
	tempSatellite->translation = callistoTranslation;
	tempSatellite->translationSpeed = callistoTranslationSpeed;
	tempSatellite->distance = callistoDistance;
	tempSatellite->inclination = callistoInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = callistoTilt;
	
	appendElement(tempPlanet->satellites, tempSatellite);
	
	appendElement(planetList, tempPlanet);
	
	//Saturn
	
	tempPlanet = createPlanet(saturnSize);
	tempPlanet->rotation = saturnRotation;
	tempPlanet->rotationSpeed = saturnRotationSpeed;
	tempPlanet->translation = saturnTranslation;
	tempPlanet->translationSpeed = saturnTranslationSpeed;
	tempPlanet->distance = saturnDistance;
	tempPlanet->inclination = saturnInclination;
	tempPlanet->material = CSU_BRASS;
	tempPlanet->color = CSU_WHITE;
	tempPlanet->tilt = saturnTilt;
	
	//titan iapetus rhea
	
	tempSatellite = createPlanet(titanSize);
	tempSatellite->rotation = titanRotation;
	tempSatellite->rotationSpeed = titanRotationSpeed;
	tempSatellite->translation = titanTranslation;
	tempSatellite->translationSpeed = titanTranslationSpeed;
	tempSatellite->distance = titanDistance;
	tempSatellite->inclination = titanInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = titanTilt;
	
	tempPlanet->satellites = (linkedElement *)malloc(sizeof(linkedElement));
	tempPlanet->satellites->thePlanet = tempSatellite;
	tempPlanet->satellites->next = 0;
	
	tempSatellite = createPlanet(iapetusSize);
	tempSatellite->rotation = iapetusRotation;
	tempSatellite->rotationSpeed = iapetusRotationSpeed;
	tempSatellite->translation = iapetusTranslation;
	tempSatellite->translationSpeed = iapetusTranslationSpeed;
	tempSatellite->distance = iapetusDistance;
	tempSatellite->inclination = iapetusInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = iapetusTilt;
	
	appendElement(tempPlanet->satellites, tempSatellite);
	
	tempSatellite = createPlanet(rheaSize);
	tempSatellite->rotation = rheaRotation;
	tempSatellite->rotationSpeed = rheaRotationSpeed;
	tempSatellite->translation = rheaTranslation;
	tempSatellite->translationSpeed = rheaTranslationSpeed;
	tempSatellite->distance = rheaDistance;
	tempSatellite->inclination = rheaInclination;
	tempSatellite->material = CSU_PEARL;
	tempSatellite->color = CSU_WHITE;
	tempSatellite->tilt = rheaTilt;
	
	appendElement(tempPlanet->satellites, tempSatellite);
	
	appendElement(planetList, tempPlanet);
	
	//Uranus
	
	tempPlanet = createPlanet(uranusSize);
	tempPlanet->rotation = uranusRotation;
	tempPlanet->rotationSpeed = uranusRotationSpeed;
	tempPlanet->translation = uranusTranslation;
	tempPlanet->translationSpeed = uranusTranslationSpeed;
	tempPlanet->distance = uranusDistance;
	tempPlanet->inclination = uranusInclination;
	tempPlanet->material = CSU_TURQUOISE;
	tempPlanet->color = CSU_WHITE;
	tempPlanet->tilt = uranusTilt;
	
	appendElement(planetList, tempPlanet);
	
	//Neptune
	
	tempPlanet = createPlanet(neptuneSize);
	tempPlanet->rotation = neptuneRotation;
	tempPlanet->rotationSpeed = neptuneRotationSpeed;
	tempPlanet->translation = neptuneTranslation;
	tempPlanet->translationSpeed = neptuneTranslationSpeed;
	tempPlanet->distance = neptuneDistance;
	tempPlanet->inclination = neptuneInclination;
	tempPlanet->material = CSU_EMERALD;
	tempPlanet->color = CSU_WHITE;
	tempPlanet->tilt = neptuneTilt;
	
	appendElement(planetList, tempPlanet);
	
	//Pluto
	
	tempPlanet = createPlanet(plutoSize);
	tempPlanet->rotation = plutoRotation;
	tempPlanet->rotationSpeed = plutoRotationSpeed;
	tempPlanet->translation = plutoTranslation;
	tempPlanet->translationSpeed = plutoTranslationSpeed;
	tempPlanet->distance = plutoDistance;
	tempPlanet->inclination = plutoInclination;
	tempPlanet->material = CSU_CYAN_RUBBER;
	tempPlanet->color = CSU_WHITE;
	tempPlanet->tilt = plutoTilt;
	
	appendElement(planetList, tempPlanet);

	
}

void mouseZoomRotate(int x, int y)
{
	if(mouseButton == GLUT_LEFT_BUTTON)
	{
		if(currentModifiers == GLUT_ACTIVE_SHIFT)
		{
			switch(activeView)
			{
				case CSU_LEFTVIEW:
					break;
				case CSU_TOPVIEW:
					break;
				case CSU_FRONTVIEW:
					break;
				case CSU_PERSPECTIVEVIEW:
					theta -= 75*(GLfloat)(lastY - y)/(GLfloat)wh;
					phi -= 50*(GLfloat)(lastX - x)/(GLfloat)ww;
					if(theta > 360.0)
						theta -= 360.0;
					else if(theta < 0.0)
						theta += 360.0;
					if(phi > 360.0)
						phi -= 360.0;
					else if(phi < 0.0)
						phi += 360.0;
					lastY = y;
					lastX = x;
					eyeX = radius * cos(theta/180.0 * M_PI) * sin(phi/180.0 * M_PI);
					eyeY = radius * sin(theta/180.0 * M_PI) * sin(phi/180.0 * M_PI);
					eyeZ = radius * cos(phi/180.0 * M_PI);
					//printf("(%3f, %3f, %3f)\n",eyeX, eyeY, eyeZ);
					break;
				default:
					break;
			}
		}
		else
		{
			if(lastY - y > 0)
			{
				zoom += 0.1;
				lastY = y;
			}
			else
			{
				zoom -= 0.1;
				lastY = y;
			}
			adjustOrtho();
		}
		glutPostRedisplay();
	}
}


void keyHandler(unsigned char key, int x, int y)
{
	int i = 0, j = 0;
	switch(key)
	{
		case 'A':
		case 'a':
			if(animate)
			{
				animate = 0;
			}
			else
			{
				glutTimerFunc(33, animateSystem, -1);
				animate = 1;
			}
			break;
		case 'T':
		case 't':
			if(transparency == 0)
			{
				for(i = 0; i < 8; i++)
				{
					colors[i][3] = 0.5;
				}
				for(i = 0; i < 24; i++)
				{
					for(j = 0; j < 3; j++)
					{
						materials[i][j][3] = 0.5;
					}
				}
				transparency = 1;
			}
			else
			{
				for(i = 0; i < 8; i++)
				{
					colors[i][3] = 1.0;
				}
				for(i = 0; i < 24; i++)
				{
					for(j = 0; j < 3; j++)
					{
						materials[i][j][3] = 1.0;
					}
				}
				transparency = 0;
			}
			break;
		case 'L':
		case 'l':
			if(lightingEnabled == 0)
			{
				lightingEnabled = 1;
			}
			else
			{
				lightingEnabled = 0;
			}
			break;
		case 'W':
		case 'w':
			if(wireframe == 0)
			{
				wireframe = 1;
				polygonMode = GL_FILL;
			}
			else
			{
				wireframe = 0;
				polygonMode = GL_LINE;
			}
			break;
		case 'S':
		case 's':
			if(mode == GL_SMOOTH)
				mode = GL_FLAT;
			else
				mode = GL_SMOOTH;
			break;
		case 'F':
		case 'f':
			if(fullscreen == 0)
			{
				savedww = ww;
				savedwh = wh;
				glutFullScreen();
				fullscreen = 1;
			}
			else
			{
				glutPositionWindow(10, 10);
				glutReshapeWindow(savedww, savedwh);
				fullscreen = 0;
			}
			break;
		case '-':
		case '_':
			zoom += 0.5;
			adjustOrtho();
			break;
		case '+':
		case '=':
			zoom -= 0.5;
			adjustOrtho();
			break;
		case 'Q':
		case 'q':
			exit(0);
			break;
	}
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	// Left Viewport (lower-left)
	glViewport(0,0,ww/2, wh/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near, far);
	gluLookAt(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawSolarSystem();
	// Front viewport (upper-right)
	glViewport(ww/2,wh/2,ww/2,wh/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near, far);
	gluLookAt(0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawSolarSystem();
	// Top viewport (upper-left)
	glViewport(0,wh/2,ww/2,wh/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near, far);
	gluLookAt(0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawSolarSystem();
	// Perspective viewport (lower-right)
	glViewport(ww/2, 0, ww/2,wh/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(persAngle, (GLdouble)ww/(GLdouble)wh, 0.5, 1000);
	gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawSolarSystem();
	glPopMatrix();
	glutSwapBuffers();
}

void myinit(void)
{
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_position1[] = { 0.0, 0.0, 0.0, 1.0 };
	
	/* Change the color of the light */
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	
	glShadeModel(GL_SMOOTH);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	
	/* Your specular light could be different */
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	
	/* Even though you set stuff, you need to enable these features */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_AUTO_NORMAL);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	
	buildSolarSystem();
}

void mymenu(int id)
{
	GLfloat tempPos[3];
	switch( id )
	{
		case 2:
			exit(0);
			break;
		case 1:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glutSwapBuffers();
			break;
		case 11: //Sun
			getPosition(getPlanet(planetList,0), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 12: //Mercury
			getPosition(getPlanet(planetList,1), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 13: //Venus
			getPosition(getPlanet(planetList,2), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 14: //Earth
			getPosition(getPlanet(planetList,3), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 15: //Mars
			getPosition(getPlanet(planetList,4), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 16: //Jupiter
			getPosition(getPlanet(planetList,5), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 17: //Saturn
			getPosition(getPlanet(planetList,6), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 18: //Uranus
			getPosition(getPlanet(planetList,7), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 19: //Neptune
			getPosition(getPlanet(planetList,8), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;
		case 20: //Pluto
			getPosition(getPlanet(planetList,9), tempPos);
			printf("This planet is at (%f, %f, %f).\n", tempPos[0], tempPos[1], tempPos[2]);
			eyeX = tempPos[0];
			eyeY = tempPos[1];
			eyeZ = tempPos[2];
			glutPostRedisplay();
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	int menu_id, planet_menu;
	mSet = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Solar System");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseZoomRotate);
	glutKeyboardFunc(keyHandler);
	glutReshapeFunc(reshape);
	glutTimerFunc(33, animateSystem, -1);
	myinit();
	
	planet_menu = glutCreateMenu(mymenu);
	glutAddMenuEntry("Sun", 11);
	glutAddMenuEntry("Mercury", 12);
	glutAddMenuEntry("Venus", 13);
	glutAddMenuEntry("Earth", 14);
	glutAddMenuEntry("Mars", 15);
	glutAddMenuEntry("Jupiter", 16);
	glutAddMenuEntry("Saturn", 17);
	glutAddMenuEntry("Uranus", 18);
	glutAddMenuEntry("Neptune", 19);
	glutAddMenuEntry("Pluto", 20);
	
	menu_id = glutCreateMenu(mymenu);
	glutAddMenuEntry("clear Screen", 1);
	glutAddSubMenu("view planet", planet_menu);
	glutAddMenuEntry("exit", 2);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glutMainLoop();
}

