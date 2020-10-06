//
// Created by filippo on 06/10/20.
//

#include "ui.h"
#include "../Simulation.h"

std::shared_ptr<Simulation> UI::simulation = nullptr;

float UI::deltaAngle = 0.0f;
float UI::deltaMove[3] = {0.0f, 0.0f, 0.0f};
float UI::angle = 0.0;

int UI::fov = 50;
float UI::look[3] = {0.0f, 0.0f, -1.0f};
float UI::eye[3] = {10.0f, 1.0f, 10.0f};

bool UI::leftDragging = false;
bool UI::shiftPressed = false;
bool UI::middleDragging = false;
int UI::draggingCenter[2] = {0, 0};
float UI::middleDraggingAngle = 0;

UI::UI(int argc, char **argv, std::shared_ptr<Simulation> s) {
    UI::simulation = s;

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutInitWindowSize(1000, 1000);   // Set the window's initial width & height
    glutCreateWindow("Physics Engine"); // Create a window with the given title

    //glutFullScreen();

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // ############ KEYBOARD
    //glutKeyboardFunc
    glutSpecialFunc(pressKey);
    glutIgnoreKeyRepeat(1);
    glutSpecialUpFunc(releaseKey);

    // ############ MOUSE
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMove);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // OpenGL init
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glutTimerFunc(1000 / FPS, idle, 0);

    // enter GLUT event processing loop
    glutMainLoop();
}

void UI::idle(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, idle, 0);
}

void UI::drawOverlay() {

    bool lightingOn = glIsEnabled(GL_LIGHTING);

    if (lightingOn) glDisable(GL_LIGHTING);

    // Save transform attributes (Matrix Mode and Enabled Modes)
    glPushAttrib(GL_TRANSFORM_BIT | GL_ENABLE_BIT);

    // Save projection matrix and set unit transform
    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glLoadIdentity();
    //glOrtho(-asp, +asp, -1, 1, -1, 1);
    // Save model view matrix and set to identity
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw bottom with texture
    glEnable(GL_TEXTURE_2D);

    //glBindTexture(GL_TEXTURE_2D, currentTexture);

    glColor3f(1, 1, 1);

    // Print time
    glRasterPos2f(-1, -1);
    glutPrintStr("T:" + std::to_string(simulation->t));

    // Print Alpha
    glRasterPos2f(-1, 0.9);
    glutPrintStr("Alpha X: " + std::to_string(X(simulation->alpha)));
    glRasterPos2f(-1, 0.85);
    glutPrintStr("Alpha Y: " + std::to_string(Y(simulation->alpha)));
    glRasterPos2f(-1, 0.8);
    glutPrintStr("Alpha Z: " + std::to_string(Z(simulation->alpha)));

    // Print Omega
    glRasterPos2f(-1, 0.7);
    glutPrintStr("Omega X: " + std::to_string(X(simulation->omega)));
    glRasterPos2f(-1, 0.65);
    glutPrintStr("Omega Y: " + std::to_string(Y(simulation->omega)));
    glRasterPos2f(-1, 0.6);
    glutPrintStr("Omega Z: " + std::to_string(Z(simulation->omega)));


    glDisable(GL_TEXTURE_2D);

    // Draw the inside in grey
    //glColor3f(0.6, 0.6, 0.6);

    /*glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, 20);
    glVertex2f(10, 10);
    glVertex2f(20, 10);
    glEnd();*/

    // TODO draw small 3d gimbal

    //  Reset model view matrix
    glPopMatrix();

    //  Reset projection matrix
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    //  Pop transform attributes (Matrix Mode and Enabled Modes)
    glPopAttrib();

    if (lightingOn) glEnable(GL_LIGHTING);
}

/**
     *
     * @param v
     */
void UI::drawVector(const vec<double, 3> &v) {
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(X(v), Y(v), Z(v));
    glEnd();

    glPushAttrib(GL_ENABLE_BIT);  // dashed
    glLineStipple(4, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);

    glBegin(GL_LINES);
    glVertex3f(X(v), Y(v), Z(v));
    glVertex3f(X(v), 0, Z(v));
    glEnd();

    glPopAttrib();
}

/**
 *
 */
