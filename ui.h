//
// Created by filippo on 05/10/20.
//

#ifndef QUATERNIONS_UI_H
#define QUATERNIONS_UI_H

#define FPS 25

#include <boost/qvm/all.hpp>
#include <GL/glut.h>
#include <cmath>
#include <cstring>
#include <memory>

using namespace boost::qvm;
using namespace std;

class Simulation;

class UI {

    static std::shared_ptr<Simulation> simulation;

    static float deltaAngle;
    static float deltaMove[3];

    static float angle; // angle of rotation for the camera direction
    static int fov;
    static float look[3]; // camera's direction
    static float eye[3]; // XZ position of the camera

    static bool leftDragging;
    static bool shiftPressed;
    static bool middleDragging;
    static int draggingCenter[2];
    static float middleDraggingAngle;

public:

    /**
     *
     * @param argc
     * @param argv
     * @param s
     */
    UI(int argc, char **argv, std::shared_ptr<Simulation> s);

    /**
     *
     */
    static void idle(int);

    /**
     *
     * @param v
     */
    static void drawVector(const vec<double, 3> &v);

    /**
     *
     */
    static void drawOverlay();

    /**
     *
     */
    static void drawGrid();

    /**
     *
     */
    static void pushQuaternionRotationMatrix(const quat<double> &q) {
        glPushMatrix();

        // turn the quaternion into a rotation matrix
        GLfloat d = S(q);
        GLfloat a = X(q);
        GLfloat b = Y(q);
        GLfloat c = Z(q);

        GLfloat d_sq = std::pow(d, 2);
        GLfloat a_sq = std::pow(a, 2);
        GLfloat b_sq = std::pow(b, 2);
        GLfloat c_sq = std::pow(c, 2);

        GLfloat rot_matrix[4][4] = {
                {d_sq + a_sq - b_sq - c_sq, 2 * a * b - 2 * c * d,     2 * a * c + 2 * b * d,     0},
                {2 * a * b + 2 * c * d,     d_sq - a_sq + b_sq - c_sq, 2 * b * c - 2 * a * d,     0},
                {2 * a * c - 2 * b * d,     2 * b * c + 2 * a * d,     d_sq - a_sq - b_sq + c_sq, 0},
                {0,                         0,                         0,                         1}
        };

        // multiply it with the top of the stack
        glMultMatrixf((GLfloat *) rot_matrix);
    }

    /**
     * Compute the new camera pos when moving in a direction
     * @param _deltaMove
     */
    static void computeEyePosition(const float _deltaMove[3]);

    static void computeDir(float _deltaAngle);

    static void renderScene();

    static void changeSize(int w, int h);

    static void pressKey(int key, int xx, int yy);

    static void releaseKey(int key, int x, int y);

    static void zoom(float scale);

    /**
     *
     * @param button
     * @param state
     * @param x
     * @param y
     */
    static void mouseCallback(int button, int state, int x, int y);

    /**
     *
     * @param mx
     * @param my
     */
    static void mouseMove(int mx, int my);

};

#endif //QUATERNIONS_UI_H
