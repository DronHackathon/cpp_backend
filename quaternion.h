#ifndef QUATERNION_H
#define QUATERNION_H

// Declaration of quaternion structure
typedef struct { double W, X, Y, Z; } quat;

typedef struct { double X, Y, Z; } vec3;

////////////////////////////////////////////////////
/// Constructors
////////////////////////////////////////////////////

/**
 * @desc       Constructs qauternion from three angles
 * @param      Pointer to array of angles
 * @return     Pointer to newly created quat
 */
quat *quat_fromAngles(double angles[3]);

/**
 * @desc       Constructs qauternion from vector and angle
 * @param      angle of rotation
 * @param      vector of rotation
 * @return     Pointer to newly created quat
 */
quat *quat_fromVectorAndAngle(double angle, double vector[3]);

/**
 * @desc       Constructs qauternion from gravity vector
 * @param      vector that as to be as similar to gravity vector, as possible
 * @return     Pointer to newly created quat
 */
quat *quat_fromGravityVector(double vector[3]);

////////////////////////////////////////////////////
/// Operations with quaternions
////////////////////////////////////////////////////

/**
 * @desc       Adds quaternion rhs to quaternion lhs
 * @param      Quat value to be added to
 * @param      Quat value to be added
 * @return     lhs after adding rhs
 */
quat *quat_add(quat *lhs, quat *rhs);

/**
 * @desc       Divides quaternion rhs from quaternion lhs
 * @param      Quat value to be divided from
 * @param      Quat value to be divided
 * @return     lhs after dividing rhs
 */
quat *quat_substract(quat *lhs, quat *rhs);

/**
 * @desc       Multiplies quaternion lhs by quaternion rhs
 * @param      Quat value to be multiplied
 * @param      Quat value to be multiplied by
 * @return     lhs after multiplying by rhs
 */
quat *quat_multiply(quat *lhs, quat *rhs);

/**
 * @desc       Divides quaternion lhs by quaternion rhs
 * @param      Quat value to be divided
 * @param      Quat value to be divided by
 * @return     lhs after dividing by rhs
 */
quat *quat_divide(quat *lhs, quat *rhs);

////////////////////////////////////////////////////
/// Other functions
////////////////////////////////////////////////////

/**
 * @desc          Rotates vector using quaternion
 * @param         quat of rotation
 * @param         vector to be rotated
 * @return        rotated vector
*/
vec3 quat_rotateVector(quat *lhs, double vector[3]);

/**
 * @desc          Calculates angles using quaternion
 * @param         quat - source of data
 * @return        rotation angles
 */
vec3 quat_get3EulerAngles(quat *rhs);

/**
 * @desc       Gives copy of current inverted quat
 * @param      Quat to be inverted
 * @return     Inverted copy of rhs
 */
quat quat_inverted(quat *rhs);

/**
 * @desc       Calculates modulus of quat
 * @param      Quat which modulus is supposed to be calculated
 * @return     Modulus of quat
 */
double quat_modulus(quat *rhs);

/**
 * @desc       Normalizes quaternion
 * @param      Pointer to quat to be normalized
 * @return     Pointer to normalized rhs
 */
quat *quat_normalize(quat *rhs);

#endif
