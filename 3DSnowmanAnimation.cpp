
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <stdlib.h>
#include <math.h>


// ***********************************************
// Keys:
// L            - Turn lighting On/Off
// D            - Turn depth test On/Off
// Up/Down      - Zoom in/out
// Left/Right   - Rotate around the object
// ***********************************************

GLUquadricObj *q_obj;

static const float pi_value = acos (-1.0f);
static const int width = 800;
static const int height = 600;

// Waving parameters
static const int waving_delay = 5; // Waving delay
static const float broom_angle_min  = -10.0f;
static const float broom_angle_max  = 20.0f;
static float broom_angle_inc        = 0.3f; // Angle increment during the waving animation
static float broom_angle            = 0.0f; // Broom initial angle

// Object geometry
static float ball_radius [3]        = {12.0f, 9.0f, 5.5f};
static const float ball_overlap     = 1.5f;  // The snowballs will have some overlap to appear real
static const float ball_scale_y     = 0.85f; // The snowballs will be scaled down on the Y axis (vertically)
static float ball_center_y [3];            // Will be computed based on ball_radius in initialize_objects()
static const float plane [4] [3]    = {{-500, 0, 500}, {-500, 0, -500}, {500, 0, -500}, {500, 0, 500}};

static const float eye_radius       = 0.7f;
static const float button_radius    = 0.7f;
static const float broom_radius     = 0.4f;
static const float straw_radius_1   = 0.2f;
static const float straw_radius_2   = 0.1f;
static const int num_straws			= 6;
// The straws will have irregular lengths, defined here
static const float straw_lengths [num_straws] = {1.0f, 1.15f, 1.18f, 1.1f, 1.05f, 0.95f};

// Light
static const float light0_position [4] = {10.0f, 25.0f, -140.0f, 0.0f};

// Colors
static const float ambient_color [3]        = {0.9f, 0.9f, 0.8f};
static const float snow_color [4]           = {0.9f, 0.9f, 0.9f, 1};
static const float snow_emission_color [4]  = {0.3f, 0.3f, 0.3f, 0.8f};
static const float coal_color [4]           = {0.1f, 0.1f, 0.1f, 1};
static const float button_color [4]         = {0.3f, 0.3f, 0.3f, 1};
static const float nose_color [4]           = {0.8f, 0.4f, 0.0f, 1};
static const float broom_color [4]          = {0.32f, 0.27f, 0.2f, 1};
static const float plane_color [4]          = {0.5f, 0.5f, 0.6f, 0.5f};
static const float no_color [4]             = {0, 0, 0, 0};

// Camera distance and angle (can be controlled with the arrow keys)
static float camera_alpha = 0.35f;     // Initial camera angle
static float camera_distance = 100.0f; // Initial camera (viewer position) distance

// Depth test active ('D' key)
static bool depth_test_active = true;

// Lighting active ('L' key)
static bool lighting_active = true;

// **************************************************************************************
// Primitive objects used:
//
// glutSolidSphere   => the snowballs, the eyes, at the end of the broom-stick
// gluCylinder       => the broom, (cylinder shape), the straws, the nose (conic shape)
// gluDisk           => the "buttons"
//
// Also used:
// - A polygon (rectangle) for the ground/floor plane
// - A light source positioned in front of the snowman (far away) and above the ground
// **************************************************************************************
void display ()
{
    float tr_x, tr_y, tr_z;
    float inv_scale_y = 1.0f / ball_scale_y;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_color);
    
    // Draw the plane
    glColor3fv(plane_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, plane_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, plane_color);
    glPushMatrix();
    glTranslatef(0, -ball_scale_y * ball_radius [0] * 0.8f, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 4; ++i)
        glVertex3fv (plane [i]);
    glEnd();
    glPopMatrix();
    
    glEnable (GL_CULL_FACE); // Do not draw back-oriented polygons
    
    // Draw the snowballs
    glColor3fv(snow_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, snow_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, snow_emission_color);
    
    glPushMatrix();
    glScalef(1, ball_scale_y, 1);
    glutSolidSphere(ball_radius [0], 30, 30);
    
    tr_y = (ball_center_y [1] - ball_center_y [0]) * inv_scale_y;
    glTranslatef(0, tr_y, 0);
    glutSolidSphere(ball_radius [1], 30, 30);
    
    tr_y = (ball_center_y [2] - ball_center_y [1]) * inv_scale_y;
    glTranslatef(0, tr_y, 0);
    glutSolidSphere(ball_radius [2], 30, 30);
    glPopMatrix();
    
    // Draw the eyes (spheres)
    glColor4fv(coal_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, coal_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, coal_color);
    
    glPushMatrix();
    tr_x = ball_radius [2] * 0.3f;
    tr_y = ball_center_y [2] + 0.35 * ball_radius [2] * ball_scale_y;
    tr_z = -ball_radius [2] * 0.85f;
    glTranslatef(-tr_x, tr_y, tr_z);
    glScalef(1, 1, 0.8f);
    glutSolidSphere(eye_radius, 15, 15);
    glTranslatef(2.0f * tr_x, 0, 0);
    glutSolidSphere(eye_radius, 15, 15);
    glPopMatrix();
    
    // Draw the nose (conic shape)
    glColor4fv(nose_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, nose_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, nose_color);
    
    glPushMatrix();
    float nose_len = ball_radius [2] * 0.7f;
    float nose_radius = eye_radius * 1.2f;
    tr_y = ball_center_y [2];
    tr_z = -ball_radius [2] * 0.9f - nose_len;
    glTranslatef(0, tr_y, tr_z);
    gluCylinder(q_obj, 0, nose_radius, nose_len, 15, 15);
    glPopMatrix();
    
    // Draw the broom (cylinder)
    glColor4fv(broom_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, broom_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, broom_color);
    for (int nb = -1; nb <= 1; nb++)
    {
    float ref_size = ball_radius [1] * ball_scale_y;
    float broom_length = ref_size * 1.8f;
    float broom_delta_x = ball_radius [1] * ball_scale_y * 0.55f;
    float broom_delta_y = ball_radius [1] * ball_scale_y * 2.0f;
    float straw_length = ref_size * 0.9f;
    
    glPushMatrix();
    glTranslatef(0, ball_center_y [1] - ball_radius [1] * ball_scale_y * 0.5f, 0);
    glRotated(nb *(45+ broom_angle), 0, 0, 1);
    glRotated(-90, 1, 0, 0);
    gluCylinder(q_obj, broom_radius, broom_radius, broom_length, 5, 5);
    
    glDisable (GL_CULL_FACE); // Re-enable the drawing of back-oriented polygons (for the cylinder)
    
    // Draw the straws at the end of the broom
    glTranslatef(0, 0, broom_length);
    
    glutSolidSphere (broom_radius, 8, 8); // A sphere at the end of the broom-stick to make the ending smoother
    
    glRotated(-5 * num_straws / 2, 0, 1, 0);
    for (int j = 0; j < num_straws; ++j)
    {
        glRotated(5, 0, 1, 0);
        gluCylinder(q_obj, straw_radius_1, straw_radius_2, straw_length * straw_lengths [j], 5, 5);
    }
    glPopMatrix();
    }//for
    
    // Draw the buttons (disk shapes)
    glColor4fv(button_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, button_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, button_color);
    
    for (int b_idx = 0; b_idx <= 1; ++b_idx) // for ball index 0, 1
    {
        for (int b_angle = -18; b_angle <= 18; b_angle += 36) // draw two buttons on each ball with angle -18 and 18
        {
            glPushMatrix();
            glTranslatef(0, ball_center_y [b_idx], 0);
            glScalef(1, ball_scale_y, 1);
            glRotated(b_angle, 1, 0, 0);
            glTranslatef(0, 0, -ball_radius [b_idx]);
            gluDisk(q_obj, 0, button_radius, 20, 10);
            glPopMatrix();
        }
    }
    
    glutSwapBuffers();
}

