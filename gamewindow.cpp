#include "gamewindow.h"

#include <sstream>

#include "boardwidget.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(800, 500);
    mCentralWidget = new QWidget(this);
    mCentralWidget->setObjectName(QString::fromUtf8("mCentralWidget"));

    mBoardAndSideBarLayout = new QHBoxLayout();
    mBoardAndSideBarLayout->setSpacing(6);
    mBoardAndSideBarLayout->setObjectName(QString::fromUtf8("mBoardAndSideBarLayout"));

    mBoardScrollArea = new QScrollArea(mCentralWidget);
    mBoardScrollArea->setObjectName(QString::fromUtf8("mBoardScrollArea"));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(mBoardScrollArea->sizePolicy().hasHeightForWidth());
    mBoardScrollArea->setSizePolicy(sizePolicy);
    mBoardScrollArea->setMinimumSize(QSize(400, 400));
    mBoardScrollArea->setBaseSize(QSize(100, 100));
    mBoardScrollArea->setWidgetResizable(true);

    mBoardWidget = new BoardWidget();
    mBoardWidget->setObjectName(QString::fromUtf8("mBoardWidget"));
    mBoardWidget->setGeometry(QRect(0, 0, 400, 400));
    mBoardScrollArea->setWidget(mBoardWidget);

    connect(mBoardWidget, SIGNAL(clicked(uint, uint)), this, SLOT(onClicked(uint, uint)));

    mBoardAndSideBarLayout->addWidget(mBoardScrollArea);

    mSideBarLayout = new QVBoxLayout();
    mSideBarLayout->setObjectName(QString::fromUtf8("mSideBarLayout"));
    mSideBarLayout->setSpacing(6);

    mTilesLeft = new QLabel(mCentralWidget);
    mTilesLeft->setObjectName(QString::fromUtf8("mTilesLeft"));
    mTilesLeft->setText("X tiles left");

    mSideBarLayout->addWidget(mTilesLeft);

    mPickedTileLabel = new QLabel(mCentralWidget);
    mPickedTileLabel->setObjectName(QString::fromUtf8("mPickedTileLabel"));
    mPickedTileLabel->setFixedSize(QSize(100, 100));
    mPickedTileLabel->setText(".");
    mPickedTileLabel->setAlignment(Qt::AlignCenter);

    mSideBarLayout->addWidget(mPickedTileLabel);

    mSideBarLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

    mBoardAndSideBarLayout->addLayout(mSideBarLayout);

    mCentralWidget->setLayout(mBoardAndSideBarLayout);
    setCentralWidget(mCentralWidget);

//    mMenuBar = new QMenuBar(this);
//    mMenuBar->setObjectName(QString::fromUtf8("mMenuBar"));
//    mMenuBar->setGeometry(QRect(0, 0, 800, 21));
//    setMenuBar(mMenuBar);
//    mMainToolBar = new QToolBar(this);
//    mMainToolBar->setObjectName(QString::fromUtf8("mMainToolBar"));
//    addToolBar(Qt::TopToolBarArea, mMainToolBar);
//    mStatusBar = new QStatusBar(this);
//    mStatusBar->setObjectName(QString::fromUtf8("mStatusBar"));
//    setStatusBar(mStatusBar);
}

GameWindow::~GameWindow()
{
}

void
GameWindow::addRowsOnTop(unsigned int inNr)
{
    mBoardWidget->addRowsOnTop(inNr);
}

void
GameWindow::addRowsBelow(unsigned int inNr)
{
    mBoardWidget->addRowsBelow(inNr);
}

void
GameWindow::addColsLeft(unsigned int inNr)
{
    mBoardWidget->addColsLeft(inNr);
}

void
GameWindow::addColsRight(unsigned int inNr)
{
    mBoardWidget->addColsRight(inNr);
}

void
GameWindow::displayTilesLeft(unsigned int inNr)
{
    mTilesLeft->setText(QString::number(inNr).append(" tiles left."));
}

void
GameWindow::setTile(unsigned int inCol, unsigned int inRow, std::string inId, int inRotation)
{
    mBoardWidget->setTile(inCol, inRow, inId, inRotation);
}

void
GameWindow::setNextTile(std::string inId)
{
    std::stringstream sstr;
    sstr << ":/tiles/" << inId << ".png";
    const QPixmap pm(QString::fromStdString(sstr.str()));
    mPickedTileLabel->setPixmap(pm);
}

void
GameWindow::onClicked(unsigned int inCol, unsigned int inRow)
{
    std::cout << "GameWindow sees a click" << std::endl;
    emit clicked(inCol, inRow);
}
