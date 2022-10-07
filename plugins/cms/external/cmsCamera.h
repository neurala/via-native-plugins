#pragma once
#include <string>

#define TriggerRising 0
#define TriggerFalling 1
#define TriggerSoftware 2

#define ShutterGlobal 0
#define ShutterRolling 1

/*! Class used to configure the camera*/
class cmsCamera {
public:

	
	//! Close the camera
	/*! 
	*/
	virtual void close() = 0;
	//! Retreiving the depth of the camera
	/*! returns the depth of the camera in bits
	*/
	virtual void getBitDepth(int* bitDepth) = 0;
	//! Setting the gain
	/*! sets the gain
	*/
	virtual void setGain(double gain) = 0;
	//! Setting the framerate
	/*! sets the framerate in frames per second
	*/
	virtual void setFrameRate(double frameRate) = 0;
	//! Setting the exposure time
	/*! sets the exposure time in ms
	*/
	virtual void setExposure(double exposureTime) = 0;
	//! Setting the pixel clock
	/*! sets the framerate in MHz
	*/
	virtual void setPixelClock(int pixelClock) = 0;
	//! Setting the black level
	/*! sets the black level 
	*/
	virtual void setBlackLevel(int blackLevel) = 0;
	//! Setting the trigger
	/*! Mode available : TriggerRising, TriggerFalling, TriggerSoftware
	*/
	virtual bool setTrigger(int mode) = 0;
	//! Setting the shutter
	/*! Mode available : ShutterGlobal, ShutterRolling
	*/
	virtual bool setShutter(int mode) = 0;
	//! Setting the event of new frame
	/*! 
	\param[in] function : the function which is call by the event
	*/
	virtual void setEvent(void(*function)()) = 0;
	//! Retreiving the range of the available gain
	/*!
	\param[out] min : the minimum available gain
	\param[out] max : the maximum available gain
	*/
	virtual void getGainRange(double* min, double* max) = 0;
	//! Retreiving the range of the available framerate in frames per second
	/*!
	\param[out] min : the minimum available framerate
	\param[out] max : the maximum available framerate
	*/
	virtual void getFrameRateRange(double* min, double* max) = 0;
	//! Retreiving the range of the available exposure time in ms
	/*!
	\param[out] min : the minimum available exposure time
	\param[out] max : the maximum available exposure time
	*/
	virtual void getExposureRange(double* min, double* max) = 0;
	//! Retreiving the range of the available pixel clock in MHz
	/*!
	\param[out] min : the minimum available pixel clock
	\param[out] max : the maximum available pixel clock
	*/
	virtual void getPixelClockRange(int* min, int* max) = 0;
	//! Retreiving the range of the available black level
	/*!
	\param[out] min : the minimum available black level
	\param[out] max : the maximum available black level
	*/
	virtual void getBlackLevelRange(int* min, int* max) = 0;

	//! Retreiving the current value of exposure in ms
	/*!
	\param[out] exposure : the current value of exposure
	*/
	virtual void getExposure(double* exposure) = 0;
	//! Retreiving the current value of framerate
	/*!
	\param[out] FPS : the current value of framerate
	*/
	virtual void getFrameRate(double* FPS) = 0;
	//! Retreiving the current value of pixel clock in MHz
	/*!
	\param[out] PixelClock : the current value of pixel clock
	*/
	virtual void getPixelClock(int* PixelClock) = 0;
	//! Retreiving the current value of black level
	/*!
	\param[out] BlackLevel : the current value of black level
	*/
	virtual void getBlackLevel(int* BlackLevel) = 0;
	//! Retreiving the serial number of the manufacturer
	/*!
	*/
	virtual std::string getSerNoCamera() = 0; 
	//! Copy the raw image at the adress of memory
	/*!
	*/
	virtual void copyDataRaw(char* memory, int size) = 0;
	//! Retreiving the pointer of the line
	/*!
	*/
	virtual char* getLinePtr(int y) = 0;
	//! Retreiving if the function is enable
	/*!
	*/
	virtual bool isManufacturerSensorCorrection() = 0;
	//! Setting boolean which control the manufacturer sensor correction
	/*! \param[in] state : if true, the manufacturer sensor is activated
	*/
	virtual void setManufacturerSensorCorrection(bool state) = 0;

};