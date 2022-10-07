#pragma once
#include <opencv2\core\wimage.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>

#include "cmsInfos.h"
#include "cmsCamera.h"

#include <iostream>
#include <vector>
#include <algorithm>

#define GRAYColor 0
#define RGBSColor 1
#define springColor 2
#define summerColor 3
#define rainbowColor 4
#define hotColor 5
#define copperColor 6
#define coolColor 7
#define coolColor8 8

#define algReferenceSpectral 0
#define algNDVI 1
#define algContrastInterChannel 2

#define MONOROI 1
#define MULTIROI 4
#define NOROI 0

/*! Class used to stack and manipulated multispectral images*/
class cmsImages
{
public:
	//! Constructor
	/*! \param[in] cmsInfos : object containing informations of the camera
	\param[in] cmsCamera : object that configures the camera
	\param[in] bitDepth : configures the depth of the camera
	*/
	__declspec(dllexport) cmsImages(cmsInfos* cmsInfos, cmsCamera* cmsCamera, int bitDepth);

	//! Standard destructor
	__declspec(dllexport) ~cmsImages();

	//! Setting the raw image
	/*! sets the data of the raw image in 16 bits
	*/
	__declspec(dllexport) void setImageRaw();

	//! Setting the raw image
	/*! sets the data of the raw image in 16 bits
	\param[in] data : the data of the raw image
	\param[in] sizeX : size horizontal of the image
	\param[in] sizeY : size vertical of the image
	*/
	__declspec(dllexport) void setImageRaw(char* data, int sizeX, int sizeY);
	//! Setting the raw image
	/*! sets the data of the raw image in 8 bits
		\param[in] data : the data of the raw image
		\param[in] sizeX : size horizontal of the image
		\param[in] sizeY : size vertical of the image
	*/
	__declspec(dllexport) void setImageRaw8(char* data, int sizeX, int sizeY);

	//! Setting boolean which control the crosstalk correction
	/*! \param[in] state : if true, the crosstalk correction is activated
	*/
	__declspec(dllexport) void setStateCrosstalkCorrection(bool state);

	//! Setting the maximum value can be achieve by a pixel
	/*! \param[in] pixMax : maximal value
	*/
	__declspec(dllexport) void setValueMaximumPixel(int pixMax);

	//! Setting the cmsInfos
	/*! This object contains informations about the CMS camera
		\param[in] cmsInfos : object containing informations of the camera
	*/
	__declspec(dllexport) void setCmsInfos(cmsInfos* cmsInfos);

	//! Setting the size of the raw image
	/*! 
		\param[in] sizeX : horizontal size of the image
		\param[in] sizeY : vertical size of the image
	*/
	__declspec(dllexport) void setSizeImage(int sizeX, int sizeY);

	//! Setting the size of channel image
	/*!
		\param[in] sizeX : horizontal size of the channel
		\param[in] sizeY : vertical size of the channel
	*/
	__declspec(dllexport) void setSizeImageChannel(int sizeX, int sizeY);


	//! Setting the size of macropixel
	/*!
		\param[in] sizeX : horizontal size of the macropixel
		\param[in] sizeY : vertical size of the macropixel
	*/
	__declspec(dllexport) void setSizeMacroPixel(int sizeX, int sizeY);

	//! Defines coefficients for the white
	/*! With coordinates of an area of the image with white, coefficients are automatically determined.
		\param[in] firstX : X coordinate of first pixel 
		\param[in] firstY : Y coordinate of first pixel 
		\param[in] secondX : X coordinate of second pixel 
		\param[in] secondY : Y coordinate of second pixel 
	*/
	__declspec(dllexport) void setCoefficientsWhiteColorImage(int firstX, int firstY, int secondX, int secondY);

	//! Defines coefficients for the white
	/*! 
	\param[in] arrayValue: array of value
	*/
	__declspec(dllexport) void setCoefficientsWhiteColorImage(double * arrayValue);

