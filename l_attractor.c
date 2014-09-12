/*
 *  Coordinates
 *
 *  Display 2, 3 and 4 dimensional coordinates in 3D.
 *
 *  Key bindings:
 *  1      2D coordinates
 *  2      2D coordinates with fixed Z value
 *  3      3D coordinates
 *  4      4D coordinates
 *  +/-    Increase/decrease z or w
 *  arrows Change view angle
 *  0      Reset view angle
 *  ESC    Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th = 0;       // Azimuth of view angle
int ph = 0;       // Elevation of view angle
int mode = 3;     // Dimension (1-4)
double dim = 55;   // Dimension of orthogonal box
char* text[] = {"","2D","3D constant Z","3D","4D"};  // Dimension display text

//  Lorenz Parameters
double s = 10;
double b = 2.6666;
double r = 28;
int i = 0.0;

// Change over time variables
double dx = 0.0;
double dy = 0.0;
double dz = 0.0;
double dt = 0.0;

// Color variables
double cx = 0.0;
double cy = 0.0;
double cz = 0.0;

// Coordinate Parameters
double x = 0.0;     // X variable
double y = 0.0;     // Y variable
double z = 0.0;     // Z variable
double def_z = 0.0; // Z variable default value
double w = 1.0;     // W variable
double def_w = 1.0; // W variable default value

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Display the scene
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   
   //  Reset previous transforms
   glLoadIdentity();
   
   //  Set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   
   //  Draw 10 pixel yellow points
   glColor3f(1,1,0);
   glPointSize(1);
   glBegin(GL_POINTS);
   switch (mode)
   {
   //  Two dimensions
   case 1:
   /*  Coordinates  */
      x = 1.0;
      y = 1.0;

      /*  Time step  */
      dt = 0.001;

      /*
       *  Integrate 50,000 steps (50 time units with dt = 0.001)
       *  Explicit Euler integration
       */
      for (i=0; i<50000; i++)
      {
         dx = s*(y-x);
         dy = x*(r-z)-y;
         x = (double)(x + dt*dx);
         y = (double)(y + dt*dy);
         glColor3f(dx, dy, 0);
         glVertex2d(x, y);
      }
      break;
   //  Three dimensions - constant Z
   case 2:
      /*  Coordinates  */
      x = 1.0;
      y = 1.0;
      z = def_z;

      /*  Time step  */
      dt = 0.001;

      /*
       *  Integrate 50,000 steps (50 time units with dt = 0.001)
       *  Explicit Euler integration
       */
      for (i=0; i<50000; i++)
      {
         dx = s*(y-x);
         dy = x*(r-z)-y;
         x = (double)(x + dt*dx);
         y = (double)(y + dt*dy);
         glColor3f(dx, dy, 0);
         glVertex3d( x, y, z);
      }
      break;
   //  Three dimensions - variable Z
   case 3:
      /*  Coordinates  */
      x = 1.0;
      y = 1.0;
      z = 1.0;

      // Reset Color
      cx = 0.0;
      cy = 0.0;
      cz = 0.0;

      /*  Time step  */
      dt = 0.001;

      /*
       *  Integrate 50,000 steps (50 time units with dt = 0.001)
       *  Explicit Euler integration
       */
      for (i=0; i<50000; i++)
      {
         //Calculate differentials
         dx = s*(y-x);
         dy = x*(r-z)-y;
         dz = x*y - b*z;
         
         //Determine new variables for point
         x = (double)(x + dt*dx);
         y = (double)(y + dt*dy);
         z = (double)(z + dt*dz);
         
         // Set Color and Create Point
         if ( dx < 0.1 && dy < 0.1 && dz < 0.1)
            glColor3f(8.0, 8.0, 8.0);
         else
            glColor3f(dx, dy, dz);
         glVertex3d( x, y, z);
      }
      break;
   //  Four dimensions
   case 4:
      /*  Coordinates  */
      x = 1.0;
      y = 1.0;
      z = 1.0;
      w = def_w;

      /*  Time step  */
      dt = 0.001;

      /*
       *  Integrate 50,000 steps (50 time units with dt = 0.001)
       *  Explicit Euler integration
       */
      for (i=0; i<50000; i++)
      {
         dx = s*(y-x);
         dy = x*(r-z)-y;
         dz = x*y - b*z;
         x = (double)(x + dt*dx);
         y = (double)(y + dt*dy);
         z = (double)(z + dt*dz);
         glColor3f(dx, dy, dz);
         glVertex4d(x, y, z, w);
      }
      break;
   }
   glEnd();

   //  Draw axes in white
   glColor3f(1,1,1);
   glBegin(GL_LINES);
      glVertex3d(0,0,0);
      glVertex3d(50,0,0);
      glVertex3d(0,0,0);
      glVertex3d(0,50,0);
      glVertex3d(0,0,0);
      glVertex3d(0,0,50);
   glEnd();

   //  Label axes
   glRasterPos3d(50,0,0);
   Print("X");
   glRasterPos3d(0,50,0);
   Print("Y");
   glRasterPos3d(0,0,50);
   Print("Z");

   //  Display parameters
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d  %s",th,ph,text[mode]);
   if (mode==2)
      Print("  z=%.1f",z);
   else if (mode==4)
      Print("  w=%.1f",w);

   //  Flush and swap
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   
   //  Switch dimensions
   else if ('1'<=ch && ch<='4')
   {
      mode = ch-'0';
      if (mode==2) z = 0;
      if (mode==4) w = 1;
   }
   
   //  Alter s for equation by +1
   else if (ch == '+')
   {
      s = s + 1;  
   }
   //  Alter s for equation by -1
   else if (ch == '-')
   {
      s = s - 1;
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Coordinates");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
