#include <cctype>			// for std::tolower
#include <algorithm>
#include <array>
#include <zlib.h>
#include <cstring>
#ifdef _WIN32
#include <cstdlib>
#else
#include <sys\syslimits.h>
#endif
#include "ZipFile.hpp"


// --------------------------------------------------------------------------
// Basic types.
// --------------------------------------------------------------------------
using dword = unsigned long;
using word = unsigned short;
using byte = unsigned char;

#ifdef _WIN32
constexpr size_t MAX_PATH = _MAX_PATH;
#else
constexpr size_t MAX_PATH = PATH_MAX;
#endif

// --------------------------------------------------------------------------
// ZIP file structures. Note these have to be packed.
// --------------------------------------------------------------------------

#pragma pack(1)
// --------------------------------------------------------------------------
// struct ZipFile::TZipLocalHeader					- Chapter 8, page 215
// --------------------------------------------------------------------------
struct ZipFile::TZipLocalHeader {
	enum {
		SIGNATURE = 0x04034b50,
	};
	dword   sig;
	word    version;
	word    flag;
	word    compression;      // Z_NO_COMPRESSION or Z_DEFLATED
	word    modTime;
	word    modDate;
	dword   crc32;
	dword   cSize;
	dword   ucSize;
	word    fnameLen;         // Filename string follows header.
	word    xtraLen;          // Extra field follows filename.
};

// --------------------------------------------------------------------------
// struct ZipFile::TZipDirHeader					- Chapter 8, page 215
// --------------------------------------------------------------------------
struct ZipFile::TZipDirHeader {
	enum {
		SIGNATURE = 0x06054b50
	};
	dword   sig;
	word    nDisk;
	word    nStartDisk;
	word    nDirEntries;
	word    totalDirEntries;
	dword   dirSize;
	dword   dirOffset;
	word    cmntLen;
};

// --------------------------------------------------------------------------
// struct ZipFile::TZipDirFileHeader					- Chapter 8, page 215
// --------------------------------------------------------------------------
struct ZipFile::TZipDirFileHeader {
	enum {
		SIGNATURE = 0x02014b50
	};
	dword   sig;
	word    verMade;
	word    verNeeded;
	word    flag;
	word    compression;      // COMP_xxxx
	word    modTime;
	word    modDate;
	dword   crc32;
	dword   cSize;            // Compressed size
	dword   ucSize;           // Uncompressed size
	word    fnameLen;         // Filename string follows header.
	word    xtraLen;          // Extra field follows filename.
	word    cmntLen;          // Comment field follows extra field.
	word    diskStart;
	word    intAttr;
	dword   extAttr;
	dword   hdrOffset;

	char *GetName() const { return (char *)(this + 1); }
	char *GetExtra() const { return GetName() + fnameLen; }
	char *GetComment() const { return GetExtra() + xtraLen; }
};

#pragma pack()


/*
==================
ZipFile::Init

	Initialize the object and read the zip file directory.
==================
*/
bool ZipFile::Init(const std::string &resFileName) {
	End();

	m_pFile = fopen(resFileName.c_str(), "rb");
	if (!m_pFile) {
		return false;
	}

	// Assuming no extra comment at the end, read the whole end record.
	TZipDirHeader dh;

	fseek(m_pFile, -(int)sizeof dh, SEEK_END);
	const auto dhOffset = ftell(m_pFile);
	memset(&dh, 0, sizeof dh);
	fread(&dh, sizeof dh, 1, m_pFile);

	// Check
	if (dh.sig != TZipDirHeader::SIGNATURE) {
		return false;
	}

	// Go to the beginning of the directory.
	fseek(m_pFile, dhOffset - dh.dirSize, SEEK_SET);

	// Allocate the data buffer, and read the whole thing.
	m_pDirData = new char[dh.dirSize + dh.nDirEntries * sizeof(*m_papDir)];
	if (!m_pDirData) {
		return false;
	}
		
	memset(m_pDirData, 0, dh.dirSize + dh.nDirEntries * sizeof(*m_papDir));
	fread(m_pDirData, dh.dirSize, 1, m_pFile);

	// Now process each entry.
	char *pfh = m_pDirData;
	m_papDir = reinterpret_cast<const TZipDirFileHeader**>(m_pDirData + dh.dirSize);

	bool success = true;

	for (int i = 0; i < dh.nDirEntries && success; i++) {
		TZipDirFileHeader &fh = *reinterpret_cast<TZipDirFileHeader*>(pfh);

		// Store the address of nth file for quicker access.
		m_papDir[i] = &fh;

		// Check the directory entry integrity.
		if (fh.sig != TZipDirFileHeader::SIGNATURE) {
			success = false;
		} else {
			pfh += sizeof(fh);

			// Convert UNIX slashes to DOS backlashes.
			for (int j = 0; j < fh.fnameLen; j++) {
				if (pfh[j] == '/') {
					pfh[j] = '\\';
				}				
			}

			std::array<char, MAX_PATH> fileName;
			memcpy(fileName.data(), pfh, fh.fnameLen);
			fileName[fh.fnameLen] = 0;
			std::transform(fileName.begin(), fileName.end(), fileName.begin(), (int(*)(int))std::tolower);
			std::string spath = fileName.data();
			m_ZipContentsMap[spath] = i;

			// Skip name, extra and comment fields.
			pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
		}
	}

	if (!success) {
		delete[] m_pDirData;
	} else {
		m_nEntries = dh.nDirEntries;
	}

	return success;
}

