#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QWidget>
#include <random>
#include <ctime>
#include <QVector>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class appWindow; }
QT_END_NAMESPACE

class appWindow : public QWidget
{
    Q_OBJECT

public:
    appWindow(QWidget *parent = nullptr);
    ~appWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void setReach(int value);
private:
    Ui::appWindow *ui;
    Graph graph;
};
#endif // APPWINDOW_H
