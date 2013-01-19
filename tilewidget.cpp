#include "tilewidget.h"

#include <iostream>
#include <sstream>

namespace
{
int kWidth = 100;
int kHeight = 100;
}

TileWidget::TileWidget(QWidget *parent) :
    QLabel(parent)
{
    setFixedSize(kWidth, kHeight);
    setAlignment(Qt::AlignCenter);
    setText(".");
}

int
TileWidget::getWidth()
{
    return kWidth;
}

int
TileWidget::getHeight()
{
    return kHeight;
}

void
TileWidget::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

void
TileWidget::setTile(std::string inId, int inRotation)
{
    std::stringstream sstr;
    sstr << ":/tiles/" << inId << ".png";
    const QPixmap pm(QString::fromStdString(sstr.str()));
    QTransform transform;
    QTransform transf = transform.rotate(inRotation);
    setPixmap(pm.transformed(transf));
}
