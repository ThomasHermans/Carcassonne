#include "StartScreenRow.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QSize>
#include <QString>
#include <QStyle>
#include <QStyleOption>

namespace
{
	int const kAIIndex = 0;
	int const kPlayerIndex = 1;
	
	QString
	fromStd( std::string const & inString )
	{
		return QString::fromUtf8( inString.c_str() );
	}
}

View::StartScreenRow::StartScreenRow( QWidget * inParent )
:
	QWidget( inParent ),
	mNameLineEdit( new QLineEdit( this ) ),
	mColorComboBox( new QComboBox( this ) ),
	mAIComboBox( new QComboBox( this ) ),
	mDeleteButton( new QPushButton( this ) )
{
	mColorComboBox->addItem( "Red" );
	mColorComboBox->addItem( "Green" );
	mColorComboBox->addItem( "Blue" );
	mColorComboBox->addItem( "Yellow" );
	mColorComboBox->addItem( "Black" );
	mColorComboBox->addItem( "Gray" );
	connect( mColorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onColorChanged(int)) );

	mAIComboBox->addItem( "Robot" );
	mAIComboBox->addItem( "Player" );

	mDeleteButton->setIcon( QPixmap( ":/Bin.png" ) );
	mDeleteButton->setIconSize( QSize( 12, 12 ) );
	connect( mDeleteButton, SIGNAL( clicked() ), this, SIGNAL( removed() ) );

	QHBoxLayout * rowLayout = new QHBoxLayout();
	setLayout( rowLayout );

	rowLayout->addWidget( mNameLineEdit );
	rowLayout->addWidget( mColorComboBox );
	rowLayout->addWidget( mAIComboBox );
	rowLayout->addWidget( mDeleteButton );

	mNameLineEdit->setFocus();
}

View::StartScreenRow::~StartScreenRow()
{
}

QString
View::StartScreenRow::getName() const
{
	return mNameLineEdit->text();
}

View::Color
View::StartScreenRow::getColor() const
{
	return Color( mColorComboBox->currentIndex() );
}

bool
View::StartScreenRow::isAI() const
{
	return mAIComboBox->currentIndex() == kAIIndex;
}

void
View::StartScreenRow::setName( std::string const & inName )
{
	mNameLineEdit->setText( fromStd( inName ) );
}

void
View::StartScreenRow::setColor( Color inColor )
{
	mColorComboBox->setCurrentIndex( int( inColor ) );
}

void
View::StartScreenRow::setAI( bool inAI )
{
	mAIComboBox->setCurrentIndex( inAI ? kAIIndex : kPlayerIndex );
}

void
View::StartScreenRow::onColorChanged( int inIndex )
{
	emit colorChanged( Color( inIndex ) );
}
