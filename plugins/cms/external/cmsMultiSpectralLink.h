#pragma once
#include "cmsImages.h"
#include "cmsInfos.h"
#include "cmsCamera.h"
#include <filesystem>
#include <vector>

//#include <PvSampleUtils.h>
//#include "PvSampleUtils.h"

#define IDS 0
#define ISG 1

#define LIVE 124
#define REPLAY 125

#define MAXBITDEPTH -1

/*! Main class used to link all the objects*/
class cmsMultiSpectralLink
{
public:

	//! Standard constructor
	__declspec(dllexport) cmsMultiSpectralLink();
	
	//!  Returns the object image
	/*!	
	\param[in] SerialNumber : the serialnumber of the camera
	\param[in] mode : LIVE or REPLAY
	\param[in] bitDepth : configures the depth of the camera
	*/
	__declspec(dllexport) cmsMultiSpectralLink(std::string SerialNumber, int mode = LIVE, int bitDepth = MAXBITDEPTH);
	
	//! Standard destructor
	__declspec(dllexport) ~cmsMultiSpectralLink();
	
	//!  Returns the list of camera's serial number which are connected
	/*!
	*/
	__declspec(dllexport) static std::vector<std::string>	getCameraList();

	//! Returns the list of cameras whose sensor is a snake.
	/*!
	*/
	__declspec(dllexport) static std::vector<std::string>	getCameraListSnake();

	//!  Adding a new image in the image's list
	__declspec(dllexport) void			addCmsImages() { this->_cmsImages.push_back(new cmsImages(this->_cmsInfos, this->_cmsCamera, this->bitDepth)); }

	//!  Returns the object image
	/*!	Return the cmsImages corresponding to the index
		\param[in] ID index in image's list
	*/
	__declspec(dllexport) cmsImages*		getCmsImages(int ID) { return this->_cmsImages.at(ID); }
	
	//!  Returns the object cmsInfos corresponding of the current camera
	__declspec(dllexport) cmsInfos*		getCmsInfos() { return this->_cmsInfos; }
	
	//!  Returns the object cmsCamera
	__declspec(dllexport) cmsCamera*		getCmsCamera() { return this->_cmsCamera; }


private:

	cmsInfos*			_cmsInfos;
	cmsCamera*			_cmsCamera;
	vector<cmsImages*>	_cmsImages;
	int bitDepth;

	

};
