#pragma once

#include <QObject>
#include <QRectF>
#include <QPolygonF>
#include <QUrl>
#include <QDebug>

class ObjectDescription
{
  Q_GADGET

public:
  int type = 0;
  QRectF boundingBox;
  QPolygonF polygon;
  // NOTE: Box2d expects the vertices of a polygon as a QVariantList
  QList<QVariantList> points;
  bool isStatic = false;
  int rotation = 0;
};

inline QDebug operator<<(QDebug debug, const ObjectDescription &o)
{
  debug << "ObjectDescription(" << o.type << o.boundingBox.x() << o.boundingBox.y() << o.boundingBox.width()
        << o.boundingBox.height() << o.isStatic << ")";
  return debug;
}

class LevelData
{
  Q_GADGET

public:
  QString name;
  QUrl backgroundImage;
  QUrl music;
  QList<ObjectDescription> objects;
  bool groundEnabled = true;
  bool leftWallEnabled = true;
  bool rightWallEnabled = true;
  bool topWallEnabled = true;
};
