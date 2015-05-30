#ifndef FORCE_API_H
#define FORCE_API_H

/**
 * @desc       force_recognize() - recognizes data got from Force
 * @return     Code of gesture
 */
PyObject *force_recognize(PyObject *self, PyObject *args);

/**
 * @desc       force_open() - Opens force on port
 * @param      port, string
 * @return     nothing
 */
PyObject *force_open(PyObject *self, PyObject *args);

/**
 * @desc       force_close() - Closes port
 * @param      no arguments
 * @return     nothing
 */
PyObject *force_close(PyObject *self, PyObject *args);

/**
 * @desc       force_get_gyro() - returns tuple(s) of gyro data
 * @param      no arguments
 * @return     - 3-dimension tuple if one amount of data available
 *             - tuple of 3-dimension tuples if more than one amount of data
 *             	available
 */
PyObject *force_get_gyro(PyObject *self, PyObject *args);

/**
 * @desc       force_get_accel() - returns tuple(s) of accelerometer data
 * @param      no arguments
 * @return     - 3-dimension tuple if one amount of data available
 *             - tuple of 3-dimension tuples if more than one amount of data
 *             	available
 */
PyObject *force_get_accel(PyObject *self, PyObject *args);

/**
 * @desc       force_gyro_count() - returns count of gyro data available
 * @param      no arguments
 * @return     count of data available
 */
PyObject *force_gyro_count(PyObject *self, PyObject *args);

/**
 * @desc       force_gyro_count() - returns count of accelerometer data
 *             								available
 * @param      no arguments
 * @return     count of data available
 */
PyObject *force_accel_count(PyObject *self, PyObject *args);

#endif
