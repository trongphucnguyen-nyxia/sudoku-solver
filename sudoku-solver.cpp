//#include <windows.h>
//#pragma comment(lib, "user32.lib") 

#include <GL/glut.h>  // (or others, depending on the system in use)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

#include "Puzzle.h"

int drawMode=1;

int numDraws=0;
int windowW = 800;
int windowH = 800;
int puzzleW = windowW - 100;
int puzzleH = windowH - 100;
static int window_id;
static int menu_id;

bool isSimulating=false;

Puzzle* puzzle;
pair<int,int> lastClicked;
//string puzzleFile = "puzzle1.txt";
//string puzzleFile = "puzzle2.txt";
string puzzleFile = "puzzle3.txt";

void initPuzzle() {
  if( puzzle == NULL ) {
    puzzle = new Puzzle(puzzleFile, puzzleW, puzzleH);
  }
}

void init() {
  glClearColor(1.0, 1.0, 1.0, 0.0); //Set display-window color to white.

  glMatrixMode(GL_PROJECTION);      //Set projection parameters.
  glLoadIdentity();
  //gluOrtho2D(-windowW/2,windowW/2, -windowH/2, windowH/2);
  //gluOrtho2D(0,windowW, 0, windowH);
  gluOrtho2D(0,windowW, 0, windowH);
  initPuzzle();
}


