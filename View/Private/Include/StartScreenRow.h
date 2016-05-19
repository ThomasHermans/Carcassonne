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
		explicit StartScreenRow( QWidget * inParent = 0 );
		~StartScreenRow();

		QString getName() const;
		Color getColor() const;
		bool isAI() const;

		void setName( std::string const & inName );
		void setColor( Color inColor );
		void setAI( bool inAI );

	signals:
		void removed();
		void colorChanged( Color inColor );

	private slots:
		void onColorChanged( int inIndex );

	private:
		QLineEdit * mNameLineEdit;
		QComboBox * mColorComboBox;
		QComboBox * mAIComboBox;
		QPushButton * mDeleteButton;
	};
}

#endif // STARTSCREENROW_H
