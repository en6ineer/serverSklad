#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QClipboard>
#include "httpserver.h"
#include "barcodesdata.h"
#include "qclipboardproxy.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    HttpServer server;


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("httpServer", &server);

    BarcodesData barcodesData;
    engine.rootContext()->setContextProperty("barcodesData", &barcodesData);

    // Создание и регистрация экземпляра QClipboardProxy
    QClipboardProxy clipboardProxy(QGuiApplication::clipboard());
    engine.rootContext()->setContextProperty("clipboard", &clipboardProxy);

    const QUrl url(u"qrc:/qt/qml/Main/main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    server.startServer();

    return app.exec();
}
