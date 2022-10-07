#pragma once

#include <vector>
#include "cmsManagementXML.h"


using namespace std;

/*! Class used to managed the informations of the camera*/
class cmsInfos
{

public:
	//! Standard contructor
	/*! Use with unknown camera
	*/
	__declspec(dllexport) cmsInfos();
	
	//! Constructor link to a specified SN camera
	/*! 
		\param[in] electronicSerialNumber serial number of camera constructor
	*/ 
	__declspec(dllexport) cmsInfos(std::string* electronicSerialNumber);

	//! Standard destructor
	/*!
	*/
	__declspec(dllexport) ~cmsInfos();

	//! Retreiving number of channels
	/*!
		\return number of channels
	*/
	__declspec(dllexport) int		getNbChannels() { return this->_nbChannels; }
	
	//! Retreiving the column offset
	/*!
	\return column offset use to retreive channels
	*/
	__declspec(dllexport) int		getOffsetColumn() { return this->_offsetColumn; }

	//! Retreiving the row offset
	/*!
	\return raw offset use to retreive channels
	*/
	__declspec(dllexport) int		getOffsetRow() { return this->_offsetRow; }

	//! Retreiving the camera serial number (manufacturer)
	/*!
	\return serial number (manufacturer)
	*/
	__declspec(dllexport) string	getSNManufacturer() { return this->_SNManufacturer; }

	//! Retreiving the camera serial number (CMS series)
	/*!
	\return serial number (CMS series)
	*/
	__declspec(dllexport) string	getSNCMS() { return this->_SNCMS; }

	//! Retreiving the camera type
	/*!
	\return type , can be for example CMS-C1-S-EVR1M-USB3 or CMS-C1-V-EVR1M-USB3
	*/
	__declspec(dllexport) string	getTypeCamera() { return this->_typeCamera; }

	//! Retreiving the size of the raw image
	/*!
	\return 2 dimensions tables sizeX, sizeY
	*/
	__declspec(dllexport) int*	getSizeImage() { return this->_sizeImage; }
	
	//! Retreiving the size of the channels images
	/*!
	\return 2 dimensions tables sizeX, sizeY
	*/
	__declspec(dllexport) int*	getSizeImageChannel() { return this->_sizeImageChannel; }

	//! Retreiving the size of the macopixel
	/*!
	\return 2 dimensions tables sizeX, sizeY
	*/
	__declspec(dllexport) int*	getSizeMacroPixel() { return this->_sizeMacroPixel; }

	//! Retreiving the order of channels displaying
	/*!
	\return number of the channel to display
	*/
	__declspec(dllexport) vector<int>	getDisplayChannelOrder() { return this->_displayChannelOrder; }

	//! Retreiving matrix use to calculate the color image
	/*!
	\return (n,3) matrix to transform the n channels on 3 color channels (RGB)
	*/
	__declspec(dllexport) vector<vector<double>>	getMatCoefficientsColor() { return this->_matCoefficientsColor; }

	//! Retreiving the centering of the filter channels
	/*!
	\return centering in nanometer for each filter channels
	*/
	__declspec(dllexport) vector<int>	getCentering();

	//! Retreiving the FWHM of the filter channels
	/*!
	\return FWHM in nanometer for each filter channels
	*/
	__declspec(dllexport) vector<int>	getFWHM();

	//! Retreiving the wavelength order of the filter channels
	/*!
	\return permit to rearrange the channels in croissant order
	*/
	__declspec(dllexport) vector<int>	getWavelengthOrder();

	//! Retreiving the crosstalk correction matrix
	/*!
	\return the matrix use to correct the crosstalk
	*/
	__declspec(dllexport) vector<vector<double>> getCoefficientCrosstalk();


	//! Retreiving the cmsManagementXML of the camera
	/*!
	\return cmsManagementXML of the camera (information getting from xml file linked to the camera)
	*/
	__declspec(dllexport) cmsManagementXML*		getCmsManagementXML() { return this->_cmsManagementXML; }



private:

	int _nbChannels; 
	int _offsetColumn;                                                                                                        
	int _offsetRow; 
	int	_sizeImage[2];
	int	_sizeImageChannel[2];
	int	_sizeMacroPixel[2];
	string _SNManufacturer;
	string _SNCMS;
	string _typeCamera;
	string _caracCam[15];
	cmsManagementXML*	_cmsManagementXML;
	vector<int>				_displayChannelOrder;
	vector<vector<double>>	_matCoefficientsColor;
	vector<int> _centering;
	vector<int> _FWHM;
	vector<int> _wavelengthOrder;
	vector<vector<double>>	_matCoefficientsCrosstalk;
};

