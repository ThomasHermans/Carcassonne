#include "tilewidget.h"

#include <iostream>

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
    if (pixmap() == 0)
    {
        QPixmap pm = QPixmap(QString::fromUtf8(":/tiles/CRRs.png"));
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
