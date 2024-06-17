#include "barcodesdata.h"

BarcodesData::BarcodesData(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int BarcodesData::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

int BarcodesData::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3; // Колонки: barcode, quantity, comment
}

QVariant BarcodesData::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const BarcodeItem &item = m_data[index.row()];

    switch (role) {
    case BarcodeRole:
        return item.barcode;
    case QuantityRole:
        return item.quantity;
    case CommentRole:
        return item.comment;
    default:
        return QVariant();
    }
}

QVariant BarcodesData::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
    case 0:
        return "Штрихкод";
    case 1:
        return "Количество";
    case 2:
        return "Комментарий";
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BarcodesData::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[BarcodeRole] = "barcode";
    roles[QuantityRole] = "quantity";
    roles[CommentRole] = "comment";
    return roles;
}

void BarcodesData::addRow(const QString &barcode, int quantity, const QString &comment)
{
    // Проверяем, существует ли строка с таким штрихкодом
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].barcode == barcode) {
            // Если строка найдена, увеличиваем количество
            m_data[i].quantity += quantity;
            // Уведомляем об изменении данных в строке
            emit dataChanged(index(i, 0), index(i, 1), { QuantityRole });
            return;
        }
    }

    // Если штрихкод не найден, добавляем новую строку
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append({ barcode, quantity, comment });
    endInsertRows();
}

void BarcodesData::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}
