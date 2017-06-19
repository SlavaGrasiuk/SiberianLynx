#include "procsdef.hpp"
#include "commdef.hpp"


static UserApp g_userApp;


int main(int argc, char **argv);


/*
==================
UserApp::exec
==================
*/
template<> void UserApp::exec() {

	//call app ctors, change CWD and call main

	main(0, nullptr);

	OS::sleep();
}