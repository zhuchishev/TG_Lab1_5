#include "appwindow.h"
#include "ui_appwindow.h"
#include <cstdlib>

appWindow::appWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::appWindow)
    , graph(0)
{
    ui->setupUi(this);
}

appWindow::~appWindow()
{
    delete ui;
}


void appWindow::on_pushButton_clicked()
{
    graph = Graph(ui->spinBox->value());
    QString message;
    if ((ui->spinBox->value() < 2) || (ui->spinBox->value() > 50)) {
        message.append("Ошибка: в графе должно быть от 2-х до 50-ти вершин");
        ui->textEdit->append(message);
        ui->tableWidget->setColumnCount(0);
        ui->tableWidget->setRowCount(0);
        graph.setIsCreated(false);
        ui->label_2->clear();
        ui->label_2->setText("Граф не создан");
        ui->comboBox_2->clear();
        ui->comboBox_2->addItem("0");
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("0");
        ui->label_8->setText("0");
    }
    else {
        ui->tableWidget->setColumnCount(ui->spinBox->value());
        ui->tableWidget->setRowCount(ui->spinBox->value());
        graph.fillMatrixAdj(ui->spinBox->value());
        QString temp;
        for (int i = 0; i < ui->spinBox->value(); i++) {
            for (int j = 0; j < ui->spinBox->value(); j++) {
                ui->tableWidget->setItem(i, j, new QTableWidgetItem(temp.setNum(graph.getMatrixAdjElement(i, j))));
            }
        }
        message.append("Граф создан");
        ui->textEdit->append(message);
        graph.writeDotFile("C:\\pictures\\graph.dot");
        system("dot C:\\pictures\\graph.dot -Tpng -Gdpi=300 -o C:\\pictures\\graph.png");
        QPixmap * mypix = new QPixmap("C:\\pictures\\graph.png");
        ui->label_2->setPixmap(mypix->scaled(800, 800, Qt::KeepAspectRatio));
        graph.fillMatrixReach();
        ui->comboBox_2->clear();
        ui->comboBox_3->clear();
        QString intBuf;
        for (int i = 0; i < this->graph.getDimension(); i++) {
            intBuf.setNum(i + 1);
            ui->comboBox_2->addItem(intBuf);
            ui->comboBox_3->addItem(intBuf);
        }
    }
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(0);
}

void appWindow::on_pushButton_2_clicked()
{
    QString message;
    if (!graph.getIsCreated()) {
        message.append("Ошибка: граф не создан");
        ui->textEdit->append(message);
        return;
    }
    if (ui->spinBox_2->value() == 0) {
        message.append("Ошибка: длина маршрутов должна быть больше 0 и не больше количества вершин");
        ui->textEdit->append(message);
        return;
    }
    if (ui->spinBox_2->value() > graph.getDimension()) {
        message.append("Ошибка: длина маршрутов должна быть больше 0 и не больше количества вершин");
        ui->textEdit->append(message);
        return;
    }
    graph.fillMatrixShim(ui->spinBox_2->value(), ui->comboBox->currentIndex());
    ui->tableWidget_2->setRowCount(graph.getDimension());
    ui->tableWidget_2->setColumnCount(graph.getDimension());
    QString temp;
    for (int i = 0; i < graph.getDimension(); i++) {
        for (int j = 0; j < graph.getDimension(); j++) {
            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(temp.setNum(graph.getMatrixShimElement(i, j))));
        }
    }
    message.append("Матрица метода Шимбелла создана");
    ui->textEdit->append(message);
}

void appWindow::setReach(int value)
{
    if ((ui->comboBox_2->currentIndex() != -1) && (ui->comboBox_3->currentIndex() != -1) && (this->graph.getIsCreated())) {
        ui->label_8->setNum(this->graph.getMatrixReachElement(ui->comboBox_2->currentIndex(), ui->comboBox_3->currentIndex()));
    }
}
