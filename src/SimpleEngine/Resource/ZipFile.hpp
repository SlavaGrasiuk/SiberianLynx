#pragma once

#include <cstdio>
#include <map>
#include <string>

using ZipContentsMap = std::map<std::string, int>;		// maps path to a zip content id

class ZipFile {
	struct TZipDirHeader;
	struct TZipDirFileHeader;
	struct TZipLocalHeader;

	FILE *m_pFile;		// Zip file
	char *m_pDirData;	// Raw data buffer.
	int  m_nEntries;	// Number of entries.

	const TZipDirFileHeader **m_papDir;		// Pointers to the dir entries in pDirData.

public:
	ZipFile(): m_nEntries(0), m_pFile(nullptr), m_pDirData(nullptr) {}
	virtual ~ZipFile();

	bool Init(const std::string &resFileName);
	void End();

	int GetNumFiles()const { return m_nEntries; }
	std::string GetFilename(int i) const;
	int GetFileLen(int i) const;
	bool ReadFile(int i, void *pBuf);

	int Find(const std::string &path) const;

	ZipContentsMap m_ZipContentsMap;
};
