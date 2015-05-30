#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "quaternion.h"

// Functions for test purposes
void assertQuatByPtr(quat *got, quat expected) {
  bool successed = true;
  if (fabs(got->W - expected.W) > 1e-2) {
    fprintf(stderr, "Expected W %3.2f, but had %3.2f\n", expected.W, got->W);
    successed = false;
  }
  if (fabs(got->X - expected.X) > 1e-2) {
    fprintf(stderr, "Expected X %3.2f, but had %3.2f\n", expected.X, got->X);
    successed = false;
  }
  if (fabs(got->Y - expected.Y) > 1e-2) {
    fprintf(stderr, "Expected Y %3.2f, but had %3.2f\n", expected.Y, got->Y);
    successed = false;
  }
  if (fabs(got->Z - expected.Z) > 1e-2) {
    fprintf(stderr, "Expected Z %3.2f, but had %3.2f\n", expected.Z, got->Z);
    successed = false;
  }
  if (successed) {
    printf("Test passed\n");
  }
}

void assertQuat(quat got, quat expected) { assertQuatByPtr(&got, expected); }

// Quaternion test case list
void test_creationFromVecAndAngle();
void test_creationFromGravityVec();
void test_creationFromAngles();

void test_multiply();

void test_getAngles();

int main(int argc, char const *argv[]) {
  printf("Quaternion test cases\n");
  // Creation tests
  test_creationFromVecAndAngle();
  test_creationFromGravityVec();
  test_creationFromAngles();
  // Operations tests
  test_multiply();
  // test get Angles
  test_getAngles();
  return 0;
}

void test_creationFromVecAndAngle() {
  printf("Testing quat creation FromVecAndAngle\n");
  double vec[3] = {0, 0, 0};
  quat *first = quat_fromVectorAndAngle(2 * M_PI, vec);
  quat expected = {-1, 0, 0, 0};
  assertQuatByPtr(first, expected);
  free(first);

  vec[0] = 1;
  first = quat_fromVectorAndAngle(M_PI, vec);
  expected.W = 0;
  expected.X = 1;
  assertQuatByPtr(first, expected);
  free(first);

  vec[0] = 0;
  vec[1] = 1;
  first = quat_fromVectorAndAngle(M_PI, vec);
  expected.X = 0;
  expected.Y = 1;
  assertQuatByPtr(first, expected);
  free(first);
}

void test_creationFromGravityVec() {
  printf("Testing quat creation FromGravityVec\n");

  double vec[3] = {0, 0, 1};
  quat *first = quat_fromGravityVector(vec);
  quat expected = {1, 0, 0, 0};
  assertQuatByPtr(first, expected);
  free(first);

  vec[2] = 0;
  vec[0] = 1;
  first = quat_fromGravityVector(vec);
  expected.W = 0.71;
  expected.Y = 0.71;
  assertQuatByPtr(first, expected);
  free(first);

  vec[0] = 0;
  vec[1] = 1;
  first = quat_fromGravityVector(vec);
  expected.W = 0.71;
  expected.X = -0.71;
  expected.Y = 0;
  assertQuatByPtr(first, expected);
  free(first);
}

void test_creationFromAngles() {
  printf("Testing quat creation FromGravityVec\n");

  double angles[3] = {M_PI, 0, 0};
  quat *first = quat_fromAngles(angles);
  quat expected = {0, 1, 0, 0};
  assertQuatByPtr(first, expected);
  free(first);

  angles[0] = 0;
  angles[1] = M_PI;
  first = quat_fromAngles(angles);
  expected.X = 0;
  expected.Y = 1;
  expected.Z = 0;
  assertQuatByPtr(first, expected);
  free(first);
}

void test_multiply() {
  printf("Testing multiplication \n");
  double angles[3] = {M_PI, 0, 0};
  quat *first = quat_fromAngles(angles), *second;
  angles[0] = 0;
  angles[1] = M_PI;
  second = quat_fromAngles(angles);
  angles[0] = M_PI;
  quat expected = {0, 0, 0, 1};
  assertQuatByPtr(quat_multiply(first, second), expected);
  free(first);
  free(second);
}

void test_divide() {
  quat *first, *second;
  first = (quat *)malloc(sizeof(quat));
  first->W = 1;
  first->X = 0;
  first->Y = 1;
  first->Z = 1;

  second = (quat *)malloc(sizeof(quat));
  second->W = 1;
  second->X = 0;
  second->Y = 0;
  second->Z = 1;

  quat expected = {0, 0, 1, 0};
  assertQuatByPtr(quat_divide(first, second), expected);
  free(first);
  free(second);
}

void test_getAngles() {
  printf("Testing angles\n");
  double vec[3] = {M_PI / 2, M_PI / 3, M_PI / 2};
  quat *first = quat_fromAngles(vec);
  vec3 tst = quat_get3EulerAngles(first);
  fprintf(stderr, "Angles (Expected 90 60 90): %3.0f %3.0f %3.0f\n", tst.X,
          tst.Y, tst.Z);
}
