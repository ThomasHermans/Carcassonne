#include "StartScreenRow.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QString>
#include <QStyle>
#include <QStyleOption>

namespace
{
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
	mDeleteButton( new QPushButton( "X", this ) )
{
	mColorComboBox->addItem( "Red" );
	mColorComboBox->addItem( "Green" );
	mColorComboBox->addItem( "Blue" );
	mColorComboBox->addItem( "Yellow" );
	mColorComboBox->addItem( "Black" );
	mColorComboBox->addItem( "Gray" );
	connect( mColorComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onCurrentIndexChanged( int ) ) );
	connect( mDeleteButton, SIGNAL( clicked() ), this, SIGNAL( removed() ) );

	QHBoxLayout * rowLayout = new QHBoxLayout();
	setLayout( rowLayout );

	rowLayout->addWidget( mNameLineEdit );
	rowLayout->addWidget( mColorComboBox );
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
View::StartScreenRow::onCurrentIndexChanged( int inIndex )
{
	emit colorChanged( Color( inIndex ) );
}

void
View::StartScreenRow::paintEvent( QPaintEvent * inEvent )
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
