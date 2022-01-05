#pragma once

#include <QAbstractListModel>
#include <QtQml/qqml.h>

#include "leveldata.h"

class LevelDataModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name NOTIFY nameChanged)
  Q_PROPERTY(QUrl backgroundImage READ backgroundImage WRITE setBackgroundImage NOTIFY backgroundImageChanged)
  // TOOD: add properties for level walls

public:
  enum Roles
  {
    Type,
    BoundingBox,
    Static,
  };
  Q_ENUM(Roles)

  explicit LevelDataModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  QString name() const;

  void setLevelData(const LevelData &data);

  void addObject(int type, const QRect &boundingRect, bool isStatic, int rotation);
  void removeObject(int index);
  void clear();
  QList<ObjectDescription> objects() const;

  QUrl backgroundImage() const;
  void setBackgroundImage(const QUrl &url);

signals:
  void nameChanged();
  void backgroundImageChanged();

private:
  void setName(const QString &name);

  QList<ObjectDescription> m_objects;
  QString m_name;
  QUrl m_backgroundImage;
};
