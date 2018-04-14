#pragma once

#include "../include/Interfaces.hpp"
#include <string>

class ZipFile;

/*
===============================================================================

	Implementation of IResourceFile interface to access .zip archives

===============================================================================
*/
class ResourceZipFile: public IResourceFile {
	ZipFile *m_zipFile;
	std::string m_resFileName;

public:
	ResourceZipFile(const std::string &resFileName): m_zipFile(nullptr), m_resFileName(resFileName) {}
	virtual ~ResourceZipFile();

	virtual bool Open() override;
	virtual int GetRawResourceSize(const std::string &r) override;
	virtual std::unique_ptr<uint8_t[]> GetRawResource(const std::string &r) override;
	virtual int GetNumResources() const override;
	virtual std::string GetResourceName(int num) const override;
};


/*
===============================================================================

	

===============================================================================
*/
class ResourceManager {
	IResourceFile *m_resFile;

};
