#include "quaternion.h"

#include "stdlib.h"
#include "math.h"

////////////////////////////////////////////////////
/// Constructors
////////////////////////////////////////////////////

quat *quat_fromAngles(double rhs[3]) {
  rhs[0] /= 2;
  rhs[1] /= 2;
  rhs[2] /= 2;
  quat *res = (quat *)malloc(sizeof(quat));
  res->W = (cos(rhs[0]) * cos(rhs[1]) * cos(rhs[2]) +
            sin(rhs[0]) * sin(rhs[1]) * sin(rhs[2]));
  res->X = (sin(rhs[0]) * cos(rhs[1]) * cos(rhs[2]) -
            cos(rhs[0]) * sin(rhs[1]) * sin(rhs[2]));
  res->Y = (cos(rhs[0]) * sin(rhs[1]) * cos(rhs[2]) +
            sin(rhs[0]) * cos(rhs[1]) * sin(rhs[2]));
  res->Z = (cos(rhs[0]) * cos(rhs[1]) * sin(rhs[2]) -
            sin(rhs[0]) * sin(rhs[1]) * cos(rhs[2]));
  return res;
}

quat *quat_fromVectorAndAngle(double angle, double vector[3]) {
  angle /= 2;
  quat *res = (quat *)malloc(sizeof(quat));
  res->W = cos(angle);
  res->X = sin(angle) * vector[0];
  res->Y = sin(angle) * vector[1];
  res->Z = sin(angle) * vector[2];
  return res;
}

quat *quat_fromGravityVector(double vector[3]) {
  double grav[3] = {0, 0, -1.0};
  const double modulus =
      pow(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2],
          0.5);
  vector[0] /= modulus;
  vector[1] /= modulus;
  vector[2] /= modulus;
  double rotation_vec[3] = {-vector[1], vector[0], 0};
  double angle = asin(grav[2] * vector[2]);
  return quat_fromVectorAndAngle(angle, rotation_vec);
}

////////////////////////////////////////////////////
/// Operations with quaternions
////////////////////////////////////////////////////

quat *quat_add(quat *lhs, quat *rhs) {
  lhs->W += rhs->W;
  lhs->X += rhs->X;
  lhs->Y += rhs->Y;
  lhs->Z += rhs->Z;
  return lhs;
}

quat *quat_substract(quat *lhs, quat *rhs) {
  lhs->W -= rhs->W;
  lhs->X -= rhs->X;
  lhs->Y -= rhs->Y;
  lhs->Z -= rhs->Z;
  return lhs;
}

quat *quat_multiply(quat *lhs, quat *rhs) {
  double result[4];
  result[0] =
      lhs->W * rhs->W - lhs->X * rhs->X - rhs->Y * lhs->Y - rhs->Z * lhs->Z;
  result[1] =
      rhs->W * lhs->X + rhs->X * lhs->W - rhs->Y * lhs->Z + rhs->Z * lhs->Y;
  result[2] =
      rhs->W * lhs->Y + rhs->X * lhs->Z + rhs->Y * lhs->W - rhs->Z * lhs->X;
  result[3] =
      rhs->W * lhs->Z + rhs->X * lhs->Y + rhs->Y * lhs->X + rhs->Z * lhs->W;
  lhs->W = result[0];
  lhs->X = result[1];
  lhs->Y = result[2];
  lhs->Z = result[3];
  return lhs;
}

quat *quat_divide(quat *lhs, quat *rhs) {
  double result[4];
  result[0] =
      rhs->W * lhs->W + rhs->X * lhs->X + rhs->Y * lhs->Y + rhs->Z * lhs->Z;
  result[1] =
      rhs->X * lhs->W - rhs->W * lhs->X - rhs->Z * lhs->Y + rhs->Y * lhs->Z;
  result[2] =
      rhs->Y * lhs->W + rhs->Z * lhs->X - rhs->W * lhs->Y - rhs->X * lhs->Z;
  result[3] =
      rhs->Z * lhs->W - rhs->Y * lhs->X + rhs->X * lhs->Y - rhs->W * lhs->Z;
  double rhs_module = quat_modulus(rhs);
  lhs->W = result[0] / rhs_module;
  lhs->X = result[1] / rhs_module;
  lhs->Y = result[2] / rhs_module;
  lhs->Z = result[3] / rhs_module;
  return lhs;
}

////////////////////////////////////////////////////
/// Other functions
////////////////////////////////////////////////////

vec3 quat_rotateVector(quat *lhs, double vector[3]) {
  quat vec = {0, vector[0], vector[1], vector[2]};
  quat tmp = *lhs, ilhs = quat_inverted(lhs);
  quat_multiply(&vec, &ilhs);
  quat_multiply(&tmp, &vec);
  vec3 result = {tmp.X, tmp.Y, tmp.Z};
  return result;
}

vec3 quat_get3EulerAngles(quat *rhs) {
  const double correction_coef = 1e+4;
  double c[5] = {
      round(2 * (rhs->W * rhs->X + rhs->Y * rhs->Z) * correction_coef) /
          correction_coef,
      round((1 - 2 * (rhs->X * rhs->X + rhs->Y * rhs->Y)) * correction_coef) /
          correction_coef,
      round(2 * (rhs->Y * rhs->W - rhs->X * rhs->Z) * correction_coef) /
          correction_coef,
      round(2 * (rhs->W * rhs->Z + rhs->X * rhs->Y) * correction_coef) /
          correction_coef,
      round((1 - 2 * (rhs->Y * rhs->Y + rhs->Z * rhs->Z)) * correction_coef) /
          correction_coef};
  double result[3] = {round(atan2(c[0], c[1]) * 180 / M_PI),
                      round(asin(c[2]) * 180 / M_PI),
                      round(atan2(c[3], c[4]) * 180 / M_PI)};
  if (c[0] > -1e-3 && c[0] < 1e-2 && c[1] > -1e-3 && c[1] < 1e-2)
    result[0] = 0.0f;
  if (c[3] > -1e-3 && c[3] < 1e-2 && c[4] > -1e-3 && c[4] < 1e-2)
    result[2] = 0.0f;
  // while (result[0] > 180)
  //   result[0] -= 180;
  // while (result[0] < -180)
  //   result[0] += 180;
  // while (result[1] > 180)
  //   result[1] -= 180;
  // while (result[1] < -0)
  //   result[1] += 180;
  // while (result[2] > 90)
  //   result[2] -= 180;
  // while (result[2] < -0)
  //   result[2] += 180;
  vec3 r = {result[0], result[1], result[2]};
  return r;
}

quat quat_inverted(quat *rhs) {
  quat res = {rhs->W, -rhs->X, -rhs->Y, -rhs->Z};
  return res;
}

double quat_modulus(quat *rhs) {
  return pow(rhs->W * rhs->W + rhs->X * rhs->X + rhs->Y * rhs->Y +
                 rhs->Z * rhs->Z,
             0.5);
}

quat *quat_normalize(quat *rhs) {
  const double modulus = quat_modulus(rhs);
  rhs->W /= modulus;
  rhs->X /= modulus;
  rhs->Y /= modulus;
  rhs->Z /= modulus;
  return rhs;
}
