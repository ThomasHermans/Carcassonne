#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include "board.h"
#include "gamewindow.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = 0);
    explicit BoardWidget(GameWindow *inGameWindow, QWidget *parent = 0);

private:
    GameWindow *mGameWindow;
    Board mBoard;
    
signals:
    
public slots:
    
};

#endif // BOARDWIDGET_H
