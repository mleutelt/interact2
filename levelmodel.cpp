#include "levelmodel.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlm, "app.models.levelmodel")

LevelModel::LevelModel(QObject *parent) : QAbstractListModel { parent }
{
}

int LevelModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)

  return m_data.count();
}

QVariant LevelModel::data(const QModelIndex &index, int role) const
{
  if (index.row() < 0 || index.row() >= m_data.count())
    return QVariant();

  const LevelDescription &data = m_data[index.row()];

  switch (role) {
  case Name:
    return data.name;
  case Preview:
    return data.preview;
  case Path:
    return data.path;
  }

  return QVariant();
}

QHash<int, QByteArray> LevelModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[Name] = "name";
  roles[Preview] = "preview";
  roles[Path] = "path";

  return roles;
}

void LevelModel::addLevel(const LevelDescription &level)
{
  beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
  m_data << level;
  endInsertRows();
}

void LevelModel::clear()
{
  beginResetModel();
  m_data.clear();
  endResetModel();
}
