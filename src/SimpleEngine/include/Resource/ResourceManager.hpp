#pragma once

#include "../Interfaces.hpp"
#include <forward_list>
#include <map>

using std::make_shared;

class ZipFile;

/*
===============================================================================

	Implementation of IResourceFile interface to access .zip archives.

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
	virtual size_t GetRawResource(const std::string &r, uint8_t *buffer) override;
	virtual int GetNumResources() const override;
	virtual std::string GetResourceName(int num) const override;
};

/*
===============================================================================

	Default loader for all resource types.

===============================================================================
*/
class DefaultResourceLoader : public IResourceLoader {
public:
	virtual bool UseRawFile() const override {
		return true;
	}
	virtual bool DiscardRawBufferAfterLoad() const override {
		return true;
	}
	virtual size_t GetLoadedResourceSize(uint8_t *rawBuffer, size_t rawSize) override {
		return rawSize;
	}
	virtual bool LoadResource(uint8_t *rawBuffer, size_t rawSize, std::shared_ptr<ResHandle> handle) override {
		return true;
	}
	virtual const char *GetPattern() const override {
		return "*";
	}
};

/*
===============================================================================

	Interface to handle resource-kind-specific data.

===============================================================================
*/
class IResourceExtraData {
public:
	virtual std::string ToString() const = 0;
};

/*
===============================================================================

	Class for handling extracted resource data.

===============================================================================
*/
class ResHandle {
	std::string m_resourceName;
	uint8_t *m_buffer;
	const size_t m_size;
	std::shared_ptr<IResourceExtraData> m_extra;

public:
	ResHandle(const std::string &resource, uint8_t *buffer, size_t size) : m_resourceName(resource), m_buffer(buffer), m_size(size), m_extra(nullptr) {}

	virtual ~ResHandle();

	const std::string &GetName() const { 
		return m_resourceName;
	}
	size_t Size() const {
		return m_size;
	}
	const uint8_t *Buffer() const {
		return m_buffer;
	}
	uint8_t *WritableBuffer() {
		return m_buffer;
	}

	std::shared_ptr<IResourceExtraData> GetExtra() {
		return m_extra;
	}
	void SetExtra(std::shared_ptr<IResourceExtraData> extra) {
		m_extra = extra;
	}
};

using ResourceLoadersList = std::forward_list<std::shared_ptr<IResourceLoader>>;
using ResourceMap = std::map<std::string, std::weak_ptr<ResHandle>>;
using ResHandlePtr = std::shared_ptr<ResHandle>;

/*
===============================================================================

	Resource loading and unpacking.

===============================================================================
*/
class ResourceManager {
	IResourceFile *m_resFile;
	ResourceLoadersList m_loaders;
	ResourceMap m_loadedResources;

public:
	ResourceManager(IResourceFile *resFile) : m_resFile(resFile) {}
	virtual ~ResourceManager();

	bool Init();

	void RegisterLoader(std::shared_ptr<IResourceLoader> loader) {
		m_loaders.push_front(loader);
	}

	ResHandlePtr GetHandle(const std::string &r);

private:
	std::weak_ptr<ResHandle> Find(const std::string &r);
	ResHandlePtr Load(const std::string &r);

	std::shared_ptr<IResourceLoader> FindLoader(const std::string & r);
};
