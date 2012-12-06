#ifndef TILEWIDGET_H
#define TILEWIDGET_H

#include <QLabel>

class TileWidget : public QLabel
{
    Q_OBJECT
public:
    explicit TileWidget(QWidget *parent = 0);

private:
    void mousePressEvent(QMouseEvent *);
    
signals:
    
public slots:
    
};

#endif // TILEWIDGET_H
