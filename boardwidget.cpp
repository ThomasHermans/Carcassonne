#include "boardwidget.h"

namespace
{
int spacing = 3;
}

BoardWidget::BoardWidget(unsigned int inCols, unsigned int inRows, QWidget *parent) :
    QWidget(parent)
{
    mGridLayout = new QGridLayout();
    mGridLayout->setSpacing(spacing);
    mCols = inCols;
    mRows = inRows;
    for (unsigned int row = 0; row < mRows; ++row)
    {
        for (unsigned int col = 0; col < mCols; ++col)
        {
            TileWidget * theLabel = new TileWidget(this);
            mTiles.push_back(theLabel);
            connect(theLabel, SIGNAL(clicked()), this, SLOT(onTileClicked()));
            mGridLayout->addWidget(theLabel, row, col);
            theLabel->show();
        }
    }
    setFixedSize( mCols * TileWidget::getWidth() + (mCols - 1) * spacing, mRows * TileWidget::getHeight() + (mRows - 1) * spacing );
    setContentsMargins(0, 0, 0, 0);
    setLayout(mGridLayout);
}

void
BoardWidget::addRowsOnTop(unsigned int inNr)
{
    for (unsigned int i = 0; i < mTiles.size(); ++i)
    {
        mGridLayout->removeWidget( mTiles[i] );
    }
    mRows += inNr;
    unsigned int row = 0;
    std::vector< TileWidget* >::iterator it = mTiles.begin();
    for (; row < inNr; ++row)
    {
        for (unsigned int col = 0; col < mCols; ++col)
        {
            TileWidget * theLabel = new TileWidget(this);
            it = mTiles.insert(it, theLabel);
            ++it;
            connect(theLabel, SIGNAL(clicked()), this, SLOT(onTileClicked()));
            mGridLayout->addWidget(theLabel, row, col);
            theLabel->show();
        }
    }
    for (; row < mRows; ++row)
    {
        for (unsigned int col = 0; col < mCols; ++col)
        {
            mGridLayout->addWidget(mTiles[row * mCols + col], row, col);
        }
    }
    setFixedSize( mCols * TileWidget::getWidth() + (mCols - 1) * spacing, mRows * TileWidget::getHeight() + (mRows - 1) * spacing );
    setLayout(mGridLayout);
}

void
BoardWidget::addRowsBelow(unsigned int inNr)
{
    for (unsigned int i = 0; i < mTiles.size(); ++i)
    {
        mGridLayout->removeWidget( mTiles[i] );
    }
    unsigned int row = 0;
    std::vector< TileWidget* >::iterator it = mTiles.begin();
    for (; row < mRows; ++row)
    {
        for (unsigned int col = 0; col < mCols; ++col)
        {
            mGridLayout->addWidget(mTiles[row * mCols + col], row, col);
            ++it;
        }
    }
    for (; row < mRows + inNr; ++row)
    {
        for (unsigned int col = 0; col < mCols; ++col)
        {
            TileWidget * theLabel = new TileWidget(this);
            it = mTiles.insert(it, theLabel);
            ++it;
            connect(theLabel, SIGNAL(clicked()), this, SLOT(onTileClicked()));
            mGridLayout->addWidget(theLabel, row, col);
            theLabel->show();
        }
    }
    mRows += inNr;
    setFixedSize( mCols * TileWidget::getWidth() + (mCols - 1) * spacing, mRows * TileWidget::getHeight() + (mRows - 1) * spacing );
    setLayout(mGridLayout);
}

void
BoardWidget::addColsLeft(unsigned int inNr)
{
    for (unsigned int i = 0; i < mTiles.size(); ++i)
    {
        mGridLayout->removeWidget( mTiles[i] );
    }
    std::vector< TileWidget* >::iterator it = mTiles.begin();
    mCols += inNr;
    for (unsigned int row = 0; row < mRows; ++row)
    {
        unsigned int col = 0;
        for (; col < inNr; ++col)
        {
            TileWidget * theLabel = new TileWidget(this);
            it = mTiles.insert(it, theLabel);
            ++it;
            connect(theLabel, SIGNAL(clicked()), this, SLOT(onTileClicked()));
            mGridLayout->addWidget(theLabel, row, col);
            theLabel->show();
        }
        for (; col < mCols; ++col)
        {
            mGridLayout->addWidget(mTiles[row * mCols + col], row, col);
            ++it;
        }
    }
    setFixedSize( mCols * TileWidget::getWidth() + (mCols - 1) * spacing, mRows * TileWidget::getHeight() + (mRows - 1) * spacing );
    setLayout(mGridLayout);
}

void
BoardWidget::addColsRight(unsigned int inNr)
{
    for (unsigned int i = 0; i < mTiles.size(); ++i)
    {
        mGridLayout->removeWidget( mTiles[i] );
    }
    std::vector< TileWidget* >::iterator it = mTiles.begin();
    for (unsigned int row = 0; row < mRows; ++row)
    {
        unsigned int col = 0;
        for (; col < mCols; ++col)
        {
            ++it;
        }
        for (; col < mCols + inNr; ++col)
        {
            TileWidget * theLabel = new TileWidget(this);
            connect(theLabel, SIGNAL(clicked()), this, SLOT(onTileClicked()));
            theLabel->show();
            it = mTiles.insert(it, theLabel);
            ++it;
        }
    }
    mCols += inNr;
    for (unsigned int row = 0; row < mRows; ++row)
    {
        for (unsigned int col = 0; col < mCols; ++col)
        {
            mGridLayout->addWidget(mTiles[row * mCols + col], row, col);
        }
    }
    setFixedSize( mCols * TileWidget::getWidth() + (mCols - 1) * spacing, mRows * TileWidget::getHeight() + (mRows - 1) * spacing );
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
