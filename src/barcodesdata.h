#ifndef BARCODESDATA_H
#define BARCODESDATA_H

#include <QObject>
#include <QAbstractTableModel>
#include <QList>

struct BarcodeItem {
    QString barcode;
    QString quantity;
    QString comment;
};

class BarcodesData : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum BarcodeRoles {
        BarcodeRole = Qt::UserRole + 1,
        QuantityRole,
        CommentRole
    };

    explicit BarcodesData(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addRow(const QString &barcode, QString quantity, const QString &comment = QString());
    Q_INVOKABLE void clear();
    Q_INVOKABLE QString getColumnData(int column) const; // Новый метод для получения данных из колонки


private:
    QList<BarcodeItem> m_data;
};

#endif // BARCODESDATA_H
