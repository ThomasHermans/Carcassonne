#include "tilewidget.h"

#include <iostream>
#include <sstream>

TileWidget::TileWidget(QWidget *parent) :
    QLabel(parent)
{
    setMinimumSize(QSize(100, 100));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
    setText(".");
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
