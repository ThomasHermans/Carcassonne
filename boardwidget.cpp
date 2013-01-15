#include "boardwidget.h"

BoardWidget::BoardWidget(unsigned int inCols, unsigned int inRows, QWidget *parent) :
    QWidget(parent)
{
    QGridLayout * mGridLayout = new QGridLayout(this);
    mCols = inCols;
    for (unsigned int row = 0; row < inRows; ++row)
    {
        for (unsigned int col = 0; col < inCols; ++col)
        {
            TileWidget * theLabel = new TileWidget(this);
            theLabel->show();
            mGridLayout->addWidget(theLabel, row, col);
            mTiles.push_back(theLabel);
            connect(theLabel, SIGNAL(clicked()), this, SLOT(onTileClicked()));
        }
    }
    setLayout(mGridLayout);
}

void
BoardWidget::setTile(unsigned int inCol, unsigned int inRow, std::string inId, int inRotation)
{
    mTiles[inRow * mCols + inCol]->setTile(inId, inRotation);
}

void
BoardWidget::onTileClicked()
{
    unsigned int tileNr = (unsigned int)-1;
    for (unsigned int i = 0; i < mTiles.size(); ++i)
    {
        if (QObject::sender() == mTiles[i])
        {
            tileNr = i;
            break;
        }
    }
    std::cout << tileNr << std::endl;
    if (tileNr != (unsigned int)-1)
    {
        int row = tileNr / mCols;
        int col = tileNr % mCols;
        emit clicked(col, row);
    }
}
