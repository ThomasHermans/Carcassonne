#ifndef STARTSCREENROW_H
#define STARTSCREENROW_H

#include "src-view/Typedefs.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPaintEvent>
#include <QPushButton>
#include <QWidget>

class StartScreenRow : public QWidget
{
	Q_OBJECT
public:
	StartScreenRow( QWidget * inParent = 0 );
	~StartScreenRow();

	QString getName() const;
	Gui::Color getColor() const;

	void setColor( Gui::Color inColor );

signals:
	void removed();
	void colorChanged( Gui::Color inColor );

private slots:
	void onCurrentIndexChanged( int inIndex );

private:
	void paintEvent( QPaintEvent * inEvent );

private:
	QLineEdit * mNameLineEdit;
	QComboBox * mColorComboBox;
	QPushButton * mDeleteButton;
};

#endif // STARTSCREENROW_H