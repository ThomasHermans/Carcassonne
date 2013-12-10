#ifndef STARTSCREENROW_H
#define STARTSCREENROW_H

#include "View/Typedefs.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPaintEvent>
#include <QPushButton>
#include <QWidget>

namespace View
{
	class StartScreenRow : public QWidget
	{
		Q_OBJECT
	public:
		StartScreenRow( QWidget * inParent = 0 );
		~StartScreenRow();

		QString getName() const;
		Color getColor() const;

		void setName( QString const & inName );
		void setColor( Color inColor );

	signals:
		void removed();
		void colorChanged( Color inColor );

	private slots:
		void onCurrentIndexChanged( int inIndex );

	private:
		void paintEvent( QPaintEvent * inEvent );

	private:
		QLineEdit * mNameLineEdit;
		QComboBox * mColorComboBox;
		QPushButton * mDeleteButton;
	};
}

#endif // STARTSCREENROW_H
