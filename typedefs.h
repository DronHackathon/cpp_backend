#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265359
#endif

// Codes of movements directions
#define UP 0x01
#define DOWN 0x02
#define RIGHT 0x03
#define LEFT 0x04
#define PUSH 0x05
#define PULL 0x06

#define DRONE_LEFT 0x10
#define DRONE_RIGHT 0x11
#define DRONE_FORW 0x12
#define DRONE_BACKW 0x13

// For linear gesture recognition
#define RECOGN_LEN 5

#define TOO_BIG_SPEED 600
#define IGNORE_COUNTS 13
#define X_SPEED_PLUS_THRESHOLD 260  // right
#define X_SPEED_MINUS_THRESHOLD 360 // left
#define X_SPEED_DISTANCE 600
#define X_AXIS_THRESHOLD 0.56
#define Y_SPEED_PLUS_THRESHOLD 220  // pull
#define Y_SPEED_MINUS_THRESHOLD 280 // push
#define Y_SPEED_DISTANCE 300
#define Y_AXIS_THRESHOLD 0.51
#define Z_SPEED_PLUS_THRESHOLD 240  // up
#define Z_SPEED_MINUS_THRESHOLD 260 // down
#define Z_SPEED_DISTANCE 600
#define Z_AXIS_THRESHOLD 0.51

// For client_ge features
#define aTr 50
#define lTr 30
#define DELAY_COUNTS_G 20

// For client_ballance
#define SPEED_THRESHOLD_B 300
#define DELAY_COUNTS_B 10

// For rotation and direction correction
#define ACC_THRESHOLD 500
#define GYRO_THRESHOLD 300
#define ZERO_SPEED_THRESHOLD 160
#define INTEGRATION_PERIOD 20
#define SPEED_LPF_TIME_CONSTANT 2000.0

#ifdef __linux__
#define VK_UP 0xff52
#define VK_DOWN 0xff54
#define VK_LEFT 0xff51
#define VK_RIGHT 0xff53

#define VK_VOLUME_UP 0x1008ff13
#define VK_VOLUME_DOWN 0x1008ff11
#define VK_VOLUME_MUTE 0x1008ff12

#define VK_MULTIMEDIA_PLAY 0x1008ff14
#define VK_MULTIMEDIA_STOP 0x1008ff15
#define VK_MULTIMEDIA_PAUSE 0x1008ff31
#define VK_MULTIMEDIA_NEXT 0x1008ff17
#define VK_MULTIMEDIA_PREV 0x1008ff16

#define VK_ADD 0xffab
#define VK_SUBTRACT 0xffad
#define VK_LSHIFT 0xffe1
#endif

// Temporaly, for *nix to be built on
// #ifndef VK_UP
// 	#define VK_UP UP
// #endif
// #ifndef VK_DOWN
// 	#define VK_DOWN DOWN
// #endif
// #ifndef VK_LEFT
// 	#define VK_LEFT LEFT
// #endif
// #ifndef VK_RIGHT
// 	#define VK_RIGHT RIGHT
// #endif
// #ifndef VK_VOLUME_UP
// 	#define VK_VOLUME_UP 0x06
// #endif
// #ifndef VK_VOLUME_DOWN
// 	#define VK_VOLUME_DOWN 0x07
// #endif
// #ifndef VK_ADD
// 	#define VK_ADD 0x08
// #endif
// #ifndef VK_SUBTRACT
// 	#define VK_SUBTRACT 0x09
// #endif

#endif
#endif // CONFIG_H
