#include "boardwidget.h"

#include <QGridLayout>
#include <QLabel>
#include "tilewidget.h"

BoardWidget::BoardWidget(QWidget *parent) :
    QWidget(parent)
{
    mNrCols = 5;
    mNrRows = 5;
    mTiles = std::vector< boost::optional< TileOnBoard > >( mNrCols * mNrRows, boost::optional< TileOnBoard >());

    QGridLayout * gridLayout = new QGridLayout(this);
    for (unsigned int i = 0; i < mTiles.size(); ++i)
    {
        TileWidget * theLabel = new TileWidget(this);
        int row = i / mNrCols;
        int col = i % mNrCols;
        theLabel->show();
        gridLayout->addWidget(theLabel, row, col);
    }
    this->setLayout(gridLayout);
}