	//! Defines coefficients for the white reference
	/*! With coordinates of an area of the image considered as a true white, coefficients are automatically determined.
		\param[in] firstX : X coordinate of first pixel 
		\param[in] firstY : Y coordinate of first pixel 
		\param[in] secondX : X coordinate of second pixel 
		\param[in] secondY : Y coordinate of second pixel 
	*/
	__declspec(dllexport) void setCoefficientsWhiteReference(int firstX, int firstY, int secondX, int secondY);
	
	//! Defines coefficients for the white reference
	/*!
		\param[in] arrayValue : array of value
	*/
	__declspec(dllexport) void setCoefficientsWhiteReference(double * arrayValue);

	//! Resets of white coefficients
	__declspec(dllexport) void resetCoefficientsWhiteColorImage();

	//! Resets of the white reference coefficients
	__declspec(dllexport) void resetCoefficientsWhiteReference();

	//! Returns the raw image
	/*! \return raw image (camera full depth)
	*/
	__declspec(dllexport) IplImage*	getImageRaw();

	//! Returns the image raw to display
	/*! Needs to be set with setImageRaw()
	\return raw image (8 bits)
	*/
	__declspec(dllexport) IplImage*	getImageRawDisplay();

	//! Returns the multispectral cube
	/*! Needs to be calculated by calcImageMultisprectralCube() 
	\return multispectral cube (camera full depth)
	*/
	__declspec(dllexport) IplImage*	getImageMultispectralCube();

	//! Returns a visualization of the multispectral cube
	/*!Needs to be calculated by calcImageMultisprectralMosaicDisplay()
	  \n The image is composed by a mosaic of the channels
		\return a mosaic channels image (8 bits)
	*/
	__declspec(dllexport) IplImage*	getImageMultispectralMosaicDisplay();

	//! Returns the RGB image display
	/*! Needs to be calculated by calcImageRGB()
	\return BGR image (8bits)
	*/
	__declspec(dllexport) IplImage*	getImageRGBDisplay();

	//! Returns the BGR image display
	/*! Needs to be calculated by calcImageBGR() 
		\return BGR image (8bits)
	*/
	__declspec(dllexport) IplImage*	getImageBGRDisplay();
	
	//! Returns the RGB image (camera full depth)
	/*! Needs to be calculated by calcImageRGB() 
		\return RGB image (camera full depth)
	*/
	__declspec(dllexport) IplImage*	getImageRGB();

	//! Returns the BGR image (camera full depth)
	/*! Needs to be calculated by calcImageBGR() 
		\return BGR image (camera full depth)
	*/
	__declspec(dllexport) IplImage*	getImageBGR();

	//! Returns the indexed channel image (camera full depth)
	/*! Needs to be calculated by calcImageMultisprectralCube() 
		\param[in] ID : index of the channel image in the list 
		\return channel image (camera full depth)
	*/
	__declspec(dllexport) IplImage*	getImageChannel(int ID);

	//! Returns the indexed channel image to display
	/*! Needs to be calculated by calcImageMultisprectralCube()
		\param[in] ID : index of the channel image in the list 
		\return channel image (8 bits)
	*/
	__declspec(dllexport) IplImage*	getImageChannelDisplay(int ID);

	//! Returns the state of the crosstalk correction
	/*!
		\return boolean : if true, the crosstalk correction is activated
	*/
	
	__declspec(dllexport) bool		getStateCrosstalkCorrection();

	//! Returns the maximum value of a pixel
	/*!
		\return the maximum value
	*/
	__declspec(dllexport) int		getValueMaximumPixel() { return this->_pixMax; }

	//! Returns the size of the raw image in pixels
	/*!
		\return table with sizeX and sizeY
	*/
	__declspec(dllexport) int*		getSizeImage() { return this->_sizeImage; }
	

