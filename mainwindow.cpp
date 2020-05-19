#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    cl = new Client(this);
    cl->readUserName();
    this->setWindowTitle(cl->userName);

    cl->readAddress();

    connect(cl, &Client::cannotConnect, this, &MainWindow::connectUnsuccess);
    cl->connectToServer();

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::sendButtonClick);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::sendButtonClick);
    connect(cl, &Client::enemySpotted, ui->textEdit, &QTextEdit::append);
    //connect(cl->socketClient, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::handleError);
}

MainWindow::~MainWindow() {
    cl->socketClient->close();
    delete ui;
}

void MainWindow::sendButtonClick()
{
    if (ui->lineEdit->text() == "") {
        qDebug() << "Empty message";
        return;
    }
    qDebug() << "Message to json";
    if (!(ui->lineEdit->text()[0] == "~")) {
        cl->pack("username", cl->userName);
        cl->pack("message", ui->lineEdit->text());
        qDebug() << cl->jo;
        cl->writeToSocket();
    } else {
        if (ui->lineEdit->text() == "~exit") {
            exit(1);
        } else {
            if (ui->lineEdit->text() == "~reconnect") {
                cl->socketClient->close();
                delete cl->socketClient;
                cl->connectToServer();
            } else {
                qDebug() << "Command not found";
            }
        }
    }
    ui->lineEdit->setText("");
}

void MainWindow::connectUnsuccess()
{
    ui->textEdit->setText("<font color='red'>Cannot connect to server</font>");
}

void MainWindow::handleError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
    connectUnsuccess();
}



