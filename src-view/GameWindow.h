#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "src-view/BoardView.h"
#include "src-view/TileItem.h"

#include <QMainWindow>

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    void clearTile(int x, int y);
    void rotateTile(int x, int y, std::string inId, int inRotation);
    void displayTilesLeft(unsigned int inNr);

    void finishCloister(int inX, int inY);
    void finishCity(int inLeft, int inRight, int inTop, int inBottom);
    
signals:
    void clicked(int x, int y);
    void submitCurrentTile();
    
public slots:
    void setTile(int inX, int inY, std::string inId, int inRotation);
    void setNextTile(std::string inId);
    void fadeNextTile();
    void onClicked(int x, int y);

private:
    void updateSceneRect();

private:
    QWidget *mCentralWidget;
    QHBoxLayout *mBoardAndSideBarLayout;
    QGraphicsScene *mBoardScene;
    BoardView *mBoardView;
    std::vector< TileItem* > mTiles;
    QVBoxLayout *mSideBarLayout;
    QLabel *mTilesLeft;
    QLabel *mPickedTileLabel;
    QPushButton *mSubmitTileButton;
};

#endif // GAMEWINDOW_H
