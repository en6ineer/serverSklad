#include "httpserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkInterface>
#include <QDebug>

HttpServer::HttpServer(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &HttpServer::onNewConnection);
}

void HttpServer::startServer()
{
    if (!tcpServer->listen(QHostAddress::Any, 8080)) {
        qCritical() << "Unable to start the server:" << tcpServer->errorString();
    } else {
        qDebug() << "Server started, listening on port 8080";
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost) {
                serverUrl = "http://" + address.toString() + ":8080/sklad";
                emit serverUrlChanged();
                qDebug() << "Server URL:" << serverUrl;
                break;
            }
        }
    }
}

QString HttpServer::getUrl() const
{
    return serverUrl;
}

void HttpServer::onNewConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &HttpServer::onReadyRead);
}

void HttpServer::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;

    QByteArray requestData = clientSocket->readAll();
    QString requestString(requestData);

    if (requestString.contains("POST /sklad")) {
        int jsonStartIndex = requestString.indexOf("["); // Изменено на поиск '[' для массива JSON
        int jsonEndIndex = requestString.lastIndexOf("]");
        QString jsonString = requestString.mid(jsonStartIndex, jsonEndIndex - jsonStartIndex + 1);

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
        if (!jsonDoc.isNull() && jsonDoc.isArray()) { // Проверяем, что JSON является массивом
            QJsonArray jsonArray = jsonDoc.array();
            for (const QJsonValue &value : jsonArray) {
                if (value.isObject()) {
                    QJsonObject jsonObject = value.toObject();
                    QString barcode = jsonObject["barcode"].toString();
                    int quantity = jsonObject["quantity"].toInt();
                    QString comment = jsonObject["comment"].toString();
                    emit newPostRequest(QString("{\"barcode\":\"%1\", \"quantity\":%2, \"comment\":\"%3\"}")
                                            .arg(barcode).arg(quantity).arg(comment));
                }
            }
            QString response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"message\": \"Структура успешно получена\"}\r\n";
            clientSocket->write(response.toUtf8());
        } else {
            QString response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nInvalid JSON\r\n";
            clientSocket->write(response.toUtf8());
        }
    } else {
        QString response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found\r\n";
        clientSocket->write(response.toUtf8());
    }
    clientSocket->disconnectFromHost();
}
