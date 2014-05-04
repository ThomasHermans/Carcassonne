#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

namespace Controller
{
	namespace Settings
	{
		template< class T >
		void
		storeValue
		(
			std::string const & inKey,
			T const & inValue
		);

		template< class T >
		T
		getValue
		(
			std::string const & inKey,
			T const & inDefaultValue
		);
	}
}

#include "Settings.hpp"

#endif