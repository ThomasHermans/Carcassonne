#include "boardwidget.h"

#include <QGridLayout>
#include <QLabel>
#include "tilewidget.h"

BoardWidget::BoardWidget(QWidget *parent) :
    QWidget(parent)
{
    mBoard = Board();
    unsigned int cols = mBoard.getNrOfCols();
    unsigned int rows = mBoard.getNrOfRows();

    QGridLayout * gridLayout = new QGridLayout(this);
    for (unsigned int row = 0; row < rows; ++row)
    {
        for (unsigned int col = 0; col < cols; ++col)
        {
            TileWidget * theLabel = new TileWidget(mBoard.getTile(col, row), this);
            theLabel->show();
            gridLayout->addWidget(theLabel, row, col);
        }
    }
    this->setLayout(gridLayout);
}

BoardWidget::BoardWidget(GameWindow *inGameWindow, QWidget *parent) :
    QWidget(parent)
{
    mGameWindow = inGameWindow;
    mBoard = inGameWindow->getBoard();
    unsigned int cols = mBoard.getNrOfCols();
    unsigned int rows = mBoard.getNrOfRows();

    QGridLayout * gridLayout = new QGridLayout(this);
    for (unsigned int row = 0; row < rows; ++row)
    {
        for (unsigned int col = 0; col < cols; ++col)
        {
            TileWidget * theLabel = new TileWidget(inGameWindow, mBoard.getTile(col, row), this);
            theLabel->show();
            gridLayout->addWidget(theLabel, row, col);
        }
    }
    this->setLayout(gridLayout);
}
