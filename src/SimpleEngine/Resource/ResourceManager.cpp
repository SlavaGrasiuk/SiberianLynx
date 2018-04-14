#include "ResourceManager.hpp"
#include "ZipFile.hpp"


/*
==================
ResourceZipFile::~ResourceZipFile
==================
*/
ResourceZipFile::~ResourceZipFile() {
	if (m_zipFile) {
		delete m_zipFile;
	}
}

/*
==================
ResourceZipFile::Open
==================
*/
bool ResourceZipFile::Open() {
	m_zipFile = new ZipFile;
	if (m_zipFile) {
		return m_zipFile->Init(m_resFileName);
	}
	return false;
}

/*
==================
ResourceZipFile::GetRawResourceSize
==================
*/
int ResourceZipFile::GetRawResourceSize(const std::string & r) {
	const int resNum = m_zipFile->Find(r);
	if (resNum == -1) {
		return -1;
	}

	return m_zipFile->GetFileLen(resNum);
}

/*
==================
ResourceZipFile::GetRawResource
==================
*/
std::unique_ptr<uint8_t[]> ResourceZipFile::GetRawResource(const std::string &r) {
	const int resNum = m_zipFile->Find(r);
	if (resNum == -1) {
		return std::unique_ptr<uint8_t[]>();
	}

	const int resSize = m_zipFile->GetFileLen(resNum);
	if (resSize == -1) {
		return std::unique_ptr<uint8_t[]>();
	}

	auto resource = std::make_unique<uint8_t[]>(resSize);
	if (!m_zipFile->ReadFile(resNum, resource.get())) {
		return std::unique_ptr<uint8_t[]>();
	}

	return resource;
}

/*
==================
ResourceZipFile::GetNumResources
==================
*/
int ResourceZipFile::GetNumResources() const {
	return (m_zipFile == nullptr) ? 0 : m_zipFile->GetNumFiles();
}

/*
==================
ResourceZipFile::GetResourceName
==================
*/
std::string ResourceZipFile::GetResourceName(int num) const {
	if (m_zipFile == nullptr) {
		return "";
	}

	return m_zipFile->GetFilename(num);
}
