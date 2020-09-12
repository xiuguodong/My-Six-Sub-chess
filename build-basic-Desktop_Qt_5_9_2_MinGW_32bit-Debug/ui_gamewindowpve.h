/********************************************************************************
** Form generated from reading UI file 'gamewindowpve.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWINDOWPVE_H
#define UI_GAMEWINDOWPVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameWindowPVE
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GameWindowPVE)
    {
        if (GameWindowPVE->objectName().isEmpty())
            GameWindowPVE->setObjectName(QStringLiteral("GameWindowPVE"));
        GameWindowPVE->resize(800, 600);
        menubar = new QMenuBar(GameWindowPVE);
        menubar->setObjectName(QStringLiteral("menubar"));
        GameWindowPVE->setMenuBar(menubar);
        centralwidget = new QWidget(GameWindowPVE);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        GameWindowPVE->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(GameWindowPVE);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        GameWindowPVE->setStatusBar(statusbar);

        retranslateUi(GameWindowPVE);

        QMetaObject::connectSlotsByName(GameWindowPVE);
    } // setupUi

    void retranslateUi(QMainWindow *GameWindowPVE)
    {
        GameWindowPVE->setWindowTitle(QApplication::translate("GameWindowPVE", "MainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GameWindowPVE: public Ui_GameWindowPVE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWINDOWPVE_H
