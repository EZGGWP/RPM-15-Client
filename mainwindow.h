#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Client* cl; /// клиент-класс для реализации логики
    /**
     * @brief MainWindow конструктор
     * @param parent родительский объект
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    /**
     * @brief sendButtonClick функция, реализующая действия при нажатии клавиши "Send"
     */
    void sendButtonClick();
    /**
     * @brief connectUnsuccess функция, реализующая действия при невозможности подключения к серверу
     */
    void connectUnsuccess();
    /**
     * @brief handleError функция-обработчик ошибки сокета
     * @param socketError ошибка сокета
     */
    void handleError(QAbstractSocket::SocketError socketError);
private:
    /**
     * @brief ui юай
     */
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
