#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>

#include "boardwidget.h"
#include "game.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    void addRowsOnTop(unsigned int inNr);
    void addRowsBelow(unsigned int inNr);
    void addColsLeft(unsigned int inNr);
    void addColsRight(unsigned int inNr);
    
signals:
    void clicked(unsigned int inCol, unsigned int inRow);
    
public slots:
    void setTile(unsigned int inCol, unsigned int inRow, std::string inId, int inRotation);
    void setNextTile(std::string inId);
    void onClicked(unsigned int inCol, unsigned int inRow);

private:
    QWidget *mCentralWidget;
    QHBoxLayout *mBoardAndSideBarLayout;
    QScrollArea *mBoardScrollArea;
    BoardWidget *mBoardWidget;
    QVBoxLayout *mSideBarLayout;
    QLabel *mPickedTileLabel;

//    QMenuBar *mMenuBar;
//    QToolBar *mMainToolBar;
//    QStatusBar *mStatusBar;
};

#endif // GAMEWINDOW_H
