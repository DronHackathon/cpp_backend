// #include <cstdio>
//#include <cstring>
//#include <cmath>
//#include <algorithm>
#include <Python.h>
#include "force.h"

#ifdef _WIN32
#include <windows.h>
#define usleep(x) Sleep(x / 1000)
#else
#include <signal.h>
#endif

static ForceBufferStates buffer_states;
static PyObject *ClientError;
static ForceInstance *force;

PyObject *client_open(PyObject *self, PyObject *args) {
  char *portName;
  if (!PyArg_ParseTuple(args, "s", &portName)) {
    PyErr_SetString(ClientError, "Error: Invalid usage of function");
    return NULL;
  }

  uint32_t major, minor;
  Force_GetLibraryVersion(&major, &minor);
  if (major != 0) {
    PyErr_SetString(ClientError, "Error: libforce v0.x is only supported");
    return NULL;
  }

  // open the device
  force = Force_Open(portName);
  if (!force) {
    PyErr_SetString(ClientError, "Error: could not open the device");
    return NULL;
  }

  // init the device
  if (!Force_Init(force)) {
    Force_Close(force);
    PyErr_SetString(ClientError, "Error: could not init the device");
    return NULL;
  }
  // uint32_t fw_rev, proto_rev;
  // Force_GetFirmwareVersion(force, &fw_rev, &proto_rev);
  // // printf("Connected to Force: firmware=%u, protocol=%u\n", fw_rev,
  // proto_rev);
  Py_RETURN_NONE;
} // client_open

PyObject *client_close(PyObject *self, PyObject *args) {
  Force_Close(force);
  if (!force)
    free(force);
  Py_RETURN_NONE;
}

PyObject *client_get_gyro(PyObject *self, PyObject *args) {
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
      free(temp_list);
    }
  }
  return list;
}

PyObject *client_get_accel(PyObject *self, PyObject *args) {
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
      free(temp_list);
    }
  }
  return list;
}

PyObject *client_gyro_count(PyObject *self, PyObject *args) {
  Force_Communicate(force, &buffer_states, sizeof(ForceBufferStates));
  return Py_BuildValue("i", buffer_states.GyroDataCount);
}

PyObject *client_accel_count(PyObject *self, PyObject *args) {
  Force_Communicate(force, &buffer_states, sizeof(ForceBufferStates));
  return Py_BuildValue("i", buffer_states.AccelDataCount);
}

static PyMethodDef ClientMethods[] = {
    {"open", client_open, METH_VARARGS,
     "Opens port. Usage client.open(port_name)"},
    {"close", client_close, METH_NOARGS, "Closes port. Usage client.close()"},

    {"gyro_count", client_gyro_count, METH_NOARGS,
     "Returns count of collected gyro data"},
    {"accel_count", client_accel_count, METH_NOARGS,
     "Returns count of collected accel data"},

    {"get_gyro", client_get_gyro, METH_NOARGS,
     "Returns list of all collected gyro data "},
    {"get_accel", client_get_accel, METH_NOARGS,
     "Returns list of all collected accel data "},
    {NULL, NULL, 0, NULL} /* Sentinel */ co};

PyMODINIT_FUNC initforce_api(void) {
  PyObject *m;
  m = Py_InitModule("force_api", ClientMethods);
  if (!m)
    return;

  ClientError = PyErr_NewException("client.error", NULL, NULL);
  Py_INCREF(ClientError);
  PyModule_AddObject(m, "error", ClientError);
}
