//
// Created by filippo on 19/02/20.
//

#ifndef C_FORCES_UI_HELPER_H
#define C_FORCES_UI_HELPER_H

#include <GL/gl.h>

#include <boost/qvm/all.hpp>

using namespace boost::qvm;
using namespace std;

/**
 *
 * @param str
 */
void glutPrintStr(const std::string &str);

/**
 *
 * @param q
 */
void glPushQuaternionRotationMatrix(const quat<double> &q);

#endif //C_FORCES_UI_HELPER_H
