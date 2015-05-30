#include "recognizer.h"
#include "quaternion.h"
#include "force.h"
#include "typedefs.h"
#include <stdint.h>

#define gravity 8191

uint32_t analyze_speed(double v_axis, double v_sum, double ths_plus,
                       double ths_minus, double axis_rate, uint32_t ret_plus,
                       uint32_t ret_minus, uint32_t ret_none);

// Global quaternion
static quat global_quat = {1, 0, 0, 0};
// Recognition variables
static double spd[3] = {0, 0, 0}, spd_lpf[3] = {0, 0, 0},
              spd_hpf[3] = {0, 0, 0}, acc_hold[3] = {0, 0, 0};
static bool action_done = false;

static uint32_t delta_time = 0;

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
  delta_time = accel_time;
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
  // printf("%3.0lf %3.0lf %3.0lf \n", current_angle.X, current_angle.Y);
  if (current_angle.X > lTr && current_angle.X < 90) {
    return DRONE_FORW;
  } else if (current_angle.X < -lTr && current_angle.X > -90) {
    return DRONE_BACKW;
  }
  // rotation around vertical axis
  if (current_angle.Y > aTr && current_angle.Y < 90) {
    return DRONE_LEFT;
  } else if (current_angle.Y < -aTr && current_angle.Y > -90) {
    return DRONE_RIGHT;
  }
  return 0;
}

int jedi_recognizeLinear() {
  static uint32_t linear_state = 0;
  static double linear_time = 0.0;
  double speed_module =
      pow(spd[0] * spd[0] + spd[1] * spd[1] + spd[2] * spd[2], 0.5);
  uint32_t cur_state =
      analyze_speed(spd[2], speed_module, Z_SPEED_PLUS_THRESHOLD,
                    Z_SPEED_MINUS_THRESHOLD, Z_AXIS_THRESHOLD, RIGHT, LEFT, 0);
  if (cur_state && cur_state == linear_state) {
    // printf("Performing %u\n", cur_state);
    double distance = Z_SPEED_DISTANCE;
    double thres = Z_SPEED_PLUS_THRESHOLD;
    char *name = "UP";
    switch (cur_state) {
    case UP:
      distance = Z_SPEED_DISTANCE;
      thres = Z_SPEED_PLUS_THRESHOLD;
      name = "UP";
      break;
    case DOWN:
      distance = Z_SPEED_DISTANCE;
      thres = Z_SPEED_MINUS_THRESHOLD;
      name = "DOWN";
      break;
    }

    linear_time += speed_module * delta_time / thres;
    if (linear_time >= distance) {
      printf("Recognized %s\n", name);
      linear_state = 0;
      linear_time = 0;
      action_done = true;
      return cur_state;
    }
  } else {
    linear_state = cur_state;
    linear_time = 0.0;
  }
  return 0;
}

// int jedi_analizeSpeed(double sped[3], uint16_t index) { return 0; }

uint32_t analyze_speed(double v_axis, double v_sum, double ths_plus,
                       double ths_minus, double axis_rate, uint32_t ret_plus,
                       uint32_t ret_minus, uint32_t ret_none) {
  if (abs(v_axis) > v_sum * axis_rate) {
    if (v_axis > ths_plus)
      return (ret_plus);
    if (v_axis < -ths_minus)
      return (ret_minus);
  }
  return (ret_none);
}

void jedi_initQuat() {
  // global_quat = (quat *)malloc(sizeof(quat));
  global_quat.W = 1;
  global_quat.X = 0;
  global_quat.Y = 0;
  global_quat.Z = 0;
}
