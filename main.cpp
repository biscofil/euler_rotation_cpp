#include <iostream>

#include <boost/qvm/all.hpp>

using namespace boost::qvm;
using namespace std;

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

int main() {

    const vec<double, 3> vector = {{-0.27, 0.87, 0.41}};

    vec<double, 3> theta = {{0.1, 0.1, 0.1}}; // rotation

    vec<double, 3> omega = {{0, 0, 0}}; // rotational velocity

    vec<double, 3> alpha = {{0, 0.002, 0.01}}; // rotational acceleration

    mat<double, 3, 3> M;
    mat<double, 3, 3> LIB;

    for (int i = 0; i < 100; i += 1) {

        omega += alpha;

        M = getRotationMatrix(theta);

        auto angleDot = M * omega;

        // std::cout << "d:\t" << X(angleDot) << "\t"<< Y(angleDot) << "\t"<< Z(angleDot) << "\t" << std::endl;

        theta += angleDot;

        // std::cout << "a:\t" << X(angle) << "\t"<< Y(angle) << "\t"<< Z(angle) << "\t" << std::endl;

        LIB = getLIB(theta);

        /*std::cout << "[\t" << A00(LIB) << "\t" << A01(LIB) << "\t" << A02(LIB) << std::endl
                  << "\t" << A10(LIB) << "\t" << A11(LIB) << "\t" << A12(LIB) << std::endl
                  << "\t" << A20(LIB) << "\t" << A21(LIB) << "\t" << A22(LIB) << "]" << std::endl;*/

        //std::cout << std::sqrt(std::pow(X(vector), 2) + std::pow(Y(vector), 2) + std::pow(Z(vector), 2)) << std::endl;

        auto k = LIB * vector;
        double norm = std::sqrt(std::pow(X(k), 2) + std::pow(Y(k), 2) + std::pow(Z(k), 2));
        k = k / norm;

        std::cout << X(k) << "\t" << Y(k) << "\t" << Z(k) << std::endl;

    }

    return 0;

}
