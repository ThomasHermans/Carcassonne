/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed 5. Dec 22:40:09 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

#include <boardwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *mBoardAndSideBarLayout;
    QScrollArea *mBoardScrollArea;
    BoardWidget *scrollAreaWidgetContents;
    QVBoxLayout *mSideBarLayout;
    QLabel *mTilesLeftLabel;
    QHBoxLayout *mPickedTileLayout;
    QLabel *mPickedTileLabel;
    QVBoxLayout *mTurnTileLayout;
    QLabel *mTurnRightLabel;
    QLabel *mTurnLeftLabel;
    QTextBrowser *mFlowLabel;
    QLabel *mErrorLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(676, 488);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        mBoardAndSideBarLayout = new QHBoxLayout();
        mBoardAndSideBarLayout->setSpacing(6);
        mBoardAndSideBarLayout->setObjectName(QString::fromUtf8("mBoardAndSideBarLayout"));
        mBoardScrollArea = new QScrollArea(centralWidget);
        mBoardScrollArea->setObjectName(QString::fromUtf8("mBoardScrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(mBoardScrollArea->sizePolicy().hasHeightForWidth());
        mBoardScrollArea->setSizePolicy(sizePolicy);
        mBoardScrollArea->setMinimumSize(QSize(400, 400));
        mBoardScrollArea->setBaseSize(QSize(400, 400));
        mBoardScrollArea->setWidgetResizable(false);
        scrollAreaWidgetContents = new BoardWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 500, 500));
        mBoardScrollArea->setWidget(scrollAreaWidgetContents);

        mBoardAndSideBarLayout->addWidget(mBoardScrollArea);

        mSideBarLayout = new QVBoxLayout();
        mSideBarLayout->setSpacing(6);
        mSideBarLayout->setObjectName(QString::fromUtf8("mSideBarLayout"));
        mTilesLeftLabel = new QLabel(centralWidget);
        mTilesLeftLabel->setObjectName(QString::fromUtf8("mTilesLeftLabel"));
        mTilesLeftLabel->setMinimumSize(QSize(50, 0));
        mTilesLeftLabel->setAlignment(Qt::AlignCenter);

        mSideBarLayout->addWidget(mTilesLeftLabel);

        mPickedTileLayout = new QHBoxLayout();
        mPickedTileLayout->setSpacing(6);
        mPickedTileLayout->setObjectName(QString::fromUtf8("mPickedTileLayout"));
        mPickedTileLabel = new QLabel(centralWidget);
        mPickedTileLabel->setObjectName(QString::fromUtf8("mPickedTileLabel"));
        mPickedTileLabel->setMinimumSize(QSize(100, 100));
        mPickedTileLabel->setPixmap(QPixmap(QString::fromUtf8("startTile.png")));
        mPickedTileLabel->setAlignment(Qt::AlignCenter);

        mPickedTileLayout->addWidget(mPickedTileLabel);

        mTurnTileLayout = new QVBoxLayout();
        mTurnTileLayout->setSpacing(6);
        mTurnTileLayout->setObjectName(QString::fromUtf8("mTurnTileLayout"));
        mTurnRightLabel = new QLabel(centralWidget);
        mTurnRightLabel->setObjectName(QString::fromUtf8("mTurnRightLabel"));
        sizePolicy.setHeightForWidth(mTurnRightLabel->sizePolicy().hasHeightForWidth());
        mTurnRightLabel->setSizePolicy(sizePolicy);
        mTurnRightLabel->setMinimumSize(QSize(40, 40));

        mTurnTileLayout->addWidget(mTurnRightLabel);

        mTurnLeftLabel = new QLabel(centralWidget);
        mTurnLeftLabel->setObjectName(QString::fromUtf8("mTurnLeftLabel"));
        sizePolicy.setHeightForWidth(mTurnLeftLabel->sizePolicy().hasHeightForWidth());
        mTurnLeftLabel->setSizePolicy(sizePolicy);
        mTurnLeftLabel->setMinimumSize(QSize(40, 40));

        mTurnTileLayout->addWidget(mTurnLeftLabel);


        mPickedTileLayout->addLayout(mTurnTileLayout);


        mSideBarLayout->addLayout(mPickedTileLayout);

        mFlowLabel = new QTextBrowser(centralWidget);
        mFlowLabel->setObjectName(QString::fromUtf8("mFlowLabel"));

        mSideBarLayout->addWidget(mFlowLabel);

        mErrorLabel = new QLabel(centralWidget);
        mErrorLabel->setObjectName(QString::fromUtf8("mErrorLabel"));
        mErrorLabel->setAlignment(Qt::AlignCenter);

        mSideBarLayout->addWidget(mErrorLabel);


        mBoardAndSideBarLayout->addLayout(mSideBarLayout);


        horizontalLayout_3->addLayout(mBoardAndSideBarLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 676, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        mTilesLeftLabel->setText(QApplication::translate("MainWindow", "21 tiles left to play", 0, QApplication::UnicodeUTF8));
        mPickedTileLabel->setText(QString());
        mTurnRightLabel->setText(QApplication::translate("MainWindow", "Turn CW", 0, QApplication::UnicodeUTF8));
        mTurnLeftLabel->setText(QApplication::translate("MainWindow", "Turn CCW", 0, QApplication::UnicodeUTF8));
        mErrorLabel->setText(QApplication::translate("MainWindow", "Errors and warnings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
