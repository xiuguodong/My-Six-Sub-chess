#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void dealSubpeopeo();

private slots:
    void on_bntpeopeo_clicked();

    void on_bntpeonet_clicked();

    void on_bntnetnet_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
