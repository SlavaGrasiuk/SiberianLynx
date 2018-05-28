#include "../include/Resource/ResourceManager.hpp"
#include "ZipFile.hpp"
#include "../Utilities/String.hpp"
#include <cstring>


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
	m_zipFile = new(std::nothrow) ZipFile;
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
size_t ResourceZipFile::GetRawResource(const std::string &r, uint8_t *buffer) {
	const int resNum = m_zipFile->Find(r);
	if (resNum == -1) {
		return 0;
	}

	const int resSize = m_zipFile->GetFileLen(resNum);
	return m_zipFile->ReadFile(resNum, buffer) ? resSize : 0;
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

/*
==================
ResHandle::~ResHandle
==================
*/
ResHandle::~ResHandle() { 
	if (m_buffer) {
		delete[] m_buffer;
	}
}

/*
==================
ResourceManager::~ResourceManager
==================
*/
ResourceManager::~ResourceManager() {
	if (m_resFile) {
		delete m_resFile;
	}
}

/*
==================
ResourceManager::Init
==================
*/
bool ResourceManager::Init() {
	bool result = false;
	if (m_resFile->Open()) {
		RegisterLoader(make_shared<DefaultResourceLoader>());
		result = true;
	}
	return result;
}

/*
==================
ResourceManager::GetHandle

	
==================
*/
ResHandlePtr ResourceManager::GetHandle(const std::string & r) {
	auto weakHandle = Find(r);
	if (!weakHandle.expired()) {		// resource already loaded
		return weakHandle.lock();
	}

	auto handle = Load(r);
	if (handle) {			// if resource found, store weak_ptr to loaded resources map
		m_loadedResources[r] = handle;
	}
	
	return handle;
}

/*
==================
ResourceManager::Find
==================
*/
std::weak_ptr<ResHandle> ResourceManager::Find(const std::string & r) {
	auto it = m_loadedResources.find(r);
	if (it == m_loadedResources.end()) {		// resource not found in map
		return std::weak_ptr<ResHandle>();
	}

	if (it->second.expired()) {					// resource record found, but resource was deleted
		m_loadedResources.erase(r);
		return std::weak_ptr<ResHandle>();
	}

	return it->second;		// valid resource found
}

/*
==================
ResourceManager::Load

	Read resource from resource archive and unpack it.
==================
*/
ResHandlePtr ResourceManager::Load(const std::string & r) {
	std::shared_ptr<ResHandle> handle;

	auto loader = FindLoader(r);
	if (!loader) {
		return handle;		//default loader not found
	}

	const int rawSize = m_resFile->GetRawResourceSize(r);
	if (rawSize < 0) {
		return handle;		//resource not found in archive
	}

	const size_t allocSize = rawSize + ((loader->AddNullZero()) ? 1 : 0);
	uint8_t *rawBuffer = loader->UseRawFile() ? /*Allocate(allocSize)*/ new(std::nothrow) uint8_t[allocSize] : new(std::nothrow) uint8_t[allocSize];
	if (!rawBuffer) {
		return handle;		//out of memory
	}
	memset(rawBuffer, 0, allocSize);

	if (!m_resFile->GetRawResource(r, rawBuffer)) {
		return handle;		//error reading raw resource
	}

	uint8_t *buffer = nullptr;
	size_t size;

	if (loader->UseRawFile()) {
		buffer = rawBuffer;
		handle = make_shared<ResHandle>(r, buffer, size);
	} else {
		size = loader->GetLoadedResourceSize(rawBuffer, rawSize);
		if (size) {
			buffer = /*Allocate(size)*/ new(std::nothrow) uint8_t[size];
			if (!buffer) {
				return handle;		//out of memory
			}
		}

		handle = make_shared<ResHandle>(r, buffer, size);
		const bool success = loader->LoadResource(rawBuffer, rawSize, handle);
		if (loader->DiscardRawBufferAfterLoad() && rawBuffer) {
			delete[] rawBuffer;
		}

		if (!success) {
			return std::shared_ptr<ResHandle>();		//out of memory
		}
	}

	return handle;
}

/*
==================
ResourceManager::FindLoader
==================
*/
std::shared_ptr<IResourceLoader> ResourceManager::FindLoader(const std::string & r) {
	std::shared_ptr<IResourceLoader> loader;

	for (auto testLoader : m_loaders) {
		if (WildcardMatch(testLoader->GetPattern(), r.c_str())) {
			loader = testLoader;
			break;
		}
	}

	return loader;
}
