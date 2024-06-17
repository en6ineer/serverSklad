#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHostAddress>

class HttpServer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverUrl READ getUrl NOTIFY serverUrlChanged)

public:
    explicit HttpServer(QObject *parent = nullptr);
    void startServer();
    QString getUrl() const;

signals:
    void newPostRequest(const QString &data);
    void serverUrlChanged();

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer *tcpServer;
    QString serverUrl;
};

#endif // HTTPSERVER_H
