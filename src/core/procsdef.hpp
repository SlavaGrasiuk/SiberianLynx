#pragma once

#include <scmRTOS.h>

namespace core {
	using BackLightCtrl	= OS::process<OS::pr0, 200>;
	using UserApp		= OS::process<OS::pr1, 102'400>;
}

namespace OS {
	template<> void core::BackLightCtrl::exec();
	template<> void core::UserApp::exec();
}
