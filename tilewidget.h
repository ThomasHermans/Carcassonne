#ifndef TILEWIDGET_H
#define TILEWIDGET_H

#include <QLabel>
#include "boost/optional/optional.hpp"
#include "gamewindow.h"
#include "tileonboard.h"

class TileWidget : public QLabel
{
    Q_OBJECT
public:
    explicit TileWidget(QWidget *parent = 0);
    explicit TileWidget(boost::optional< TileOnBoard > inTileOnBoard, QWidget *parent = 0);
    explicit TileWidget(GameWindow *inGameWindow, boost::optional< TileOnBoard > inTileOnBoard, QWidget *parent = 0);

private:
    void mousePressEvent(QMouseEvent *);
    
signals:
    
public slots:

private:
    GameWindow *mGameWindow;
    boost::optional< TileOnBoard > mTileOnBoard;
    
};

#endif // TILEWIDGET_H
