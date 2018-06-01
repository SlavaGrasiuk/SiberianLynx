#include "procsdef.hpp"
#include "commdef.hpp"
#include "../3rdParty/FatFs/ff.h"

UserApp g_userApp;


int main(int argc, char **argv);

static FATFS g_fs;

/*
==================
UserApp::exec

	call app ctors, change CWD and call main
==================
*/
template<> void UserApp::exec() {
	if (f_mount(&g_fs, "BSD:/", 1) != FR_OK) {
		if (f_mount(&g_fs, "TSD:/", 1) != FR_OK) {
			EXCEPT_HNDL()
		} else {
			f_chdir("TSD:/");
		}
	} else {
		f_chdir("BSD:/");
	}
	
	main(0, nullptr);

	OS::sleep();
	__builtin_unreachable();
}
