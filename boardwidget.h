#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <vector>
#include <boost/optional/optional.hpp>
#include "tileonboard.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = 0);

private:
    int mNrCols;
    int mNrRows;
    std::vector< boost::optional< TileOnBoard > > mTiles;
    
signals:
    
public slots:
    
};

#endif // BOARDWIDGET_H
