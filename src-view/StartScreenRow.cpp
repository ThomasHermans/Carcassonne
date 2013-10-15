#include "src-view/StartScreenRow.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

StartScreenRow::StartScreenRow( QWidget * inParent )
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
}

StartScreenRow::~StartScreenRow()
{
}

QString
StartScreenRow::getName() const
{
	return mNameLineEdit->text();
}

Gui::Color
StartScreenRow::getColor() const
{
	return Gui::Color( mColorComboBox->currentIndex() );
}

void
StartScreenRow::setColor( Gui::Color inColor )
{
	mColorComboBox->setCurrentIndex( int( inColor ) );
}

void
StartScreenRow::onCurrentIndexChanged( int inIndex )
{
	emit colorChanged( Gui::Color( inIndex ) );
}

void
StartScreenRow::paintEvent( QPaintEvent * inEvent )
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}