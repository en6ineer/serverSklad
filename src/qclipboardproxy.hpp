#ifndef QCLIPBOARDPROXY_HPP
#define QCLIPBOARDPROXY_HPP

#include <QObject>
#include <QClipboard>
#include <QGuiApplication>

class QClipboardProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    explicit QClipboardProxy(QClipboard* clipboard, QObject* parent = nullptr);

    QString text() const;
    void setText(const QString& text);

signals:
    void textChanged();

private:
    QClipboard* clipboard;
};

#endif // QCLIPBOARDPROXY_HPP
