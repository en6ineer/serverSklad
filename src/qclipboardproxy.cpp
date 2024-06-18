#include "qclipboardproxy.hpp"

QClipboardProxy::QClipboardProxy(QClipboard* c, QObject* parent)
    : QObject(parent), clipboard(c)
{
    connect(c, &QClipboard::dataChanged, this, &QClipboardProxy::textChanged);
}

QString QClipboardProxy::text() const
{
    return clipboard->text();
}

void QClipboardProxy::setText(const QString& text)
{
    clipboard->setText(text);
    emit textChanged();
}
