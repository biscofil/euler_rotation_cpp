//
// Created by filippo on 05/10/20.
//

#ifndef QUATERNIONS_QVM_UTILS_H
#define QUATERNIONS_QVM_UTILS_H

#include <boost/qvm/all.hpp>
#include <iostream>

using namespace boost::qvm;
using namespace std;

/**
 * Normalize vector
 * @param v
 * @return
 */
inline vec<double, 3> normalize_vec3(const vec<double, 3> &v) {
    return v / std::sqrt(std::pow(X(v), 2) + std::pow(Y(v), 2) + std::pow(Z(v), 2));
}

inline quat<double> vect3_to_quat(const vec<double, 3> &v, double s = 0) {
    return {{s, X(v), Y(v), Z(v)}};
}

inline vec<double, 3> quat_to_vec3(const quat<double> &q) {
    return {{X(q), Y(q), Z(q)}};
}

/**
 *
 * @param v
 * @param q
 * @return
 */
inline vec<double, 3> rotate_vec3_by_quat(const vec<double, 3> &v, const quat<double> &q) {
    return normalize_vec3(quat_to_vec3(q * vect3_to_quat(v) * conjugate(q)));
}

inline quat<double> vect4_to_quat(const vec<double, 4> &v) {
    return {{X(v), Y(v), Z(v), W(v)}};
}

inline vec<double, 4> quat_to_vect4(const quat<double> &q) {
    return {{S(q), X(q), Y(q), Z(q)}};
}
/*
vec<double, 3> to_euler(quat<double> q) {

    // w, x, y, z;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (S(q) * X(q) + X(q) * Z(q));
    double cosr_cosp = 1 - 2 * (X(q) * X(q) + X(q) * Y(q));
    double roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (S(q) * X(q) - Z(q) * X(q));
    double pitch;
    if (std::abs(sinp) >= 1)
        pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        pitch = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (S(q) * Z(q) + X(q) * Y(q));
    double cosy_cosp = 1 - 2 * (X(q) * X(q) + Z(q) * Z(q));
    double yaw = std::atan2(siny_cosp, cosy_cosp);

    return {roll, pitch, yaw}; // roll, pitch, yaw;
}

mat<double, 4, 4> convert3x3to4x4(mat<double, 3, 3> const &mat) {
    return {{
                    {1.0, 0.0, 0.0, 0.0},
                    {0.0, A00(mat), A01(mat), A02(mat)},
                    {0.0, A10(mat), A11(mat), A12(mat)},
                    {0.0, A20(mat), A21(mat), A22(mat)}
            }};
}

quat<double> mat_quat_mult(mat<double, 4, 4> m, quat<double> q) {
    return {{
                    A00(m) * S(q) + A01(m) * X(q) + A02(m) * Y(q) + A03(m) * Z(q),
                    A10(m) * S(q) + A11(m) * X(q) + A12(m) * Y(q) + A13(m) * Z(q),
                    A20(m) * S(q) + A21(m) * X(q) + A22(m) * Y(q) + A23(m) * Z(q),
                    A30(m) * S(q) + A31(m) * X(q) + A32(m) * Y(q) + A33(m) * Z(q)
            }};
}

void print(mat<double, 4, 4> mat) {
    std::cout << A00(mat) << "\t" << A01(mat) << "\t" << A02(mat) << "\t" << A03(mat) << std::endl;
    std::cout << A10(mat) << "\t" << A11(mat) << "\t" << A12(mat) << "\t" << A13(mat) << std::endl;
    std::cout << A20(mat) << "\t" << A21(mat) << "\t" << A22(mat) << "\t" << A23(mat) << std::endl;
    std::cout << A30(mat) << "\t" << A31(mat) << "\t" << A32(mat) << "\t" << A33(mat) << std::endl;
}

void print(quat<double> q) {
    std::cout << S(q) << "\t" << X(q) << "\t" << Y(q) << "\t" << Z(q) << std::endl;
}*/

#endif //QUATERNIONS_QVM_UTILS_H
