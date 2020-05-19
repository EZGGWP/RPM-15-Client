#include "client.h"
#include <QDir>
#include <QInputDialog>
#include <QDebug>
#include <QHostAddress>
#include <QJsonDocument>

Client::Client(QObject *parent) : QObject(parent) {
    qDebug() << "Client created";
}

void Client::readUserName()
{
    QString text;
    bool ok;
    do {
        QInputDialog i;
        text = i.getText(0, tr("What is your name?"), tr("User name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
    } while (!(ok && !text.isEmpty() && (text != "SYSTEM")));
    if (text == "BJIOM 6JI9") {
        text = "SYSTEM";
    }
    this->userName = text;
}

void Client::readAddress()
{
    QString text;
    bool ok;
    do {
        QInputDialog i;
        text = i.getText(0, tr("What is server address?"), tr("Address:"), QLineEdit::Normal, "localhost", &ok);
    } while (!(ok && !text.isEmpty()));
    this->address = text;
}

void Client::readSocket(QByteArray data) {
    qDebug() << "Binary message received!";
        qDebug() << "Response";
        //QByteArray byte = socketClient->readLine();
        QByteArray byte = data;
        qDebug() << byte;
        QJsonDocument jd = jd.fromBinaryData(byte);
        qDebug() << jd;
        QJsonObject j = jd.object();
        QString text = j["message"].toString();
        QString user = j["username"].toString();
//        if (text == '\n' || user == '\n') {
//            qDebug() << user;
//            qDebug() << text;
//            continue;
//        }
        if (user == this->userName) {
            emit enemySpotted("&lt;<font color='#55efc4'>" + user + "</font>&gt;: " + text + "</b>");
        } else {
            if (user == "SYSTEM") {
                emit enemySpotted("<font color='green'>&lt;<font color='#6c5ce7'>" + user + "</font>&gt;: " + text + "</font>");
            } else {
                emit enemySpotted("&lt;<font color='#a29bfe'>" + user + "</font>&gt;: " + text + "</b>");
            }
        }
}

void Client::writeToSocket()
{
    QJsonDocument jd(this->jo);
    qDebug() << jd;
    QByteArray ba = jd.toBinaryData();
    qDebug() << ba;
    socketClient->sendBinaryMessage(ba);

}

void Client::awareError(QAbstractSocket::SocketError lel) {
    qDebug() << lel;
};

void Client::connectToServer() {
    socketClient = new QWebSocket(this->userName);
    QUrl address = "ws://" + this->address + ":1337";
    qDebug() << address;
    socketClient->ignoreSslErrors();
    connect(socketClient, &QWebSocket::connected, this, &Client::connectionSuccessful);
    connect(socketClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(awareError(QAbstractSocket::SocketError)));
    socketClient->open(address);
}

void Client::pack(QString key, QJsonValue data)
{
    this->jo.insert(key, data);
}

void Client::handleMessage(QWebSocket* socketClient) {
    if (socketClient->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "Connected!";
        connect(socketClient, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(readSocket(QByteArray)));

    } else {
        qDebug() << socketClient->error();
        qDebug() << socketClient->errorString();
    }
};

void Client::connectionSuccessful() {
    handleMessage(socketClient);
};



