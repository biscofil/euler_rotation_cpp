//
// Created by filippo on 05/10/20.
//

#ifndef QUATERNIONS_SIMULATION_H
#define QUATERNIONS_SIMULATION_H


#include <boost/qvm/all.hpp>
#include <iostream>
#include <GL/gl.h>
#include "qvm_utils.h"
#include "ui/ui.h"
#include "ui/uiHelper.h"

using namespace boost::qvm;
using namespace std;

/**
 *
 */
class Simulation {

public:

    float t = 0;
    float deltaT = 0.01;

    quat<double> theta_quat = vect3_to_quat({{0, 0, 0}}, 1);
    vec<double, 3> omega = {{0, 0, 0}}; // rotational velocity
    vec<double, 3> alpha = {{0, 0.0, 0}}; // rotational acceleration

    mat<double, 4, 4> M_quat;

    Simulation() {
        normalize(theta_quat);
    }

    /**
     *
     * @param omega
     * @return
     */
    void computeQuatRotationMatrix(const vec<double, 3> &omega) {

        auto o1 = X(omega) * 0.5;
        auto o2 = Y(omega) * 0.5;
        auto o3 = Z(omega) * 0.5;

        M_quat = {{
                          {0, -o1, -o2, -o3},
                          {o1, 0, o3, -o2},
                          {o2, -o3, 0, o1},
                          {o3, o2, -o1, 0}
                  }};
    }

    /**
     *
     */
    void simulateStep() {

        X(alpha) = std::sin(t);
        Y(alpha) = std::cos(t);

        t += deltaT;

        omega += alpha * deltaT;

        computeQuatRotationMatrix(omega);

        auto angleDot = vect4_to_quat(M_quat * quat_to_vect4(theta_quat));

        theta_quat += angleDot * deltaT;
        normalize(theta_quat);

    }

    /**
     *
     */
    void draw() {

        simulateStep();

        glPushQuaternionRotationMatrix(theta_quat);

        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(10, 0, 0);
        glEnd();

        glColor3ub(0, 255, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 10, 0);
        glEnd();

        glColor3ub(0, 0, 255);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 10);
        glEnd();

        glColor3ub(100, 100, 100);
        //glutWireCube(10);
        glutSolidTeapot(5);

        glPopMatrix();

    }

};


#endif //QUATERNIONS_SIMULATION_H