void UI::drawGrid() {

    glLineWidth(1);
    glColor4ub(100, 100, 100, 200);

    for (int i = -10; i < 11; i++) {

        // x / y

        /*glBegin(GL_LINES);
        glVertex3f(i, 0, 0);
        glVertex3f(i, 10, 0);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(0, i, 0);
        glVertex3f(10, i, 0);
        glEnd();*/

        // y / z

        /*glBegin(GL_LINES);
        glVertex3f(0, i, 0);
        glVertex3f(0, i, 10);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(0, 0, i);
        glVertex3f(0, 10, i);
        glEnd();*/

        // x / z

        glBegin(GL_LINES);
        glVertex3f(i, 0, -10);
        glVertex3f(i, 0, 10);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(-10, 0, i);
        glVertex3f(10, 0, i);
        glEnd();
    }

    glLineWidth(2);

    glColor3ub(0, 0, 255);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
    glEnd();

    glColor3ub(0, 255, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);
    glEnd();

}

/**
 * Compute the new camera pos when moving in a direction
 * @param _deltaMove
 */
void UI::computeEyePosition(const float _deltaMove[3]) {

    eye[0] += _deltaMove[0] * look[0] * 0.1f;
    eye[1] += _deltaMove[1] * look[1] * 0.1f;
    eye[2] += _deltaMove[2] * look[2] * 0.1f;
}

void UI::computeDir(float _deltaAngle) {

    angle += _deltaAngle;
    look[0] = std::sin(angle);
    look[2] = -std::cos(angle);
}

void UI::renderScene() {

    /*if (deltaMove)
        computeEyePosition(deltaMove);

    if (deltaAngle)
        computeDir(deltaAngle);*/

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    // Reset transformations
    glLoadIdentity();

    // ##############################
    // Set the camera
    // ##############################

    gluLookAt(eye[0], eye[1], eye[2],
              look[0], look[1], look[2],
              0.0f, 1.0f, 0.0f);

    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glRotatef(1, 1.0f, 0.0f, 0.0f); // camera UP

    // ##############################

    drawGrid();

    simulation->draw();

    drawOverlay();

    glutSwapBuffers();

}

void UI::changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio = 1.0 * w / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(fov, ratio, 1, 1000);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void UI::pressKey(int key, int xx, int yy) {}

void UI::releaseKey(int key, int x, int y) {}

void UI::zoom(float scale) {
    eye[0] = look[0] + ((eye[0] - look[0]) * scale);
    eye[1] = look[1] + ((eye[1] - look[1]) * scale);
    eye[2] = look[2] + ((eye[2] - look[2]) * scale);
}

/**
 *
 * @param button
 * @param state
 * @param x
 * @param y
 */
void UI::mouseCallback(int button, int state, int x, int y) {

    if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {

            if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
                shiftPressed = true;
            }

            middleDragging = true;
            //
            draggingCenter[0] = x;
            draggingCenter[1] = y;
        } else if (state == GLUT_UP) {

            shiftPressed = false;
            middleDragging = false;

            deltaMove[0] = 0;
            deltaMove[1] = 0;
            deltaMove[2] = 0;
        }

    } else if (button == GLUT_LEFT_BUTTON) {

        if (state == GLUT_DOWN) {
            leftDragging = true;
            //
            draggingCenter[0] = x;
            draggingCenter[1] = y;
        } else if (state == GLUT_UP) {
            leftDragging = false;

            deltaMove[0] = 0;
            deltaMove[1] = 0;
            deltaMove[2] = 0;
        }

    } else if (button == 3) { // UP

        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events

        zoom(0.95);

    } else if (button == 4) { // DOWN

        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events

        zoom(1.05);

    }

}

/**
 *
 * @param mx
 * @param my
 */
void UI::mouseMove(int mx, int my) {

    int delta[2] = {
            mx - draggingCenter[0],
            my - draggingCenter[1]
    };

    if (middleDragging) {

        //computeDir(deltaX / 100);

        eye[0] += delta[0] / 100;
        //eye[1] += delta[1] / 100;
        //eye[2] += delta[2] / 100;

        if (shiftPressed) {

            look[0] += delta[0] / 100;
            //look[1] += delta[1] / 100;
            //look[2] += delta[2] / 100;

        }

    }

}