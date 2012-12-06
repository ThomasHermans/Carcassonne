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
    std::cout << "Pressed" << std::endl;
    if (pixmap() == 0)
    {
        std::cout << "pixmap == 0" << std::endl;
        QPixmap pm = QPixmap(QString::fromUtf8(":/tiles/CRRs.png"));
        setPixmap(pm);
    }
    else
    {
        std::cout << "pixmap != 0" << std::endl;
        const QPixmap * pm = pixmap();
        QTransform transform;
        QTransform transf = transform.rotate(90);
        setPixmap(pm->transformed(transf));
    }
}
