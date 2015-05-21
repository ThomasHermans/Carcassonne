#ifdef CONTROLLER_SETTINGS_HPP_20140504
	#error "Template implementation for Settings.h, only Settings.h should include this (and only once)!"
#else

#include <QSettings>
#include <QString>

namespace Private
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

namespace Controller
{
	namespace Settings
	{
		template<>
		void
		storeValue< std::string >
		(
			std::string const & inKey,
			std::string const & inValue
		)
		{
			using namespace Private;
			getSettings().setValue( fromStd( inKey ), fromStd( inValue ) );
		}

		template<>
		std::string
		getValue< std::string >
		(
			std::string const & inKey,
			std::string const & inDefaultValue
		)
		{
			using namespace Private;
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

		template< class T >
		void
		storeValue( std::string const & inKey, T const & inValue )
		{
			using namespace Private;
			QVariant variant;
			variant.setValue< T >( inValue );
			getSettings().setValue( fromStd( inKey ), variant );
		}

		template< class T >
		T
		getValue( std::string const & inKey, T const & inDefaultValue )
		{
			using namespace Private;
			QVariant const value = getSettings().value( fromStd( inKey ) );
			if ( value != QVariant() )
			{
				return value.value< T >();
			}
			return inDefaultValue;
		}
		
	}
}

#endif
