// #include <cstdio>
//#include <cstring>
//#include <cmath>
//#include <algorithm>
#include <Python.h>
#include "force.h"
#include "force_api.h"

#include "recognizer.cpp"

#ifdef _WIN32
#include <windows.h>
#define usleep(x) Sleep(x / 1000)
#else
#include <signal.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

static ForceBufferStates buffer_states;
static PyObject *ForceError;
static ForceInstance *force;

static PyMethodDef ForceMethods[] = {
    {"open", force_open, METH_VARARGS,
     "Opens port. Usage Force.open(port_name)"},
    {"close", force_close, METH_NOARGS, "Closes port. Usage Force.close()"},

    {"gyro_count", force_gyro_count, METH_NOARGS,
     "Returns count of collected gyro data"},
    {"accel_count", force_accel_count, METH_NOARGS,
     "Returns count of collected accel data"},

    {"get_gyro", force_get_gyro, METH_NOARGS,
     "Returns list of all collected gyro data "},
    {"get_accel", force_get_accel, METH_NOARGS,
     "Returns list of all collected accel data "},
    {"recognize", force_recognize, METH_NOARGS,
     "Recognizes gesture by itself and return code of recognized state"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

PyMODINIT_FUNC initforce_api(void) {
  PyObject *m;
  m = Py_InitModule("force_api", ForceMethods);
  if (!m)
    return;

  ForceError = PyErr_NewException("ForceApi.Error", NULL, NULL);
  Py_INCREF(ForceError);
  PyModule_AddObject(m, "error", ForceError);
}

PyObject *force_recognize(PyObject *self, PyObject *args) {
  int result = 0;

  return Py_BuildValue("i", result);
}

PyObject *force_open(PyObject *self, PyObject *args) {
  char *portName;
  if (!PyArg_ParseTuple(args, "s", &portName)) {
    PyErr_SetString(ForceError, "Error: Invalid usage of function");
    return NULL;
  }

  uint32_t major, minor;
  Force_GetLibraryVersion(&major, &minor);
  if (major != 0) {
    PyErr_SetString(ForceError, "Error: libforce v0.x is only supported");
    return NULL;
  }

  // open the device
  force = Force_Open(portName);
  if (!force) {
    PyErr_SetString(ForceError, "Error: could not open the device");
    return NULL;
  }

  Force_SetAccelConfig(force, LSM330DL_A_RANGE_4G, LSM330DL_A_DATA_RATE_400HZ,
                       0);
  Force_SetGyroConfig(force, LSM330DL_G_RANGE_2000DPS,
                      LSM330DL_G_ODR_800HZ_LPF_110HZ, 0);
  Force_SetMagConfig(force, LIS3MDL_RANGE_8GS, LIS3MDL_DATA_RATE_1_25HZ, 0);

  // init the device
  if (!Force_Init(force)) {
    Force_Close(force);
    PyErr_SetString(ForceError, "Error: could not init the device");
    return NULL;
  }
  // uint32_t fw_rev, proto_rev;
  // Force_GetFirmwareVersion(force, &fw_rev, &proto_rev);
  // // printf("Connected to Force: firmware=%u, protocol=%u\n", fw_rev,
  // proto_rev);
  Py_RETURN_NONE;
} // Force_open

PyObject *force_close(PyObject *self, PyObject *args) {
  Force_Close(force);
  if (!force)
    free(force);
  Py_RETURN_NONE;
}

PyObject *force_get_gyro(PyObject *self, PyObject *args) {
  Force_Communicate(force, &buffer_states, sizeof(ForceBufferStates));
  PyObject *list = PyList_New(0);
  ForceMotionData motion;
  while (buffer_states.GyroDataCount--) {
    if (Force_GetGyroData(force, &motion)) {
      PyObject *temp_list = PyList_New(0);
      PyList_Append(temp_list, Py_BuildValue("i", motion.DataX));
      PyList_Append(temp_list, Py_BuildValue("i", motion.DataY));
      PyList_Append(temp_list, Py_BuildValue("i", motion.DataZ));
      PyList_Append(list, temp_list);
    }
  }
  return list;
}

PyObject *force_get_accel(PyObject *self, PyObject *args) {
  Force_Communicate(force, &buffer_states, sizeof(ForceBufferStates));
  PyObject *list = PyList_New(0);
  ForceMotionData motion;
  while (buffer_states.AccelDataCount--) {
    if (Force_GetAccelData(force, &motion)) {
      PyObject *temp_list = PyList_New(0);
      PyList_Append(temp_list, Py_BuildValue("i", motion.DataX));
      PyList_Append(temp_list, Py_BuildValue("i", motion.DataY));
      PyList_Append(temp_list, Py_BuildValue("i", motion.DataZ));
      PyList_Append(list, temp_list);
    }
  }
  return list;
}

PyObject *force_gyro_count(PyObject *self, PyObject *args) {
  Force_Communicate(force, &buffer_states, sizeof(ForceBufferStates));
  return Py_BuildValue("i", buffer_states.GyroDataCount);
}

PyObject *force_accel_count(PyObject *self, PyObject *args) {
  Force_Communicate(force, &buffer_states, sizeof(ForceBufferStates));
  return Py_BuildValue("i", buffer_states.AccelDataCount);
}

#ifdef __cplusplus
}
#endif