void update_camera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (camera_alpha < 0.0f)
        camera_alpha = 2.0f * pi_value + camera_alpha;
    if (camera_alpha > 2.0f * pi_value)
        camera_alpha -= 2.0f * pi_value;
    
    float x = camera_distance * sin(camera_alpha);
    float y = 2.5f * ball_radius [0];
    float z = camera_distance * cos(camera_alpha);
    float cx = 0;
    float cy = ball_radius [0];
    float cz = 0;
    
    // (x,y,z)    => viewer position
    // (cx,cy,cz) => reference point position
    // (0,1,0)    => the up vector (how the camera is oriented)
    gluLookAt(x, y, -z, cx, cy, cz, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
}

void specialkey_func(int key, int x, int y)
{
    const float angle_incr = 0.05f;
    const float distance_incr = 2.0f;
    
    switch (key)
    {
        case GLUT_KEY_LEFT:
            camera_alpha += angle_incr;
            update_camera();
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            camera_alpha -= angle_incr;
            update_camera();
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (camera_distance > ball_radius [0]) // Don't let the camera come too close to the object
            {
                camera_distance -= distance_incr;
                update_camera();
                glutPostRedisplay();
            }
            break;
        case GLUT_KEY_DOWN:
            camera_distance += distance_incr;
            update_camera();
            glutPostRedisplay();
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'd':
        {
            if (depth_test_active)
                glDisable (GL_DEPTH_TEST);
            else
                glEnable (GL_DEPTH_TEST);
            
            depth_test_active = !depth_test_active;
            glutPostRedisplay();
            break;
        }
        case 'l':
        {
            if (lighting_active)
                glDisable (GL_LIGHTING);
            else
                glEnable (GL_LIGHTING);
            
            lighting_active = !lighting_active;
            glutPostRedisplay();
            break;
        }
    }
}

void waving_animation()
{
    broom_angle += broom_angle_inc;
    
    if (broom_angle > broom_angle_max)
        broom_angle_inc = -fabs (broom_angle_inc);
    
    if (broom_angle < broom_angle_min)
        broom_angle_inc = fabs(broom_angle_inc);
    
    glutPostRedisplay();
}

void waving_func(int value)
{
    waving_animation();
    
    glutTimerFunc(waving_delay, waving_func, 0); // Will call the function again after "waving delay"
}

void initialize_objects()
{
    q_obj = gluNewQuadric ();
    gluQuadricDrawStyle(q_obj, GLU_FILL);
    
    ball_center_y [0] = 0;
    ball_center_y [1] = (ball_radius [0] + ball_radius [1]) * ball_scale_y - ball_overlap;
    ball_center_y [2] = ball_center_y [1] + (ball_radius [1] + ball_radius [2]) * ball_scale_y - ball_overlap;
}

void free_objects()
{
    gluDeleteQuadric(q_obj);
}

int main(int argc, char * * argv)
{
    glutInit(& argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Snowman");
    
    initialize_objects ();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ((float)width) / height, 1.0f, 300.0f);
    
    update_camera();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glClearColor(0.0, 0.0, 0.5, 0.0);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard); // The normal keys
    glutSpecialFunc(specialkey_func); // The special keys (ex. arrow keys)
    
    //glutIdleFunc(waving_animation); // Will call the animation on engine idle time
    glutTimerFunc (waving_delay, waving_func, 0); // Will call the function after "waving_delay" 
    
    glutMainLoop();
    
    //free_objects();
}


