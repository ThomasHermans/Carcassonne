#ifndef TILEWIDGET_H
#define TILEWIDGET_H

#include <QLabel>
#include "boost/optional/optional.hpp"

class TileWidget : public QLabel
{
    Q_OBJECT
public:
    explicit TileWidget(QWidget *parent = 0);

private:
    void mousePressEvent(QMouseEvent *);
    
signals:
    void clicked();
    
public slots:
    void setTile(std::string inId, int inRotation);

};

#endif // TILEWIDGET_H