/*
==================
ZipFile::~ZipFile
==================
*/
ZipFile::~ZipFile() {
	End();
	fclose(m_pFile);
}

/*
==================
ZipFile::Find
==================
*/
int ZipFile::Find(const std::string &path) const {
	std::string lowerCase(path.length(), '\0');
	std::transform(path.cbegin(), path.cend(), lowerCase.begin(), (int(*)(int))std::tolower);
	auto i = m_ZipContentsMap.find(lowerCase);
	if (i == m_ZipContentsMap.end()) {
		return -1;
	}

	return i->second;
}

/*
==================
ZipFile::End

	Finish the object
==================
*/
void ZipFile::End() {
	m_ZipContentsMap.clear();
	delete[] m_pDirData;
	m_nEntries = 0;
}

/*
==================
ZipFile::GetFilename

	Return the name of a file
==================
*/
std::string ZipFile::GetFilename(int i)  const {
	std::string fileName = "";
	if (i >= 0 && i < m_nEntries) {
		char pszDest[MAX_PATH];
		memcpy(pszDest, m_papDir[i]->GetName(), m_papDir[i]->fnameLen);
		pszDest[m_papDir[i]->fnameLen] = '\0';
		fileName = pszDest;
	}
	return fileName;
}

/*
==================
ZipFile::GetFileLen

	Return the length of a file so a buffer can be allocated
==================
*/
int ZipFile::GetFileLen(int i) const {
	if (i < 0 || i >= m_nEntries) {
		return -1;
	}
	return m_papDir[i]->ucSize;
}

/*
==================
ZipFile::ReadFile

	Uncompress a complete file
==================
*/
bool ZipFile::ReadFile(int i, void *pBuf) {
	if (pBuf == nullptr || i < 0 || i >= m_nEntries) {
		return false;
	}

	// Quick'n dirty read, the whole file at once.
	// Ungood if the ZIP has huge files inside

	TZipLocalHeader h;
	memset(&h, 0, sizeof(h));

	// Go to the actual file and read the local header.
	fseek(m_pFile, m_papDir[i]->hdrOffset, SEEK_SET);
	fread(&h, sizeof(h), 1, m_pFile);
	if (h.sig != TZipLocalHeader::SIGNATURE) {
		return false;
	}

	// Skip extra fields
	fseek(m_pFile, h.fnameLen + h.xtraLen, SEEK_CUR);

	if (h.compression == Z_NO_COMPRESSION) {
		// Simply read in raw stored data.
		fread(pBuf, h.cSize, 1, m_pFile);
		return true;
	} else if (h.compression != Z_DEFLATED) {
		return false;
	}

	// Alloc compressed data buffer and read the whole stream
	char *pcData = new char[h.cSize];
	if (!pcData) {
		return false;
	}

	memset(pcData, 0, h.cSize);
	fread(pcData, h.cSize, 1, m_pFile);

	bool ret = true;

	// Setup the inflate stream.
	z_stream stream;
	int err;

	stream.next_in = (Bytef*)pcData;
	stream.avail_in = (uInt)h.cSize;
	stream.next_out = (Bytef*)pBuf;
	stream.avail_out = h.ucSize;
	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;

	// Perform inflation. wbits < 0 indicates no zlib header inside the data.
	err = inflateInit2(&stream, -MAX_WBITS);
	if (err == Z_OK) {
		err = inflate(&stream, Z_FINISH);
		inflateEnd(&stream);
		if (err == Z_STREAM_END) {
			err = Z_OK;
		}
		inflateEnd(&stream);
	}
	if (err != Z_OK) {
		ret = false;
	}

	delete[] pcData;
	return ret;
}
