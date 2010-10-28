/* This program draws a series of polygons
 * stored in a linked list.
 * There are two types of linked lists: One that holds the polygons
 * and the other that holds the vertices for the polygons.
 * Each polygon has a linked list for its list of vertices.
 */

#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#define WIDTH 0.05
#define SIZE 512

int ww, wh;
int colormenu, menuid;
GLfloat *handleptr = 0;
GLfloat zoom = 3.0;
GLfloat worldX, worldY;
int polygonValue = 0;


GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat aqua[3] = {0.0, 1.0, 1.0};
GLfloat fuchsia[3] = {1.0, 0.0, 1.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat * color = white;


/*********************************************************************************************
 * Here starts the code for handling linked lists of vertices and polygons and it could      *
 * be placed in a separate file if necessary.                                                *
 *********************************************************************************************/
struct vertex_element {
	GLfloat coords[2];
	struct vertex_element *next;
};

typedef struct vertex_element vertex;

struct polygon_element {
	int number;
	GLfloat color[3];
	vertex vertex_list;
	int selected;
	struct polygon_element *next;
};

typedef struct polygon_element polygon;

static polygon *thePolygonList = 0;

static polygon * underConstruction = 0;

static polygon * selectedPolygon = 0;

/*
 * This appends a vertex to a linked list of vertices
 */
void appendVertex(vertex *aVertex, GLfloat x, GLfloat y)
{
	if(aVertex->next != 0)
	{
		appendVertex(aVertex->next, x, y);
	}
	else
	{
		aVertex->next = (vertex *)malloc(sizeof(vertex));
		aVertex->next->coords[0] = x;
		aVertex->next->coords[1] = y;
		aVertex->next->next = 0;
	}
}

void moveVertexes(vertex *aVertex, GLfloat x, GLfloat y)
{
	aVertex->coords[0] += x;
	aVertex->coords[1] += y;
	if(aVertex->next != 0)
	{
		moveVertexes(aVertex->next, x, y);
	}
}

/* 
 * This appends a polygon to a linked list of polygons
 */
void appendPolygon(polygon *aPolygon, polygon *newPolygon)
{
	if(aPolygon->next!= 0)
	{
		appendPolygon(aPolygon->next, newPolygon);
	}
	else
	{
		aPolygon->next = newPolygon;
	}
}

/*
 * Creates a new polygon and returns a pointer to it
 */
polygon * startPolygon(GLfloat x, GLfloat y)
{
	polygon * newPolygon;
	newPolygon = (polygon *)malloc(sizeof(polygon));
	newPolygon->vertex_list.coords[0] = x;
	newPolygon->vertex_list.coords[1] = y;
	newPolygon->vertex_list.next = 0;
	// set the color to white
	newPolygon->color[0] = 1.0;
	newPolygon->color[1] = 1.0;
	newPolygon->color[2] = 1.0;
	newPolygon->selected = 0;
	newPolygon->next = 0;
	newPolygon->number = polygonValue;
	polygonValue++;
	return newPolygon;
}

/* 
 * Appends a vertex to a polygon
 */
void appendVertexToPolygon(polygon *aPolygon, GLfloat x, GLfloat y)
{
	appendVertex(&(aPolygon->vertex_list), x, y);
}

void movePolygon(polygon *aPolygon, GLfloat x, GLfloat y)
{
	moveVertexes(&(aPolygon->vertex_list), x, y);
}

int getListSize() {
	polygon * pList = thePolygonList;
	int count = 0;
	while(pList)
	{
		count++;
		pList = pList->next;
	}
	return count;
}

int findPosition(polygon * aPolygon) {
	polygon * pList = thePolygonList;
	int count = 0;
	while(pList)
	{
		if(pList->number == aPolygon->number)
			return count;

		count++;
		pList = pList->next;
	}
	return -1;
}

polygon * removePolygon(polygon **listP, int value)
{
	polygon *currP, *prevP;
	prevP = 0;
	for (currP = *listP; currP != 0; prevP = currP, currP = currP->next) {
		if (currP->number == value) { 
			polygon * temp;
			temp = currP;
			if (prevP == 0) {
				*listP = currP->next;
			} else {
				prevP->next = currP->next;
			}
			return temp;
		}
	}
}

polygon * findPolygonAtPosition(int position) {
	polygon * pList = thePolygonList;
	int count = 0;
	while(pList)
	{
		if(count == position) {
			return pList;
		}
		count++;
		pList = pList->next;
	}
}

void moveDown(polygon *aPolygon) {
	int position = findPosition(aPolygon);
	if(position == -1)
		return;

	if(position == 0) {
		return;
	} else {
		polygon * temp;
		polygon * holder;
		holder = removePolygon(&thePolygonList, aPolygon->number);

		holder->next = findPolygonAtPosition(position-1);
		if(position != 1) {
			temp = findPolygonAtPosition(position-2);
			temp->next = holder;
		} else {
			thePolygonList = holder;
		}
	}
}

void moveUp(polygon *aPolygon) {
	int position = findPosition(aPolygon);
	if(position == -1)
		return;

	printf("Position: %i\n", position);

	moveDown(findPolygonAtPosition(position+1));

}

/*
 * Sets the polygon's color
 */
void setPolygonColor(polygon *aPolygon, GLfloat *colors)
{
	aPolygon->color[0] = colors[0];
	aPolygon->color[1] = colors[1];
	aPolygon->color[2] = colors[2];
}

/*
 * Gets a polygon from a polygon list
 */

polygon * getPolygon(polygon *aPolygon, int number)
{
	if(number == 0)
	{
		return aPolygon;
	}
	else
	{
		if(aPolygon->next)
		{
			return getPolygon(aPolygon->next, number -1);
		}
		else
		{
			return 0;
		}
	}
}

/*
 * Gets a vertex from a vertex list
 */

vertex * getVertex(vertex *aVertexList, int vertexNumber)
{
	if(vertexNumber == 0)
	{
		return aVertexList;
	}
	else
	{
		if(aVertexList->next)
		{
			return getVertex(aVertexList->next, vertexNumber - 1);
		}
		else
		{
			return 0;
		}
	}
}

/*
 * Gets a vertex from a polygon
 */
vertex * getVertexFromPolygon(polygon *aPolygon, int vertexNumber)
{
	return getVertex(&(aPolygon->vertex_list), vertexNumber);
}


/*
 * Toggles the selection of a polygon
 */
void toggleSelection(polygon *aPolygon)
{
	if(aPolygon->selected == 0)
	{
		aPolygon->selected = -1;
	}
	else
	{
		aPolygon->selected = 0;
	}
	
	if(selectedPolygon == 0) {
		selectedPolygon = aPolygon;
	} 
}

/*
 * Returns a -1 if any polygon is selected
 */
int anySelected(polygon *aPolygonList)
{
	if(thePolygonList != 0) {
		if(aPolygonList->selected)
		{
			return -1;
		}
		else
		{
			if(aPolygonList->next)
			{
				return anySelected(aPolygonList->next);
			}
			else
			{
				return 0;
			}
		}
	}
}

/*
 * Clears selection
 */
void clearSelection(polygon *aPolygonList)
{
	polygon * pList = aPolygonList;
	while(pList)
	{
		pList->selected = 0;
		pList = pList->next;
	}
	selectedPolygon = 0;
}

/************************************************************************************
 * After here you have drawing code althoug using linked lists.                     *
 ************************************************************************************/

/**
 * Draws a handle at position vertex
 * with a width of WIDTH
 */
void drawHandle(GLfloat *vertex)
{
	glPushMatrix();
	glColor4f(0.5, 0.5, 0.0, 0.8);
	glBegin(GL_POLYGON);
	glVertex2f(vertex[0] - WIDTH, vertex[1] - WIDTH);
	glVertex2f(vertex[0] + WIDTH, vertex[1] - WIDTH);
	glVertex2f(vertex[0] + WIDTH, vertex[1] + WIDTH);
	glVertex2f(vertex[0] - WIDTH, vertex[1] + WIDTH);
	glEnd();
	glPopMatrix();
}

/*
 * Draws a polygon defined by the 
 * list of vertices.
 */
void drawPolygon(polygon *aPolygon)
{
	int vertexNumber = 0;
	
	vertex *vList = &(aPolygon->vertex_list);
	glBegin(GL_POLYGON);
	glColor3fv(aPolygon->color);
	while(vList)
	{
		glVertex2fv(vList->coords);
		vList = vList->next;
	}
	glEnd();
	if(aPolygon->selected != 0)
	{
		vList = &(aPolygon->vertex_list);
		while(vList)
		{
			glPushName(vertexNumber++);
			drawHandle(vList->coords);
			glPopName();
			vList = vList->next;
		}
	}
}

/*
 * Draws the polygon list
 */
void drawPolygonList(polygon *aPolygonList)
{
	polygon *pList = aPolygonList;
	int polygonNumber = 0;
	while(pList)
	{
		glLoadName(polygonNumber++);
		drawPolygon(pList);
		pList = pList->next;
		printf("PNum: %i\n", polygonNumber);
	}
}

void menu(int id) {
	switch(id) {
	case 1:
		moveUp(selectedPolygon);
		break;
	case 2:
		moveDown(selectedPolygon);
		break;
	case 6:
		exit(0);
		break;
	case 10:
		if(selectedPolygon != 0)
			setPolygonColor(selectedPolygon, white);
		color = white;
		break;
	case 11:
		if(selectedPolygon != 0)
			setPolygonColor(selectedPolygon, black);
		color = black;
		break;
	case 12:
		if(selectedPolygon != 0)
			setPolygonColor(selectedPolygon, red);
		color = red;
		break;
	case 13:
		if(selectedPolygon != 0)
			setPolygonColor(selectedPolygon, green);
		color = green;
		break;
	case 14:
		if(selectedPolygon != 0)
			setPolygonColor(selectedPolygon, blue);
		color = blue;
		break;
	case 15:
		if(selectedPolygon != 0)
			setPolygonColor(selectedPolygon, yellow);
		color = yellow;
		break;
	case 16:
		if(selectedPolygon != 0)
			setPolygonColor(selectedPolygon, aqua);
		color = aqua;
		break;
	case 17:
		if(selectedPolygon != 0)
			setPolygonColor(selectedPolygon, fuchsia);
		color = fuchsia;
		break;
	}
	glutPostRedisplay();
}
 

void display(void)
{
	/* clear window */
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* draw all polygons */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (-zoom, zoom, -zoom, zoom);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawPolygonList(thePolygonList);
	
	/* flush GL buffers or swap GL buffers*/
	
	//glFlush();
	glutSwapBuffers();

	colormenu = glutCreateMenu(menu);
	glutAddMenuEntry("White", 10);
	glutAddMenuEntry("Black", 11);
	glutAddMenuEntry("Red", 12);
	glutAddMenuEntry("Green", 13);
	glutAddMenuEntry("Blue", 14);
	glutAddMenuEntry("Yellow", 15);
	glutAddMenuEntry("Aqua", 16);
	glutAddMenuEntry("Fuchsia", 17);

	menuid = glutCreateMenu(menu);
	
	if(selectedPolygon != 0) {
		glutAddMenuEntry("Move Up", 1);
		glutAddMenuEntry("Move Down", 2);
	}
	glutAddSubMenu("Change Color", colormenu);
	glutAddMenuEntry("Exit", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/**
 * This function builds the starting linked list and it is used for demonstration purposes.
 * You should build it interactively in an application.
 */
void buildPolygonList()
{
	int i;
	polygon *tempPolygon;
	
	
	// These arrays is just for convenience but the polygons and vertices could be specified
	// on the fly as you define them.
	GLfloat redRectangle[4][2] = {{-0.5, -0.5}, {1.0, -0.5}, {1.0, 1.0}, {-0.5, 1.0}};
	GLfloat blueRectangle[4][2] = {{-1.0, -1.0}, {0.5, -1.0}, {0.5, 0.5}, {-1.0, 0.5}};
	GLfloat greenRectangle[4][2] = {{-0.5, 1.5}, {1.5, 1.5}, {1.5, 1.75}, {-0.5, 1.75}};
	GLfloat yellowPolygon[7][2] = {{-2.0,-1.5}, {0.0, -1.75}, {0.5, -0.5}, {1.75, 0.0}, {0.5, 1.2}, {-0.5, 0.5}, {-1.3, 1.0}};
	GLfloat red[3] = {1.0, 0.0, 0.0};
	GLfloat green[3] = {0.0, 1.0, 0.0};
	GLfloat blue[3] = {0.0, 0.0, 1.0};
	GLfloat yellow[3] = {1.0, 1.0, 0.0};
	
	// the red rectangle
	thePolygonList = startPolygon(redRectangle[0][0],redRectangle[0][1]);
	for(i = 1; i < 4; i++)
	{
		appendVertexToPolygon(thePolygonList, redRectangle[i][0], redRectangle[i][1]);
	}
	setPolygonColor(thePolygonList, red);
	// The yellow polygon
	tempPolygon = startPolygon(yellowPolygon[0][0], yellowPolygon[0][1]);
	for(i = 1; i < 6; i++)
	{
		appendVertexToPolygon(tempPolygon, yellowPolygon[i][0], yellowPolygon[i][1]);
	}
	setPolygonColor(tempPolygon, yellow);
	appendPolygon(thePolygonList, tempPolygon);
	// the blue rectangle
	tempPolygon = startPolygon(blueRectangle[0][0], blueRectangle[0][1]);
	for(i = 1; i < 4; i++)
	{
		appendVertexToPolygon(tempPolygon, blueRectangle[i][0], blueRectangle[i][1]);
	}
	setPolygonColor(tempPolygon, blue);
	appendPolygon(thePolygonList, tempPolygon);
	// the green rectangle
	tempPolygon = startPolygon(greenRectangle[0][0], greenRectangle[0][1]);
	for(i = 1; i < 4; i++)
	{
		appendVertexToPolygon(tempPolygon, greenRectangle[i][0], greenRectangle[i][1]);
	}
	setPolygonColor(tempPolygon, green);
	appendPolygon(thePolygonList, tempPolygon);
	
}

void init()
{
	/* set clear color to black */
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	/* set fill color to white */
	
	glColor3f(1.0, 1.0, 1.0);
	
	//buildPolygonList();
	
	/* set up standard orthogonal view with clipping */
	/* box as cube of side 2 centered at origin */
	/* this is default view and these statements could be removed */
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-zoom, zoom, -zoom, zoom);
	
}

/**
 * The keyboard function callback
 * The characters used here are assuming that you build the test
 * linked list of polygons and they refer to the color of the 
 * polygons. You should remove them and use your code.
 */
void keyboard(unsigned char key,
			  int x, int y)
{
	polygon * tempPolygon;
	switch (key)
	{
		case 27:
			exit(0);
			break;
		case 'a' : //Being to add a polygon
		case 'A' :
			if(underConstruction == 0) {
				if(polygonValue == 0) {
					thePolygonList = startPolygon(((GLfloat)(2*zoom*x)/(GLfloat)ww-zoom), ((GLfloat)(wh - y)*2*zoom/(GLfloat)wh-zoom));
					selectedPolygon = underConstruction = thePolygonList;
					setPolygonColor(underConstruction, color);
					underConstruction->selected = 1;
				} else {
					selectedPolygon = underConstruction = startPolygon(((GLfloat)(2*zoom*x)/(GLfloat)ww-zoom), ((GLfloat)(wh - y)*2*zoom/(GLfloat)wh-zoom));
					underConstruction->selected = 1;
					setPolygonColor(underConstruction, color);
					appendPolygon(thePolygonList, underConstruction);
				}
			} else {
				underConstruction = 0;
			}
			break;
		case 'R':
		case 'r':
			if(tempPolygon = getPolygon(thePolygonList, 0))
			{
				toggleSelection(tempPolygon);
			}
			break;
		case 'd' : //Delete a polygon
		case 'D' :
			if(selectedPolygon != 0) {
				removePolygon(&thePolygonList, selectedPolygon->number);
				selectedPolygon = 0;
			}
			break;
		case 'G':
		case 'g':
			if(tempPolygon = getPolygon(thePolygonList, 3))
			{
				toggleSelection(tempPolygon);
			}
			break;
		case 'B':
		case 'b':
			if(tempPolygon = getPolygon(thePolygonList, 2))
			{
				toggleSelection(tempPolygon);
			}
			break;
		case 'Y':
		case 'y':
			if(tempPolygon = getPolygon(thePolygonList, 1))
			{
				toggleSelection(tempPolygon);
			}
			break;
		case '+':
		case '=':
			zoom -= 0.5;
			break;
		case '-':
		case '_':
			zoom += 0.5;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

/**
 * This may be used to examine the return 
 * values when using 'GL_SELECT'
 */
void examineHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr;
	printf ("hits = %d\n", hits);
	ptr = (GLuint *) buffer;
	
	for (i = 0; i < (unsigned int)hits; i++)
	{ /*  for each hit  */
		names = *ptr;
		ptr+=3;
		printf("  We got %i names.\n", names);
		for (j = 0; j < names; j++)
		{ /*  for each name */
			printf("\tName %i is %i.\n", j, *ptr);
			ptr++;
		}
		
		printf ("\n");
	}
	
}

/*
 * Selects a handle to edit, if any is found
 * in the hit list
 */
void selectHandle(GLint hits, GLuint buffer[])
{
	unsigned int i;
	GLuint names, *ptr;
	vertex *tempPTR;
	ptr = (GLuint *) buffer;
	
	if(hits == 0)
	{
		handleptr = 0;
		clearSelection(thePolygonList);
	}
	
	for (i = 0; i < (unsigned int)hits; i++)
	{ /*  for each hit  */
		names = *ptr;
		ptr+=3;
		if(names > 1) /* We have a handle if the number of names if more than one. */
		{
			/* The polygon number (position in the polygon linked list) is held in ptr[0]
			 * and the handle number (position in the vertex linked list) is held in ptr[1] 
			 */
			tempPTR = getVertexFromPolygon(getPolygon(thePolygonList,ptr[0]),ptr[1]);
			handleptr = tempPTR->coords;
		}
		ptr += names;
	}
	
}

/*
 * Selects a polygon, if any is found in the hit list
 */
void selectPolygon(GLint hits, GLuint buffer[])
{
	unsigned int i;
	GLuint names, *ptr;
	ptr = (GLuint *) buffer;
	
	for (i = 0; i < (unsigned int)hits; i++)
	{ /*  for each hit  */
		names = *ptr;
		ptr+=3;
		if((names == 1) && (i == (hits-1)))
		{
			toggleSelection(getPolygon(thePolygonList, *ptr));
		}
		ptr += names;
	}
	
}

/**
 * Picks objects
 */
void pickObjects(int x, int y)
{
	GLuint selectBuf[SIZE];
	GLint hits;
	GLint viewport[4];
	glGetIntegerv (GL_VIEWPORT, viewport);
	
	glSelectBuffer (SIZE, selectBuf);
	glRenderMode(GL_SELECT);
	
	glInitNames();
	glPushName(0);
	
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();
	/* create 5x5 pixel picking region near cursor location */
	gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y),
				   5.0, 5.0, viewport);
	gluOrtho2D (-zoom, zoom, -zoom, zoom);
	drawPolygonList(thePolygonList);
	
	
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glFlush ();
	
	hits = glRenderMode (GL_RENDER);
	// You may use the next statement to examine the hit list
	//examineHits (hits, selectBuf);
	if(anySelected(thePolygonList)) /* If there are any polygon selected we want to find a handle, if not we want to find a polygon */
	{
		selectHandle(hits, selectBuf);
	}
	else
	{
		selectPolygon(hits, selectBuf);
		//examineHits (hits, selectBuf);
	}
	
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	worldX = (GLfloat)(2*zoom*x)/(GLfloat)ww-zoom;
	worldY = (GLfloat)(wh - y)*2*zoom/(GLfloat)wh-zoom;
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(underConstruction != 0) {
			appendVertexToPolygon(underConstruction, worldX, worldY);
			printf("Appended %i & %i to Polygon# %i\n", x, y, underConstruction->number);
		} else {
			pickObjects(x, y);
		}
	}
	glutPostRedisplay();
}

