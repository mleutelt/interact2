#include "leveldatamodel.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvldm, "app.models.leveldatamodel")

LevelDataModel::LevelDataModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

int LevelDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_data.count();
}

QVariant LevelDataModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_data.count())
        return QVariant();

    return QVariant();
}

QHash<int, QByteArray> LevelDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;


    return roles;
}

QString LevelDataModel::name() const
{
    return m_name;
}

void LevelDataModel::setLevelData(const LevelData &data)
{
    setName(data.name);
}

void LevelDataModel::setName(const QString &name)
{
    if (m_name == name) {
        return;
    }

    m_name = name;
    emit nameChanged();
}
