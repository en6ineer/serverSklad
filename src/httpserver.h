#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);
    void startServer();

signals:
    void newPostRequest(const QString &data);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer *tcpServer;
};

#endif // HTTPSERVER_H