/**
 * The reshape function
 */
void reshape(int w, int h)
{
	ww = w;
	wh = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (-zoom, zoom, -zoom, zoom);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/**
 * The mouse move callback to 
 * define the new position of the 
 * selected handle
 */
void mouseMove(int x, int y)
{
	if(handleptr != 0)
	{
		worldX = (GLfloat)(2*zoom*x)/(GLfloat)ww-zoom;
		worldY = (GLfloat)(wh - y)*2*zoom/(GLfloat)wh-zoom;
		//printf("  Screen x = %i, y = %i.\n",x, y);
		//printf("   World x = %f, y = %f\n", worldX, worldY);
		
		handleptr[0] = worldX;
		handleptr[1] = worldY;
	} else {
		if(selectedPolygon != 0) {
			GLfloat dWorldX, dWorldY;
			dWorldX = ((GLfloat)(2*zoom*x)/(GLfloat)ww-zoom) - worldX;
			dWorldY = ((GLfloat)(wh - y)*2*zoom/(GLfloat)wh-zoom) - worldY;
			worldX = (GLfloat)(2*zoom*x)/(GLfloat)ww-zoom;
			worldY = (GLfloat)(wh - y)*2*zoom/(GLfloat)wh-zoom;
			//printf("   World dx = %f, dy = %f\n", dWorldX, dWorldY);
			movePolygon(selectedPolygon,dWorldX,dWorldY);
		}
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	/* initialize mode and open a window in upper-left corner of screen */
	/* window title is name of program (arg[0]) */
	
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Polygon Painter");
	glutKeyboardFunc (keyboard);
	glutReshapeFunc (reshape);
	glutDisplayFunc (display);
	glutMotionFunc (mouseMove);
	glutMouseFunc (mouse);
	init();
	glutMainLoop();
}