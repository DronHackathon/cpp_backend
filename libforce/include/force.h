
#ifndef __LIBFORCE__
#define __LIBFORCE__

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define  LIBFORCE_VERSION_MAJOR				0
#define  LIBFORCE_VERSION_MINOR				6

#define  FORCE_SENSOR_STATUS_NOT_SUPPORTED	0
#define  FORCE_SENSOR_STATUS_OKAY			1
#define  FORCE_SENSOR_STATUS_FAIL			2

/*****************************************************************
 ***                 PARAMETER DESCRIPTION 	                   ***
 *****************************************************************/

#define  ADXL345_RANGE_4G				0x01
#define  ADXL345_DATA_RATE_50HZ			0x09

#define  ITG3205_DATA_RATE_50HZ			19			// 1000 / (19 + 1) = 50
#define  ITG3205_LPF_20HZ_INT_1KHZ		0x04

#define  LSM330DL_A_DATA_RATE_SLEEP		0x00
#define  LSM330DL_A_DATA_RATE_1HZ		0x01
#define  LSM330DL_A_DATA_RATE_10HZ		0x02
#define  LSM330DL_A_DATA_RATE_25HZ		0x03
#define  LSM330DL_A_DATA_RATE_50HZ		0x04
#define  LSM330DL_A_DATA_RATE_100HZ		0x05
#define  LSM330DL_A_DATA_RATE_200HZ		0x06
#define  LSM330DL_A_DATA_RATE_400HZ		0x07
#define  LSM330DL_A_DATA_RATE_1344HZ	0x09

#define  LSM330DL_A_RANGE_2G			0x00
#define  LSM330DL_A_RANGE_4G			0x01
#define  LSM330DL_A_RANGE_8G			0x02
#define  LSM330DL_A_RANGE_16G			0x03

#define  LSM330DL_G_ODR_100HZ_LPF_12HZ	0x00
#define  LSM330DL_G_ODR_100HZ_LPF_25HZ	0x01
#define  LSM330DL_G_ODR_200HZ_LPF_12HZ	0x04
#define  LSM330DL_G_ODR_200HZ_LPF_25HZ	0x05
#define  LSM330DL_G_ODR_200HZ_LPF_50HZ	0x06
#define  LSM330DL_G_ODR_200HZ_LPF_70HZ	0x07
#define  LSM330DL_G_ODR_400HZ_LPF_20HZ	0x08
#define  LSM330DL_G_ODR_400HZ_LPF_25HZ	0x09
#define  LSM330DL_G_ODR_400HZ_LPF_50HZ	0x0A
#define  LSM330DL_G_ODR_400HZ_LPF_110HZ	0x0B
#define  LSM330DL_G_ODR_800HZ_LPF_30HZ	0x0C
#define  LSM330DL_G_ODR_800HZ_LPF_35HZ	0x0D
#define  LSM330DL_G_ODR_800HZ_LPF_50HZ	0x0E
#define  LSM330DL_G_ODR_800HZ_LPF_110HZ	0x0F

#define  LSM330DL_G_RANGE_250DPS		0x00
#define  LSM330DL_G_RANGE_500DPS		0x01
#define  LSM330DL_G_RANGE_2000DPS		0x02

#define  LIS3MDL_DATA_RATE_0_625HZ		0x00
#define  LIS3MDL_DATA_RATE_1_25HZ		0x01
#define  LIS3MDL_DATA_RATE_2_5HZ		0x02
#define  LIS3MDL_DATA_RATE_5HZ			0x03
#define  LIS3MDL_DATA_RATE_10HZ			0x04
#define  LIS3MDL_DATA_RATE_20HZ			0x05
#define  LIS3MDL_DATA_RATE_40HZ			0x06
#define  LIS3MDL_DATA_RATE_80HZ			0x07

#define  LIS3MDL_RANGE_4GS				0x00
#define  LIS3MDL_RANGE_8GS				0x01
#define  LIS3MDL_RANGE_12GS				0x02
#define  LIS3MDL_RANGE_16GS				0x03

/*****************************************************************
 ***             		    STRUCTURES 	       		           ***
 *****************************************************************/

typedef struct
{
	uint32_t AccelDataCount;
	uint32_t GyroDataCount;
	uint32_t BytesLost;			// number of discarded received bytes during this Force_Communicate()
	uint32_t MagDataCount;
} ForceBufferStates;

typedef struct
{
	uint32_t MeasTime;
	int32_t DataX;
	int32_t DataY;
	int32_t DataZ;
} ForceMotionData;

typedef struct
{
	uint8_t AccelStatus;
	uint8_t GyroStatus;
	uint8_t MagStatus;
} ForceSensorStatus;

#ifdef _WIN32
	#ifdef BUILDING_LIBFORCE
		#define  LIBFORCE_API  __stdcall __declspec(dllexport)
	#else
		#define  LIBFORCE_API  __stdcall __declspec(dllimport)
	#endif
#else
	#define  LIBFORCE_API
#endif

#ifdef BUILDING_LIBFORCE
	#include "force_int.hpp"
#else
	// The following structure is internal, and only pointers go outside the library
	typedef void ForceInstance;
#endif

/*****************************************************************
 ***                 EXPORTED FUNCTIONS 	                   ***
 *****************************************************************/

// Essentials
ForceInstance* LIBFORCE_API Force_Open(char* port_name);
bool LIBFORCE_API Force_Init(ForceInstance* handle);
bool LIBFORCE_API Force_Communicate(ForceInstance* handle, ForceBufferStates* pBufStOut, size_t BufStSize);
void LIBFORCE_API Force_Close(ForceInstance* handle);

// Set* functions: most of these should be called between Open() and Init()
void LIBFORCE_API Force_SetConnectionTimeout(ForceInstance* handle, uint32_t timeout_ms);	// 0 = disabled
void LIBFORCE_API Force_SetAccelConfig(ForceInstance* handle,
					uint8_t range, uint8_t data_rate, uint8_t avg_2n);
void LIBFORCE_API Force_SetGyroConfig(ForceInstance* handle,
					uint8_t range, uint8_t data_rate, uint8_t avg_2n);
void LIBFORCE_API Force_SetMagConfig(ForceInstance* handle,
					uint8_t range, uint8_t data_rate, uint8_t avg_2n);

// Get* functions
bool LIBFORCE_API Force_GetLibraryVersion(uint32_t* major, uint32_t* minor);
bool LIBFORCE_API Force_GetFirmwareVersion(ForceInstance* handle, uint32_t* fw_rev, uint32_t* proto_rev);
bool LIBFORCE_API Force_GetHardwareVersion(ForceInstance* handle, uint32_t* hw_rev);
bool LIBFORCE_API Force_GetSensorStatus(ForceInstance* handle, ForceSensorStatus* pSensStOut, size_t SensStSize);
//char* LIBFORCE_API Force_GetErrorMessage();
bool LIBFORCE_API Force_GetAccelData(ForceInstance* handle, ForceMotionData* pOut);
bool LIBFORCE_API Force_GetGyroData(ForceInstance* handle, ForceMotionData* pOut);
bool LIBFORCE_API Force_GetMagData(ForceInstance* handle, ForceMotionData* pOut);

#ifdef __cplusplus
	}
#endif

#endif
