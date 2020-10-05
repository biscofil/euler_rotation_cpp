//
// Created by filippo on 05/10/20.
//

#ifndef QUATERNIONS_SIMULATION_H
#define QUATERNIONS_SIMULATION_H


#include <boost/qvm/all.hpp>
#include <iostream>
#include <GL/gl.h>

using namespace boost::qvm;
using namespace std;

/**
 *
 */
class Simulation {

public:

    float t = 0;
    float deltaT = 0.01;

    /*const vec<double, 3> vector = {{-0.27, 0.87, 0.41}};
    vec<double, 3> rotatedVector;*/

    vec<double, 3> theta = {{0.1, 0.1, 0.1}}; // rotation
    vec<double, 3> omega = {{0, 0, 0}}; // rotational velocity
    vec<double, 3> alpha = {{0, 0.1, 0}}; // rotational acceleration

    mat<double, 3, 3> M;
    mat<double, 3, 3> LIB;

    /**
     * Returns the rotation matrix that converts Inertial (global) coordinates into Body (Local) coordinates
     * @param angle
     * @return
     */
    inline mat<double, 3, 3> getLIB(const vec<double, 3> &angle) {

        auto psi = X(angle);
        auto theta = Y(angle);
        auto fi = Z(angle);

        auto c_psi = std::cos(psi);
        auto s_psi = std::sin(psi);
        auto c_theta = std::cos(theta);
        auto s_theta = std::sin(theta);
        auto c_fi = std::cos(fi);
        auto s_fi = std::sin(fi);

        return {{
                        {
                                (c_fi * c_psi) - (s_fi * c_theta * s_psi),
                                -(c_fi * c_theta * s_psi) - (s_fi * c_psi),
                                s_theta * s_psi
                        },
                        {
                                (s_fi * c_theta * c_psi) + (c_fi * s_psi),
                                (c_fi * c_theta * c_psi) - (s_fi * s_psi),
                                -s_theta * c_psi
                        },
                        {
                                c_fi * s_theta,
                                c_fi * s_theta,
                                c_theta
                        }
                }};
    }

    /**
     *
     * @param angle
     * @return
     */
    inline mat<double, 3, 3> getRotationMatrix(const vec<double, 3> &angle) {

        auto theta = Y(angle);
        auto fi = Z(angle);

        auto s_theta = std::sin(theta);
        auto c_fi = std::cos(fi);
        auto s_fi = std::sin(fi);
        auto t_theta = std::tan(theta);

        return {{
                        {s_fi / s_theta, c_fi / s_theta, 0.0},
                        {c_fi, -s_fi, 0.0},
                        {-s_fi / t_theta, -c_fi / t_theta, 1.0}
                }};
    }

    /**
     *
     */
    void simulateStep() {

        t += deltaT;

        omega += alpha * deltaT;

        M = getRotationMatrix(theta);

        auto angleDot = M * omega;

        // std::cout << "d:\t" << X(angleDot) << "\t"<< Y(angleDot) << "\t"<< Z(angleDot) << "\t" << std::endl;

        theta += angleDot * deltaT;

        // std::cout << "a:\t" << X(angle) << "\t"<< Y(angle) << "\t"<< Z(angle) << "\t" << std::endl;

        LIB = getLIB(theta);

        /*std::cout << "[\t" << A00(LIB) << "\t" << A01(LIB) << "\t" << A02(LIB) << std::endl
                  << "\t" << A10(LIB) << "\t" << A11(LIB) << "\t" << A12(LIB) << std::endl
                  << "\t" << A20(LIB) << "\t" << A21(LIB) << "\t" << A22(LIB) << "]" << std::endl;*/

        //std::cout << std::sqrt(std::pow(X(vector), 2) + std::pow(Y(vector), 2) + std::pow(Z(vector), 2)) << std::endl;

        /*rotatedVector = LIB * vector;
        double norm = std::sqrt(
                std::pow(X(rotatedVector), 2) + std::pow(Y(rotatedVector), 2) + std::pow(Z(rotatedVector), 2));
        rotatedVector = rotatedVector / norm;*/

        // std::cout << X(k) << "\t" << Y(k) << "\t" << Z(k) << std::endl;

    }

    /**
     *
     * @param v
     */
    void computeAndDraw(const vec<double, 3> &v) {

        auto rotatedVector = LIB * v;
        double norm = std::sqrt(
                std::pow(X(rotatedVector), 2)
                + std::pow(Y(rotatedVector), 2)
                + std::pow(Z(rotatedVector), 2));
        rotatedVector = rotatedVector / norm;

        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(X(rotatedVector), Y(rotatedVector), Z(rotatedVector));
        glEnd();

        glPushAttrib(GL_ENABLE_BIT);  // dashed
        glLineStipple(4, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);

        glBegin(GL_LINES);
        glVertex3f(X(rotatedVector), Y(rotatedVector), Z(rotatedVector));
        glVertex3f(X(rotatedVector), 0, Z(rotatedVector));
        glEnd();

        glPopAttrib();

    }

    /**
     *
     */
    void draw() {

        simulateStep();

        /*glPushAttrib(GL_ENABLE_BIT);  // dashed
        glLineStipple(4, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);*/

        glColor3ub(255, 0, 0);
        computeAndDraw({{1, 0, 0}});

        glColor3ub(0, 255, 0);
        computeAndDraw({{0, 1, 0}});

        glColor3ub(0, 0, 255);
        computeAndDraw({{0, 0, 1}});

        //glPopAttrib();

    }

};


#endif //QUATERNIONS_SIMULATION_H
