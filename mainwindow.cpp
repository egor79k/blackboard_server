#include "mainwindow.h"
#include "ui_server.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    log = new TextBrowser_Logger(ui->log_textBrowser);
    serv.setLogger(log);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete log;
}

void MainWindow::on_Listening_checkBox_stateChanged(int arg1)
{
    if (arg1)
        serv.startListening(QHostAddress(ui->IP_lineEdit->text()), (ui->Port_lineEdit->text().toInt()));
    else
        serv.stopListening();
}
