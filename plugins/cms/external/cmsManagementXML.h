#pragma once

#include "tinyxml.h"
#include "tinystr.h"
#include <string>

/*! class to get informations from XML files*/
class cmsManagementXML
{
public:
	//! Standard constructor 
	__declspec(dllexport) cmsManagementXML();

	//! Constructor with use of camera manufacturer serial number
	/*
		\param[in] electronicSerialNumber Manufacturer serial number
	*/
	__declspec(dllexport) cmsManagementXML(std::string* electronicSerialNumber);

	//! Standard destructor
	__declspec(dllexport) ~cmsManagementXML();

	//! Retreiving the camera caracteristics
	/*! 
		\param[out] CaracsIn : list of all caracteristics
	*/
	__declspec(dllexport) void getCharacteristicsCamera(std::string CaracsIn[]);

	//! Setting the camera caracteristics
	/*! 
		\param[in] Caracs : list of all caracteristics
	*/
	__declspec(dllexport) void setCharacteristicsCamera(std::string Caracs[]);

private:

	TiXmlDocument _xmlDoc;
	std::string _nameFileFilterImager;
	std::string _SN_IDS;
	std::string _encryptPass;
	std::string _sCarac[15];
	std::string password;

	std::string decrypt(std::string valueCrypted, std::string pass);
	std::string encrypt(std::string value, std::string pass);

};

