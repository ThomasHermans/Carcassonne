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

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "BoardView.h"
#include "TileItem.h"
#include "game.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    void displayTilesLeft(unsigned int inNr);
    
signals:
    void clicked(int x, int y);
    
public slots:
    void setTile(int inX, int inY, std::string inId, int inRotation);
    void setNextTile(std::string inId);
    void onClicked(int x, int y);

private:
    QWidget *mCentralWidget;
    QHBoxLayout *mBoardAndSideBarLayout;
    QGraphicsScene *mBoardScene;
    BoardView *mBoardView;
    std::vector< TileItem* > mTiles;
    QVBoxLayout *mSideBarLayout;
    QLabel *mTilesLeft;
    QLabel *mPickedTileLabel;
};

#endif // GAMEWINDOW_H
