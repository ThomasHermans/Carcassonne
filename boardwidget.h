#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>

#include <QGridLayout>
#include "tilewidget.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(unsigned int inCols = 5, unsigned int inRows = 5, QWidget *parent = 0);

private:
    
signals:
    void clicked(unsigned int inCol, unsigned int inRow);
    
public slots:
    void setTile(unsigned int inCol, unsigned int inRow, std::string inId, int inRotation);
    void onTileClicked();

private:
    QGridLayout *mGridLayout;
    std::vector< TileWidget* > mTiles;
    unsigned int mCols;
    
};

#endif // BOARDWIDGET_H
