#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
    void sig_SendNode(QTreeWidgetItem*);    // Signals are just constructs used to pass data. They are not callable like functions.
                                            // However, they can be "emitted" by saying emit Signal_Name, as can be seen in the
                                            // constructor of MainWindow.
                                            // For organizational purposes, I like to prefix all my signal names with sig_.
};

#endif // MAINWINDOW_H
