#pragma once

#include <QAbstractListModel>

#include "leveldata.h"

class LevelDataModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name NOTIFY nameChanged)

public:
  // TODO: figure out what we actually need here
  enum Roles
  {
  };
  Q_ENUM(Roles)

  explicit LevelDataModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  QString name() const;

  void setLevelData(const LevelData &data);

signals:
  void nameChanged();

private:
  void setName(const QString &name);

  QList<LevelData> m_data;
  QString m_name;
};
