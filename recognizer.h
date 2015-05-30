#ifndef JEDI_H
#define JEDI_H

#include "quaternion.h"
#include "force.h"

//////////////////////////////////////
/// Input data processor
//////////////////////////////////////

/**
 * @desc       processes rotation from gyroscope (mostly - for positioning)
 * @param      speed, directly from gyroscope
 */
void jedi_processInput_RotationSpeed(double speed[3], uint32_t gyro_time);

/**
 * @desc       processes input acceleration (mostly for gesture recognizing)
 * @param      acceleration, directly from accelerometer
 */
void jedi_processInput_Acceleration(double acceleration[3], uint32_t gyro_time);

//////////////////////////////////////
/// Recognizer processor
//////////////////////////////////////

/**
 *	@desc       Analyzes current angular state
 * @return     the most probable angular gesture
 */
int jedi_recognizeAngular();

/**
 * @desc       Analyses current linear state
 * @return     the most probable linear gesture
 */
int jedi_recognizeLinear();

/**
 * @desc       Analyses speed
 * @return     The most probable gesture on this speed
 */
int jedi_analizeSpeed(double sped[3], uint16_t index);

/**
 * @desc       Initialize positioning quat
 */
void jedi_initQuat();

#endif
