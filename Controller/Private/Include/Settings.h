#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

namespace Controller
{
	namespace Settings
	{
		void
		storeString
		(
			std::string const & inKey,
			std::string const & inValue
		);

		std::string
		getString
		(
			std::string const & inKey,
			std::string const & inDefaultValue
		);

		void
		storeNumber
		(
			std::string const & inKey,
			std::size_t inValue
		);

		std::size_t
		getNumber
		(
			std::string const & inKey,
			std::size_t inDefaultValue
		);
	}
}

#endif