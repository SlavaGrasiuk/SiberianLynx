#include <cstdio>
#include <cstring>
#include "../3rdParty/FatFs/ff.h"
#include <new>

/*
==================
fopen
==================
*/
FILE *fopen(const char *__restrict name, const char *__restrict mode) {
	FIL *file = new(std::nothrow) FIL;
	BYTE modeFlags = 0;

	switch (mode[0]) {
		case 'r':			/* open for reading */
			modeFlags |= FA_READ;
			break;

		case 'w':			/* open for writing */
			modeFlags |= FA_CREATE_ALWAYS | FA_WRITE;
			break;

		case 'a':			/* open for appending */
			modeFlags |= FA_OPEN_APPEND;
			break;

		default:			/* illegal mode */
			delete file;
			return nullptr;
	}

	while (*++mode) {
		switch (*mode) {
			case '+':
				modeFlags |= FA_WRITE | FA_READ;
				break;

			case 'x':
				modeFlags &= ~(FA_CREATE_ALWAYS);
				modeFlags |= FA_CREATE_NEW;
				break;

			default:
				break;
		}
	}

	if (f_open(file, name, modeFlags) != FR_OK) {
		delete file;
		return nullptr;
	}

	return reinterpret_cast<FILE*>(file);
}

/*
==================
fgets
==================
*/
char *fgets(char *__restrict string, int num, FILE *__restrict filestream) {
	return f_gets(string, num, reinterpret_cast<FIL*>(filestream));
}

/*
==================
fclose
==================
*/
int	fclose(FILE *filestream) {
	auto castFile = reinterpret_cast<FIL*>(filestream);

	if (f_close(castFile) == FR_OK) {
		delete castFile;
		return 0;
	}

	return EOF;
}

/*
==================
fseek
==================
*/
int fseek(FILE *filestream, long offset, int origin) {
	auto castFile = reinterpret_cast<FIL*>(filestream);

	switch (origin) {
		case SEEK_CUR:
			return f_lseek(castFile, f_tell(castFile) + offset);

		case SEEK_END:
			return f_lseek(castFile, f_size(castFile) + offset);

		case SEEK_SET:
			return f_lseek(castFile, offset);

		default:
			break;
	}

	return 1;
}

/*
==================
fread
==================
*/
size_t fread(_PTR __restrict ptrvoid, size_t _size, size_t _n, FILE *__restrict filestream) {
	size_t readedBytesCnt;
	f_read(reinterpret_cast<FIL*>(filestream), ptrvoid, _size * _n, &readedBytesCnt);

	return readedBytesCnt;
}

/*
==================
ftell
==================
*/
long ftell(FILE *filestream) {
	return f_tell(reinterpret_cast<FIL*>(filestream));
}