	//! Returns the bit depth of the raw image
	/*!
	\return the bit depth in int
	*/
	__declspec(dllexport) int		getBitDepth() { return this->_bitDepth; }

	//! Returns the size of the channel images in pixels
	/*!
		\return table with sizeX and sizeY
	*/
	__declspec(dllexport) int*		getSizeImageChannel() { return this->_sizeImageChannel; }

	//! Returns the size of the macropixel in raw pixels
	/*!
		\return table with sizeX and sizeY
	*/
	__declspec(dllexport) int*		getSizeMacroPixel() { return this->_sizeMacroPixel; }

	//! Returns coefficients for white reference
	/*!
		\return table with the nine coefficients
	*/
	__declspec(dllexport) double*	getCoefficientsWhiteReference() { return this->_coefficientsChannelNorm; }

	//! Returns coefficients for color image white
	/*!
	\return table with the three coefficients
	*/
	__declspec(dllexport) double*	getCoefficientsWhiteColorImage() { return this->_coefficientsChannelWhite; }

	//! Returns values for a specific pixel
	/*!
		\param[in] x : coordinate of the pixel 
		\param[in] y : coordinate of the pixel 
		\return table with the nine values of the pixel
	*/
	__declspec(dllexport) int*		getPixelValue(int x, int y);

	//! Returns mean values of an image area
	/*! Mean value is calculated in the rectangle defined by (firstX,firstY) and (secondX,secondY).
		\param[in] firstX : X coordinate of first pixel 
		\param[in] firstY : Y coordinate of first pixel 
		\param[in] secondX : X coordinate of second pixel 
		\param[in] secondY : Y coordinate of second pixel 
		\return table with the nine mean values of the area
	*/
	__declspec(dllexport) double* getPixelValue(int firstX, int firstY, int secondX, int secondY);

	//! Computing of the raw image display (8 bits)
	__declspec(dllexport) void calcImageRawDisplay();

	//! Computing the multispectral cube (camera full depth)
	__declspec(dllexport) void calcImageMultisprectralCube();

	//! Computing the multispectral cube in a mosaic(8 bits)
	__declspec(dllexport) void calcImageMultisprectralMosaicDisplay(); 

	//! Computing of the RGB and BGR images (camera full depth and 8 bits)
	__declspec(dllexport) void calcImageColor(); 

	//! Returns of the processed image to display
	/*! Needs to be calculated by calcImageProcessed(double upLimit, double downLimit, double Gamma, int color, int algChoice)
		\return processed image (8bits)
	*/
	__declspec(dllexport) IplImage*	getImageProcessedDisplay();


	//! Returns of the processed image (camera full depth)
	/*! Needs to be calculated by calcImageProcessed(double upLimit, double downLimit, double Gamma, int color, int algChoice)
		\return processed image (camera full depth)
	*/
	__declspec(dllexport) IplImage*	getImageProcessed();

	//! Computing of the processed image
	/*! The several coefficients are used to control the LUT
		\param[in]  upLimit : Up limit of the LUT
		\param[in]  downLimit : Down limit of the LUT
		\param[in]  Gamma
		\param[in]  color : 0 for gray scale, 1 for RGB saturated
		\param[in]  algChoice
		\param[in]  offsetCompensation 
	*/
	__declspec(dllexport) void calcImageProcessed(double upLimit, double downLimit, double Gamma, int color, int algChoice, int offsetCompensation);

	//! Setting the coefficients for spectral reference
	/*! The coefficients are automatically calculated from the rectangle defined by (firstX,firstY) and (secondX,secondY).
		\param[in] firstX : X coordinate of first pixel 
		\param[in] firstY : Y coordinate of first pixel 
		\param[in] secondX : X coordinate of second pixel 
		\param[in] secondY : Y coordinate of second pixel 
	*/
	__declspec(dllexport) void setCoefficientsSpectralReference(int firstX, int firstY, int secondX, int secondY);
	

