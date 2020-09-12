/********************************************************************************
** Form generated from reading UI file 'gamewindoweve.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWINDOWEVE_H
#define UI_GAMEWINDOWEVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameWindowEVE
{
public:

    void setupUi(QWidget *GameWindowEVE)
    {
        if (GameWindowEVE->objectName().isEmpty())
            GameWindowEVE->setObjectName(QStringLiteral("GameWindowEVE"));
        GameWindowEVE->resize(400, 300);

        retranslateUi(GameWindowEVE);

        QMetaObject::connectSlotsByName(GameWindowEVE);
    } // setupUi

    void retranslateUi(QWidget *GameWindowEVE)
    {
        GameWindowEVE->setWindowTitle(QApplication::translate("GameWindowEVE", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GameWindowEVE: public Ui_GameWindowEVE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWINDOWEVE_H
