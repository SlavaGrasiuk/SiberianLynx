#pragma once

#include <scmRTOS.h>

using BackLightCtrl	= OS::process<OS::pr0, 200>;
using UserApp		= OS::process<OS::pr1, 102'400>;

namespace OS {
	template<> void BackLightCtrl::exec();
	template<> void UserApp::exec();
}
