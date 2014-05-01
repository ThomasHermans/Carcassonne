#include "Settings.h"

#include <QSettings>
#include <QString>

namespace
{
	QSettings &
	getSettings()
	{
		static QSettings sSettings( "TeamDinoRaptor", "Carcasonne" );

		return sSettings;
	}

	QString
	fromStd( std::string const & inString )
	{
		return QString::fromUtf8( inString.c_str() );
	}

	std::string
	toStd( QString const & inString )
	{
		return std::string( inString.toUtf8().constData() );
	}
}

void
Controller::Settings::storeString
(
	std::string const & inKey,
	std::string const & inValue
)
{
	getSettings().setValue( fromStd( inKey ), fromStd( inValue ) );
}

std::string
Controller::Settings::getString
(
	std::string const & inKey,
	std::string const & inDefaultValue
)
{
	QVariant const value = getSettings().value( fromStd( inKey ) );
	if ( value != QVariant() )
	{
		QString const string = value.toString();
		if ( !string.isEmpty() )
		{
			return toStd( string );
		}
	}
	return inDefaultValue;
}

void
Controller::Settings::storeNumber
(
	std::string const & inKey,
	std::size_t inValue
)
{
	getSettings().setValue( fromStd( inKey ), inValue );
}

std::size_t
Controller::Settings::getNumber
(
	std::string const & inKey,
	std::size_t inDefaultValue
)
{
	QVariant const value = getSettings().value( fromStd( inKey ) );
	if ( value != QVariant() )
	{
		return value.value< std::size_t >();
	}
	return inDefaultValue;
}