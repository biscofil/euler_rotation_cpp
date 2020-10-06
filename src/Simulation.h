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

    const double width = 1;
    const double height = 2;
    const double depth = 3;

    const double mass = 5;

    const mat<double, 3, 3> I; // moment of inertia

    const mat<double, 3, 3> invI;
public:

    float t = 0;
    float deltaT = 0.01;

    quat<double> theta = vect3_to_quat({{0, 0, 0}}, 1);
    vec<double, 3> omega = {{0, 0, 0}}; // rotational velocity
    vec<double, 3> alpha = {{0, 0.0, 0}}; // rotational acceleration
    vec<double, 3> torque = {{0, 0, 0}}; // torque

    mat<double, 4, 4> M;

    Simulation() : I({{
                              {Mx(), 0, 0},
                              {0, My(), 0},
                              {0, 0, Mz()}
                      }}),
                   invI(inverse(I)) {
        normalize(theta);
    }

    constexpr double Mx() const {
        return (1.0 / 12.0) * mass * (std::pow(width, 2) + std::pow(height, 2));
    }

    constexpr double My() const {
        return (1.0 / 12.0) * mass * (std::pow(depth, 2) + std::pow(height, 2));
    }

    constexpr double Mz() const {
        return (1.0 / 12.0) * mass * (std::pow(depth, 2) + std::pow(width, 2));
    }

    /**
     *
     * @param omega
     * @return
     */
    void computeQuaternionRotationMatrix() {

        auto o1 = X(omega) * 0.5;
        auto o2 = Y(omega) * 0.5;
        auto o3 = Z(omega) * 0.5;

        M = {{
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

        t += deltaT;

        X(torque) = 0;
        Y(torque) = 0;
        if (t < 4) {
            X(torque) = std::sin(t) * 3;
        } else if (t > 6 && t < 8) {
            Y(torque) = std::sin(t) * 2;
        }

        alpha = invI * torque;

        omega += alpha * deltaT;

        computeQuaternionRotationMatrix();

        auto angleDot = vect4_to_quat(M * quat_to_vect4(theta));

        theta += angleDot * deltaT;
        normalize(theta);

    }

    /**
     *
     */
    void draw() {

        simulateStep();

        glPushQuaternionRotationMatrix(theta); // rot

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

        // Enable lighting
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glColor3ub(100, 100, 100);

        glPushMatrix(); // scale
        glScaled(width, height, depth);

        glutSolidCube(1);

        glPopMatrix(); // scale

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glPopMatrix(); // rot

    }

};


#endif //QUATERNIONS_SIMULATION_H
