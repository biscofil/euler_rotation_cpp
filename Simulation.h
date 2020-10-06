//
// Created by filippo on 05/10/20.
//

#ifndef QUATERNIONS_SIMULATION_H
#define QUATERNIONS_SIMULATION_H


#include <boost/qvm/all.hpp>
#include <iostream>
#include <GL/gl.h>
#include "qvm_utils.h"
#include "ui.h"

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

    vec<double, 3> theta_vector = {{0, 0, 0}}; // rotation
    quat<double> theta_quat = vect3_to_quat(theta_vector, 1);
    vec<double, 3> omega = {{0, 0, 0}}; // rotational velocity
    vec<double, 3> alpha = {{0, 0.0, 0}}; // rotational acceleration

    mat<double, 3, 3> M_vector;
    mat<double, 3, 3> LIB;
    mat<double, 4, 4> M_quat;

    Simulation() {
        normalize(theta_quat);
    }

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
    inline mat<double, 3, 3> getVecRotationMatrix(const vec<double, 3> &angle) {

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
     * @param omega
     * @return
     */
    inline mat<double, 4, 4> getQuatRotationMatrix(const vec<double, 3> &omega) {

        auto o1 = X(omega) * 0.5;
        auto o2 = Y(omega) * 0.5;
        auto o3 = Z(omega) * 0.5;

        return {{
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

        Z(alpha) = std::sin(t);

        t += deltaT;

        omega += alpha * deltaT;

        // vector
        try {
            M_vector = getVecRotationMatrix(theta_vector);

            auto angleDot = M_vector * omega;

            // std::cout << "d:\t" << X(angleDot) << "\t"<< Y(angleDot) << "\t"<< Z(angleDot) << "\t" << std::endl;

            theta_vector += angleDot * deltaT;

            // std::cout << "a:\t" << X(angle) << "\t"<< Y(angle) << "\t"<< Z(angle) << "\t" << std::endl;

            LIB = getLIB(theta_vector);
        } catch (...) {
            cout << "Step > Vector error" << endl;
        }

        // quat
        try {

            M_quat = getQuatRotationMatrix(omega);

            auto angleDot = vect4_to_quat(M_quat * quat_to_vect4(theta_quat));
            //normalize(angleDot);
            //std::cout << S(angleDot) << "\t" << X(angleDot) << "\t" << Y(angleDot) << "\t" << Z(angleDot) << std::endl;

            theta_quat += angleDot * deltaT;
            normalize(theta_quat);
            //theta = (angleDot * theta) * deltaT;
            //normalize(theta);

        } catch (...) {
            cout << "Step > Quat error" << endl;
        }


    }

    /**
     *
     * @param v
     */
    void computeAndDraw(const vec<double, 3> &v) {

        // vec
        try {
            auto rotatedVector = LIB * v;
            double norm = std::sqrt(
                    std::pow(X(rotatedVector), 2)
                    + std::pow(Y(rotatedVector), 2)
                    + std::pow(Z(rotatedVector), 2));
            rotatedVector = rotatedVector / norm;

            UI::drawVector(rotatedVector);

        } catch (...) {
            cout << "UI > Vector error" << endl;
        }

        // quat
        try {

            glColor3ub(255, 255, 255);

            auto rotatedVec = theta_quat * vect3_to_quat(v) * conjugate(theta_quat);

            auto k = quat_to_vec3(rotatedVec);
            //auto k = rotatedVec;
            k = normalize_vec3(k);

            UI::drawVector(k);

        } catch (...) {
            cout << "UI > Quat error" << endl;
        }

    }

    /**
     *
     */
    void draw() {

        simulateStep();

        glColor3ub(255, 0, 0);
        computeAndDraw({{1, 0, 0}});

        glColor3ub(0, 255, 0);
        computeAndDraw({{0, 1, 0}});

        glColor3ub(0, 0, 255);
        computeAndDraw({{0, 0, 1}});

    }

};


#endif //QUATERNIONS_SIMULATION_H
