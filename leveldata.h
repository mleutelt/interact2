#pragma once

#include <QObject>
#include <QRectF>
#include <QPolygonF>
#include <QUrl>
#include <QColor>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

namespace {
  const QString TYPE = u"type"_qs;
  const QString X = u"x"_qs;
  const QString Y = u"y"_qs;
  const QString WIDTH = u"width"_qs;
  const QString HEIGHT = u"height"_qs;
  const QString STATIC = u"static"_qs;
  const QString ROTATION = u"rotation"_qs;
  const QString GAME_ITEM = u"gameItem"_qs;
  const QString COLOR = u"color"_qs;
  const QString POLYGON = u"polygon"_qs;
  const QString OBJECTS = u"objects"_qs;
  const QString POINTS = u"points"_qs;
  const QString NAME = u"name"_qs;
  const QString BACKGROUND_IMAGE = u"backgroundImage"_qs;
  const QString MUSIC = u"music"_qs;
  const QString TOP_WALL_ENABLED = u"topWallEnabled"_qs;
  const QString LEFT_WALL_ENABLED = u"leftWallEnabled"_qs;
  const QString RIGHT_WALL_ENABLED = u"rightWallEnabled"_qs;
  const QString GROUND_ENABLED = u"groundEnabled"_qs;
}

class ObjectDescription
{
  Q_GADGET

public:
  inline QJsonObject toJson() const
  {
    QJsonObject jsonObject;
    jsonObject[TYPE] = type;
    jsonObject[X] = boundingBox.x();
    jsonObject[Y] = boundingBox.y();
    jsonObject[WIDTH] = boundingBox.width();
    jsonObject[HEIGHT] = boundingBox.height();
    jsonObject[STATIC] = isStatic;
    jsonObject[ROTATION] = rotation;
    jsonObject[GAME_ITEM] = gameItem;
    jsonObject[COLOR] = color.name(QColor::HexArgb);

    QJsonArray pointsArrayArray;
    for (const QVariantList &pointList : points) {
      QJsonArray pointsArray;
      for (const QVariant &point : pointList) {
        QJsonObject p;
        p[X] = point.toPointF().x();
        p[Y] = point.toPointF().y();
        pointsArray << p;
      }
      pointsArrayArray << pointsArray;
    }
    jsonObject[POINTS] = pointsArrayArray;

    QJsonArray polygonArray;
    for (const QPointF &point : polygon) {
      QJsonObject p;
      p[X] = point.x();
      p[Y] = point.y();
      polygonArray << p;
    }
    jsonObject[POLYGON] = polygonArray;

    return jsonObject;
  }

  int type = 0;
  QRectF boundingBox;
  QPolygonF polygon;
  // NOTE: Box2d expects the vertices of a polygon as a QVariantList
  QList<QVariantList> points;
  bool isStatic = false;
  bool gameItem = false;
  QColor color;
  int rotation = 0;
};

inline QDebug operator<<(QDebug debug, const ObjectDescription &o)
{
  debug << "ObjectDescription(" << o.type << o.boundingBox.x() << o.boundingBox.y() << o.boundingBox.width()
        << o.boundingBox.height() << o.isStatic << o.gameItem << o.color << ")";
  return debug;
}

inline QJsonArray &operator<<(QJsonArray &array, const ObjectDescription &o)
{
  array << o.toJson();
  return array;
}

class LevelData
{
  Q_GADGET

public:
  inline QJsonObject toJson() const
  {
    QJsonObject result;
    result[GROUND_ENABLED] = groundEnabled;
    result[LEFT_WALL_ENABLED] = leftWallEnabled;
    result[RIGHT_WALL_ENABLED] = rightWallEnabled;
    result[TOP_WALL_ENABLED] = topWallEnabled;
    result[BACKGROUND_IMAGE] = backgroundImage.toString();
    result[MUSIC] = music.toString();
    result[NAME] = name;

    QJsonArray objectsArray;
    for (const ObjectDescription &object : objects) {
      objectsArray << object;
    }
    result[OBJECTS] = objectsArray;

    return result;
  }

  static LevelData fromJson(const QJsonObject &json)
  {
    LevelData levelData;
    QList<ObjectDescription> objects;

    const QJsonArray objectsArray = json.value(OBJECTS).toArray();
    for (const QJsonValue &object : objectsArray) {
      QJsonObject o = object.toObject();

      ObjectDescription objectDescription;
      objectDescription.type = o.value(TYPE).toInt();
      objectDescription.boundingBox = {
        o.value(X).toDouble(),
        o.value(Y).toDouble(),
        o.value(WIDTH).toDouble(),
        o.value(HEIGHT).toDouble(),
      };
      objectDescription.isStatic = o.value(STATIC).toBool();
      objectDescription.rotation = o.value(ROTATION).toInt();
      objectDescription.gameItem = o.value(GAME_ITEM).toBool();
      objectDescription.color = o.value(COLOR).toString();

      QJsonArray pointsArrayArray = o.value(POINTS).toArray();
      QList<QVariantList> points;
      for (const QJsonValue &pointList : pointsArrayArray) {
        QVariantList pointsArray;
        for (const QJsonValue &point : pointList.toArray()) {
          QPointF p(point[X].toDouble(), point[Y].toDouble());
          pointsArray << p;
        }
        points << pointsArray;
      }
      objectDescription.points = points;

      QJsonArray polygonArray = o.value(POLYGON).toArray();
      QPolygonF polygon;
      for (const QJsonValue &point : polygonArray) {
        QPointF p(point[X].toDouble(), point[Y].toDouble());
        polygon << p;
      }
      objectDescription.polygon = polygon;

      objects << objectDescription;
    }

    levelData.name = json.value(NAME).toString();
    levelData.objects = objects;
    levelData.backgroundImage = json.value(BACKGROUND_IMAGE).toString();
    levelData.music = json.value(MUSIC).toString();
    levelData.topWallEnabled = json.value(TOP_WALL_ENABLED).toBool();
    levelData.leftWallEnabled = json.value(LEFT_WALL_ENABLED).toBool();
    levelData.rightWallEnabled = json.value(RIGHT_WALL_ENABLED).toBool();
    levelData.groundEnabled = json.value(GROUND_ENABLED).toBool();

    return levelData;
  }

  QString name;
  QUrl backgroundImage;
  QUrl music;
  QList<ObjectDescription> objects;
  bool groundEnabled = true;
  bool leftWallEnabled = true;
  bool rightWallEnabled = true;
  bool topWallEnabled = true;
};
