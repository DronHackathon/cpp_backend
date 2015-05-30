#include "recognizer.h"
#include "quaternion.h"
#include "force.h"
#include "typedefs.h"
#include <stdint.h>

#define gravity 8191

// Global quaternion
static quat global_quat = {1, 0, 0, 0};
// Recognition variables
double spd[3] = {0, 0, 0}, spd_lpf[3] = {0, 0, 0}, spd_hpf[3] = {0, 0, 0},
       acc_hold[3] = {0, 0, 0};
bool action_done;

//////////////////////////////////////
/// Input data processor
//////////////////////////////////////

void jedi_processInput_RotationSpeed(double speed[3], uint32_t gyro_time) {
  // if (!global_quat)
  //   jedi_initQuat();
  if (fabs(speed[0]) > GYRO_THRESHOLD)
    speed[0] *= 0.07 * gyro_time / 10000.0;
  else
    speed[0] = 0.0;
  if (fabs(speed[1]) > GYRO_THRESHOLD)
    speed[1] *= 0.07 * gyro_time / 10000.0;
  else
    speed[1] = 0.0;
  if (fabs(speed[2]) > GYRO_THRESHOLD)
    speed[2] *= 0.07 * gyro_time / 10000.0;
  else
    speed[2] = 0.0;
  quat *local = quat_fromAngles(speed);
  global_quat = *quat_normalize(quat_multiply(&global_quat, local));
  free(local);
  // fprintf(stderr, "global_quat %3.2f %3.2f %3.2f %3.2f\n", global_quat.W,
  //         global_quat.X, global_quat.Y, global_quat.Z);
}

void jedi_processInput_Acceleration(double acc[3], uint32_t accel_time) {
  const double curAcc =
      pow(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2], 0.5);
  if ((curAcc > gravity - ACC_THRESHOLD) &&
      (curAcc < gravity + ACC_THRESHOLD)) {
    quat *grav_quat = quat_fromGravityVector(acc);
    global_quat = *quat_normalize(quat_add(&global_quat, grav_quat));
    //  fprintf(stderr, "global_quat %3.2f %3.2f %3.2f %3.2f\n", global_quat.W,
    //          global_quat.X, global_quat.Y, global_quat.Z);
    free(grav_quat);
    const double spdSqr = spd_hpf[0] * spd_hpf[0] + spd_hpf[1] * spd_hpf[1] +
                          spd_hpf[2] * spd_hpf[2];
    if (spdSqr < ZERO_SPEED_THRESHOLD * ZERO_SPEED_THRESHOLD) {
      spd[0] = 0;
      spd[1] = 0;
      spd[2] = 0;
      spd_lpf[0] = 0;
      spd_lpf[1] = 0;
      spd_lpf[2] = 0;
      spd_hpf[0] = 0;
      spd_hpf[1] = 0;
      spd_hpf[2] = 0;
      if (action_done) {
        printf("Ready for the next gesture.\n");
        action_done = false;
      }
    }
    vec3 rot_acc = quat_rotateVector(&global_quat, acc);
    acc_hold[0] = 0.7 * acc_hold[0] + 0.3 * rot_acc.X;
    acc_hold[1] = 0.7 * acc_hold[1] + 0.3 * rot_acc.Y;
    acc_hold[2] = 0.7 * acc_hold[2] + 0.3 * rot_acc.Z;

    for (int i = 0; i < 3; i++) {
      spd[i] += acc_hold[i] * accel_time / 200.0;
      spd_lpf[i] += (spd[i] - spd_lpf[i]) *
                    (1 - exp(-(double)accel_time / SPEED_LPF_TIME_CONSTANT));
      spd_hpf[i] = spd[i] - spd_lpf[i];
    }
  }
}

//////////////////////////////////////
/// Recognizer processor
//////////////////////////////////////

int jedi_recognizeAngular() {
  vec3 current_angle = quat_get3EulerAngles(&global_quat);
  printf("%3.0lf %3.0lf %3.0lf \n", current_angle.X, current_angle.Y);
  if (current_angle.X > lTr && current_angle.X < 90) {
    return PULL;
  } else if (current_angle.X < -lTr && current_angle.X > -90) {
    return PUSH;
  }
  // rotation around vertical axis
  if (current_angle.Y > aTr && current_angle.Y < 90) {
    return LEFT;
  } else if (current_angle.Y < -aTr && current_angle.Y > -90) {
    return RIGHT;
  }
  return 0;
}

int jedi_recognizeLinear() { return 0; }

int jedi_analizeSpeed(double sped[3], uint16_t index) { return 0; }

void jedi_initQuat() {
  // global_quat = (quat *)malloc(sizeof(quat));
  global_quat.W = 1;
  global_quat.X = 0;
  global_quat.Y = 0;
  global_quat.Z = 0;
}
