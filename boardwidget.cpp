#include "boardwidget.h"

#include <QGridLayout>
#include <QLabel>

BoardWidget::BoardWidget(QWidget *parent) :
    QWidget(parent)
{
    mNrCols = 5;
    mNrRows = 5;
    mTiles = std::vector< boost::optional< TileOnBoard > >( mNrCols * mNrRows, boost::optional< TileOnBoard >());

    QGridLayout * gridLayout = new QGridLayout(this);
    for (unsigned int i = 0; i < mTiles.size(); ++i)
    {
        QLabel * theLabel = new QLabel(this);
        int row = i / mNrCols;
        int col = i % mNrCols;
        if (row == 2 && col == 2)
        {
            QTransform transform;
            QTransform transf = transform.rotate(90);
            theLabel->setPixmap(QPixmap(QString::fromUtf8("startTile.png")).transformed(transf));
        }
        else
        {
            QString text;
            text.setNum(10*row + col);
            theLabel->setText(text);
        }
        theLabel->setMinimumSize(QSize(100, 100));
        theLabel->setAlignment(Qt::AlignCenter);
        theLabel->show();
        gridLayout->addWidget(theLabel, row, col);
    }
    this->setLayout(gridLayout);
}