	//! Resets the value of the coefficients of the spectral reference
	__declspec(dllexport) void resetCoefficientsSpectralReference();

	//! Setting the coefficients for a specific algorithm
	/*!
		\param[in] coefficients : vector of double containing the coefficients
		\param[in] IDAlgorithm : number of the specific algorithm
	*/
	__declspec(dllexport) void setCoefficientsAlgorithm(vector<double> const& coefficients, int IDAlgorithm);

	//! Returns the coefficients for a specific algorithm
	/*!
		\param[in] IDAlgorithm : number of the specific algorithm
		\return a vector of double
	*/
	__declspec(dllexport) vector<double> getCoefficientsAlgorithm(int IDAlgorithm);

	//! Resets the coefficients for a specific algorithm
	/*! 
		\param[in] IDAlgorithm : number of the specific algorithm
	*/
	__declspec(dllexport) void resetCoefficientsAlgorithm(int IDAlgorithm);

	//! Setting the image Flat field 
	/*!
		Copy the raw image in the flat field image
	*/
	__declspec(dllexport) void setFlatfield();
	//! Setting the mode of the ROI 
	/*!
		\param[in] state : The possible values are MONOROI, MULTIROI, NOROI

	*/
	__declspec(dllexport) void setModeROI(int state);
	//! Setting position of the ROI
	/*!
		\param[in] fx : the x of the first point
		\param[in] fy : the y of the first point
		\param[in] sx : the x of the second point
		\param[in] sy : the y of the second point
	*/
	__declspec(dllexport) void setPositionMonoROI(int fx, int fy, int sx, int sy);
	//! Setting position of the multi ROI
	/*!
		\param[in] x1 : the x of the first vertical axis
		\param[in] y1 : the y of the first horizontal axis
		\param[in] x2 : the x of the second vertical axis
		\param[in] y2 : the y of the second horizontal axis
		\param[in] x3 : the x of the third vertical axis
		\param[in] y3 : the y of the third horizontal axis
		\param[in] x4 : the x of the fourth vertical axis
		\param[in] y4 : the y of the fourth horizontal axis
	*/
	__declspec(dllexport) void setPositionMultiROI(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	//! Setting the image dark field 
	/*!
		Copy the raw image in the dark field image
	*/
	__declspec(dllexport) void setDarkfield();

	//!	Setting the data of the flat field image in 16 bits
	/*!	\param[in] data : the data of the flat field image
		\param[in] sizeX : size horizontal of the image
		\param[in] sizeY : size vertical of the image
	*/
	__declspec(dllexport) void setFlatfield(char* data, int sizeX, int sizeY);

	//!	Setting the data of the dark field image in 16 bits
	/*!	\param[in] data : the data of the dark field image
	\param[in] sizeX : size horizontal of the image
	\param[in] sizeY : size vertical of the image
	*/
	__declspec(dllexport) void setDarkfield(char* data, int sizeX, int sizeY);

	//! Returns the flat field image
	/*! \return raw image (camera full depth)
	*/
	__declspec(dllexport) IplImage* getFlatfield();

	//! Returns the dark field image
	/*! \return raw image (camera full depth)
	*/
	__declspec(dllexport) IplImage* getDarkfield();

	//! Resets of the flat field image
	__declspec(dllexport) void resetFlatfield();

	//! Resets of the dark field image
	__declspec(dllexport) void resetDarkfield();

	//! Setting boolean which control the flat field correction
	/*! \param[in] state : if true, the flat field correction is activated
	*/
	__declspec(dllexport) void setStateFlatfield(bool state);

	//! Returns the histogram corresponding to the image raw
	/*!
	*/
	__declspec(dllexport) void getHistogram(int* ptr, int nbElements);

	//! Setting the position of the reference exposure
	/*!
	*/
	__declspec(dllexport) void setReferenceExposure(int x1, int y1, int x2, int y2);
	//! Resets the reference exposure
	/*!
	*/
	__declspec(dllexport) void resetReferenceExposure();
	//! Computing the theoretical exposure according to the chosen area
	/*!
	*/
	__declspec(dllexport) void calcReferenceExposure();

private:
	int x1Exposure, y1Exposure, x2Exposure, y2Exposure;
	bool isAutoExposure;
	double valueAutoExposure;
	double exposure, newExpo;
	double erreur, somme_erreurs, variation_erreur, erreur_precedente;
	double Kp, Ki, Kd;
	int ROIX1, ROIY1, ROIX2, ROIY2, ROIX3, ROIY3, ROIX4, ROIY4;
	bool isMonoROI, isMultiROI;

	int*	_histogram;
	void	resetHistogram();
	int		_pixMax;
	int		_bitDepth;
	int		_bitsPerPixels;
	int		_sizeImage[2];
	int		_sizeImageChannel[2];
	int		_sizeMacroPixel[2];
	int*	_pixelValue;
	int		_colorRGBSaturation[256][3];
	int		_lut[9][256][3];
	double	_coefficientsChannelWhite[3];
	double*	_coefficientsChannelNorm;
	double*	_regionValue;
	double*	_coefficientsReferenceSpectral;
	bool	_stateCrosstalkCorrrection;
	vector<int>	_displayOrder;
	vector<vector<double>> _coefficientsAlgorithms;

	void calcImageMultispectralCubeGenerique();
	void calcImageMultispectralCubeToucan();
	void calcImageMultispectralCube16();
	void calcImageMultispectralCube9();
	void calcImageMultispectralCube4();

	void calcImageColor16();
	void calcImageColorToucan();
	void calcImageColor9();
	void calcImageColor4();
	void calcImageColorGenerique();
	void calcImageProcessedGenerique(double upLimit, double downLimit, double Gamma, int color, int algChoice, int offsetCompensation);
	void calcImageProcessedToucan(double upLimit, double downLimit, double Gamma, int color, int algChoice, int offsetCompensation);
	void calcImageProcessed16(double upLimit, double downLimit, double Gamma, int color, int algChoice, int offsetCompensation);
	void calcImageProcessed9(double upLimit, double downLimit, double Gamma, int color, int algChoice, int offsetCompensation);
	void calcImageProcessed4(double upLimit, double downLimit, double Gamma, int color, int algChoice, int offsetCompensation);

	void setColorRGBSaturation();

	void updateSizeImage();

	cmsInfos*	_cmsInfos;
	cmsCamera*	_cmsCamera;

	cv::WImageBuffer<unsigned short> imageRaw;
	cv::WImageBuffer<unsigned char> imageRawDisplay;

	cv::WImageBuffer<unsigned short> imageMultispectralCube;
	cv::WImageBufferC<unsigned char, 1> imageMultispectralCubeDisplay;
		
	vector<cv::WImageBufferC<unsigned short,1>*> imageChannel;
	vector<cv::WImageBufferC<unsigned char,1>*> imageChannelDisplay;

	cv::WImageBufferC<unsigned char, 3> imageRGBDisplay;
	cv::WImageBufferC<unsigned char, 3> imageBGRDisplay;

	cv::WImageBufferC<unsigned short, 3> imageRGB;
	cv::WImageBufferC<unsigned short, 3> imageBGR;
	
	cv::WImageBufferC<double, 1> imageProcessedCalc;
	cv::WImageBufferC<unsigned short, 3> imageProcessed;
	cv::WImageBufferC<unsigned char, 3> imageProcessedDisplay;

	cv::WImageBufferC<unsigned short, 1> Flatfield;
	cv::WImageBufferC<unsigned short, 1> Darkfield;
	bool flatfieldCorrection;

	cv::WImageBufferC<double, 1> averageField;
	cv::WImageBufferC<unsigned short, 1> imageRawFlatField;
	void calcAverageField();

	void initLUT();

};