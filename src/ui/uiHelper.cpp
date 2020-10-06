//
// Created by filippo on 19/02/20.
//

#include <string>
#include <cstring>

#include <GL/gl.h>
#include <GL/freeglut_std.h>
#include <SDL/SDL.h>
#include <iostream>
#include "uiHelper.h"


/**
 *
 * @param str
 */
void glutPrintStr(const std::string &str) {

    const char *string = str.c_str();

    //get the length of the string to display
    int len = (int) strlen(string);

    //loop to display character by character
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

/**
 *
 */
void glPushQuaternionRotationMatrix(const quat<double> &q) {
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