void menu(int num) {
  string fileToLoad="";
  bool fileSpecified = false;
  if(num==-1) {
    //points.clear();
    //edges.clear();
    glutPostRedisplay();
  }
  else if(num==0) {
    glutDestroyWindow(window_id);
    exit(0);
  }
  else if(num==1) {
    puzzleFile = "puzzle1.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
  else if(num==2) {
    puzzleFile = "puzzle2.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
  else if( num == 3 ) {
    puzzleFile = "puzzle3.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
  else if( num == 4 ) {
    puzzleFile = "puzzle4.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
  else if( num == 5 ) {
    puzzleFile = "puzzle5.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
  else if( num == 6 ) {
    puzzleFile = "puzzle6.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
  else if( num == 7 ) {
    puzzleFile = "puzzle7.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
  else if( num == 8 ) {
    puzzleFile = "puzzle8.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
  else if( num == 9 ) {
    puzzleFile = "puzzle9.txt";
    delete puzzle;
    puzzle = NULL;
    initPuzzle();
  }
}

void createMenu() {
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Clear ", -1);
	glutAddMenuEntry("Quit",0);
	glutAddMenuEntry("puzzle1",1);
	glutAddMenuEntry("puzzle2",2);
	glutAddMenuEntry("puzzle3",3);
	glutAddMenuEntry("puzzle4",4);
	glutAddMenuEntry("puzzle5",5);
	glutAddMenuEntry("puzzle6",6);
	glutAddMenuEntry("puzzle7",7);
	glutAddMenuEntry("puzzle8",8);
	glutAddMenuEntry("puzzle9",9);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int homeX, homeY;
bool mousePressed=false;
int timeSincePress=0;
int changeY=0;
int changeX=0;
int buttonYmin, buttonYmax;
double alpha = 0.5;
int numButtons = 5;
string buttonStrs[] = { "Search", "Inf1", "Inf2", "Inf3", "Undo" };
double buttonWidth;

void customDraw();

void update() {
  customDraw();
}

void customDraw() {
  if( 0 && ++numDraws % 100 == 0 ) {
    cout << " customDraw numDraws = " << numDraws << endl;
  }


  glClearColor(1, 1, 1, 0.0); //Set display-window color to white.
  glClear(GL_COLOR_BUFFER_BIT);     //Clear display window.
 
  glColor3f(0,0,0); 
  //void renderBitmapString(int type, float x, float y, const char *string);
  renderBitmapString(1, 10, windowH-15, puzzleFile.c_str());  
  string confStr = to_string(puzzle->NumConflicts());
  renderBitmapString(1, 10, windowH-30, confStr.c_str());  
  buttonWidth = windowW / numButtons;
  double ymin = puzzleH+5;
  double ymax = windowH-35;
  buttonYmin = ymin;
  buttonYmax = ymax;
  double xmax = windowW-3;
  double xmin = 3;
  glBegin(GL_LINES); 
  glVertex2d(xmin,ymin);//bot line
  glVertex2d(xmax,ymin);
  glVertex2d(xmin,ymax);//top line
  glVertex2d(xmax,ymax);
  glEnd();
  for(int i=0; i<=5; i++) {
    glBegin(GL_LINES);
    double x = i * buttonWidth;
    if( i==0 ) x+=3;
    if( i==5 ) x-=3;
    glVertex2d(x,ymin);
    glVertex2d(x,ymax);
    glEnd();
    //button str
    if( i<5 )
      renderBitmapString(1, x+4, (ymin+ymax)/2, buttonStrs[i].c_str());  
  }

  puzzle->Draw();

  glFlush();                        //Process all OpenGL routines as quickly as possible.
}

void mousebutton(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    double tx = (1.0*x/windowW)*(windowW);
    double ty = ((1.0*windowH-y)/windowH)*(windowH);
    lastClicked = make_pair(-1,-1);
    lastClicked = puzzle->CoordsToCell(tx, ty);

    //see if clicked in button
    cout << "mouse button x=" << x << " y=" << y << " buttonYmin=" << buttonYmin << " buttonYmax=" << buttonYmax << endl;
    if( ty <= buttonYmax && ty>=buttonYmin ) {
      cout << "clicked in button area. " << endl;
      int x_ind = int(x / buttonWidth);
      if(x_ind>=0 && x_ind < 5 ) {
	cout << "clicked on button: " << buttonStrs[x_ind] << endl;
	if(x_ind==0) puzzle->Search();
	else if(x_ind==1) puzzle->Inf1();
	else if(x_ind==2) puzzle->Inf2();
	else if(x_ind==3) puzzle->Inf3();
	else if(x_ind==4) puzzle->UndoMove();
      }
      else {
	cout << "unknown click." << endl;
      }
    }
    /*
    if( !mousePressed ) {
      homeX = x;
      homeY = y;
      timeSincePress = 0;
    }
    double tx = -windowW/2 + (1.0*x/windowW)*(windowW);
    double ty = -windowH/2 + ((1.0*windowH-y)/windowH)*(windowH);
    if( addIndividual )
      gSim.AddMember(-1,tx,ty);
    else if( addGroup )
      for(int i=0; i<5; i++)
	gSim.AddMember(-1,tx,ty);
    else if( addAttractionPt ) {
      gEnv->AddAttractionPoint(tx,ty);
    }
    mousePressed = true;
    glutPostRedisplay();
    */
  }

  /*
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    cout << " release button " << endl;
    theta = theta+timeSincePress*speedTheta;
    speedTheta = 1;
    changeY = 0;
    //alpha = 1;
    mousePressed = false;
    timeSincePress = 0;
  }
  if( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN ) {
    cout << "glut_middle_button == down" << endl;
  }
  */

}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'q':
  case 27:             // ESCAPE key
	  exit (0);
	  break;
  case '1':
	puzzle->SetCellToValue(lastClicked, 1); break;
  case '2':
	puzzle->SetCellToValue(lastClicked, 2); break;
  case '3':
	puzzle->SetCellToValue(lastClicked, 3); break;
  case '4':
	puzzle->SetCellToValue(lastClicked, 4); break;
  case '5':
	puzzle->SetCellToValue(lastClicked, 5); break;
  case '6':
	puzzle->SetCellToValue(lastClicked, 6); break;
  case '7':
	puzzle->SetCellToValue(lastClicked, 7); break;
  case '8':
	puzzle->SetCellToValue(lastClicked, 8); break;
  case '9':
	puzzle->SetCellToValue(lastClicked, 9); break;
  /*
  case 'b':
  	gSim.ToggleControlledAgent(); break;
  case '1':
	drawMode = 1; break;
  case '2':
	drawMode = 2; break;
  case '3':
	drawMode = 3; break;
  case 9: //tab key
  	gSim.IncrementControllingAgent(); break;
	*/

  }
}

/*
void otherKeyInput(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_UP:
      cout << "GLUT_KEY_UP" << endl;
      gSim.SendControl("forward");
      break;	
    case GLUT_KEY_DOWN:
      //do something here
      cout << "GLUT_KEY_DOWN" << endl;
      gSim.SendControl("back");
      break;
    case GLUT_KEY_LEFT:
      //do something here
      cout << "GLUT_KEY_LEFT" << endl;
      gSim.SendControl("left");
      break;
    case GLUT_KEY_RIGHT:
      //do something here
      cout << "GLUT_KEY_RIGHT" << endl;
      gSim.SendControl("right");
      break;
  }
  glutPostRedisplay();
}
*/

int main(int argc, char** argv) {
	glutInit(&argc, argv);                         //Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   //Set display mode.
	//glutInitWindowPosition(50, 100);               //Set top-left display-window position.
	glutInitWindowSize(windowW, windowH);                  //Set display-window width and height.
	window_id = glutCreateWindow("Sudoku Solver");  //Create display window.

	init();                                        //Execute initialization procedure.
	//string fileToLoad = "ConvexConcaveInputs/obj6.txt";
	///LoadPointXYONLYsFromFile(fileToLoad);


	createMenu();
	//glutIdleFunc(drawPointXYONLYs);
	glutIdleFunc(update);
	glutDisplayFunc(customDraw);                   //Send graphics to display window.
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);                    //How to handle mouse press events. 
	//glutMotionFunc(mouseMove);
	//glutSpecialFunc(otherKeyInput);
	glutMainLoop();                                //Display everything and wait.
}

