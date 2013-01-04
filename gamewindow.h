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


#include "game.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    Board getBoard() const;
    
signals:
    
public slots:

private:
    Game mGame;

    QWidget *mCentralWidget;
    QHBoxLayout *mBoardAndSideBarLayout;
    QScrollArea *mBoardScrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *mSideBarLayout;
    QLabel *mPickedTileLabel;

//    QMenuBar *mMenuBar;
//    QToolBar *mMainToolBar;
//    QStatusBar *mStatusBar;
};

#endif // GAMEWINDOW_H
