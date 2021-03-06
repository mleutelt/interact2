#pragma once

#include <QAbstractListModel>
#include <QColor>
#include <QtQml/qqml.h>

#include "leveldata.h"

class LevelDataModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name NOTIFY nameChanged)
  Q_PROPERTY(QUrl backgroundImage READ backgroundImage WRITE setBackgroundImage NOTIFY backgroundImageChanged)
  Q_PROPERTY(QUrl music READ music WRITE setMusic NOTIFY musicChanged)
  // TOOD: add properties for level walls

public:
  enum Roles
  {
    Type,
    BoundingBox,
    Static,
    Polygon,
    Points,
    GameItem,
    Color,
  };
  Q_ENUM(Roles)

  explicit LevelDataModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  QString name() const;

  void setLevelData(const LevelData &data);

  void addSimpleObject(int type, const QRectF &boundingRect, bool isStatic, bool gameItem, const QColor &color);
  void addPolygonObject(int type, const QPolygonF &originalPoints, const QList<QVariantList> &optimizedPoints, bool isStatic,
                        bool gameItem, const QColor &color);
  void addLineObject(int type, const QPolygonF &originalPoints, const QList<QVariantList> &lineSegments, bool isStatic,
                     bool gameItem, const QColor &color);
  void removeObject(int index);
  void clear();
  QList<ObjectDescription> objects() const;

  QUrl backgroundImage() const;
  void setBackgroundImage(const QUrl &url);

  QUrl music() const;
  void setMusic(const QUrl &url);

signals:
  void nameChanged();
  void backgroundImageChanged();
  void musicChanged();

private:
  void setName(const QString &name);

  QList<ObjectDescription> m_objects;
  QString m_name;
  QUrl m_backgroundImage;
  QUrl m_music;
};
