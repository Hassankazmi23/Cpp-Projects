//
//  Concave-Convex Illusion.cpp
//  Project1 CS368
//
//  Created by Daisy Ramos on 10/7/14.
//  Copyright (c) 2014 Daisy Ramos. All rights reserved.
//



#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>


static const int NUM_CIRCLES		= 6;
static const float CIRCLE_RADIUS	= 28.0f;
static float circle_x [NUM_CIRCLES]; // Will hold the circle centers x coordinate
static float circle_y [NUM_CIRCLES]; // Will hold the circle centers y coordinate


// Generates a random number between 0.0 and 1.0
float generate_random ()
{
    return (float)rand () / RAND_MAX;
}


// Verifies if the circle "n" is too close to any other circle i from 0 to n-1
bool overlap (int n)
{
    for (int i = 0; i < n; ++i)
    {
        float dx = circle_x [n] - circle_x [i];
        float dy = circle_y [n] - circle_y [i];
        float distance = sqrt (dx * dx + dy * dy); // distance between the centers
        
        if (distance < 2.5 * CIRCLE_RADIUS)
            return true;
    }
    
    return false;
}

// Generates a random position for the circle "n". If the position is too close
// to a previously generated position it generates another one until it finds
// a good fitting position.
void generate_circle_pos (int n)
{
    float total_x = 400.0f - 3.0f * CIRCLE_RADIUS;
    float total_y = 300.0f - 3.0f * CIRCLE_RADIUS;
    
    do {
        circle_x [n] = (generate_random () - 0.5f) * total_x;
        circle_y [n] = (generate_random () - 0.5f) * total_y;
    } while (overlap (n));
}


void init_circle_positions ()
{
    for (int i = 0; i < NUM_CIRCLES; ++i)
    {
        generate_circle_pos (i);
    }
}


// Draws the n-th circle
void draw_circle (int n)
{
    float xc = circle_x [n]; // circle center x coordinate
    float yc = circle_y [n]; // circle center y coordinate
    float r = CIRCLE_RADIUS;
    float pc, delta;
    
    for (int i = -r; i <= r; ++i)
    {
        if ((n % 2) == 0) // Is "n" an even number?
            pc = 1.0 - (i + r) / (2 * r); // line color decreasing from white 1.0 to black 0.0
        else
            pc = (i + r) / (2 * r); // line color increasing from black 0.0 to white 1.0
        
        delta = sqrt ((float)r * r - (float)i * i);
        glBegin(GL_LINES);
        glColor3f(pc, pc, pc);
        glVertex2f(xc - delta, yc + i);
        glVertex2f(xc + delta, yc + i);
        glEnd();
    }
}

void display ()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < NUM_CIRCLES; ++i)
        draw_circle (i);
    
    glFlush ();
}

int main (int argc, char ** argv)
{
    srand (time (NULL));
    init_circle_positions (); // Generates random positions for the circles
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Circles");			// "Circles" will be the window title
    glClearColor(0.5, 0.5, 0.5, 0.0);		// Defines the background color: mid gray
    gluOrtho2D(-200.0, 200.0, -150, 150);	// Defines the area projected on the window
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
