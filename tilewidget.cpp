#include "tilewidget.h"

#include <iostream>
#include <sstream>

TileWidget::TileWidget(boost::optional<TileOnBoard> inTileOnBoard, QWidget *parent) :
    QLabel(parent)
{
    mTileOnBoard = inTileOnBoard;
    setMinimumSize(QSize(100, 100));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
    if (mTileOnBoard)
    {
        std::string id = mTileOnBoard->getID();
        std::stringstream name;
        name << ":/tiles/" << id << ".png";
        QPixmap pm = QPixmap(QString::fromStdString(name.str()));
        setPixmap(pm);
    }
    else
    {
        setText(".");
    }
}

TileWidget::TileWidget(GameWindow *inGameWindow, boost::optional<TileOnBoard> inTileOnBoard, QWidget *parent) :
    QLabel(parent)
{
    mGameWindow = inGameWindow;
    mTileOnBoard = inTileOnBoard;
    setMinimumSize(QSize(100, 100));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
    if (mTileOnBoard)
    {
        std::string id = mTileOnBoard->getID();
        std::stringstream name;
        name << ":/tiles/" << id << ".png";
        QPixmap pm = QPixmap(QString::fromStdString(name.str()));
        setPixmap(pm);
    }
    else
    {
        setText(".");
    }
}

void
TileWidget::mousePressEvent(QMouseEvent *)
{
    if (pixmap() == 0)
    {
        QPixmap pm = QPixmap(QString::fromUtf8(":/tiles/A.png"));
        setPixmap(pm);
    }
    else
    {
        const QPixmap * pm = pixmap();
        QTransform transform;
        QTransform transf = transform.rotate(90);
        setPixmap(pm->transformed(transf));
    }
}
