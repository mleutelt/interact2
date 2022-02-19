#include "levelfileio.h"
#include "levelhandler.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlfio, "app.levelfileio")

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

LevelFileIO::LevelFileIO(QObject *parent)
  : QObject { parent }
{
}

LevelData LevelFileIO::loadLevel(const QString &path)
{
  qCInfo(lvlfio) << "reading level data from path" << path;

  QFile levelFile(path);

  LevelData levelData;

  if (!levelFile.open(QIODevice::ReadOnly)) {
    qCCritical(lvlfio) << "unable to read file";
    return levelData;
  }

  QList<ObjectDescription> objects;

  QJsonDocument document = QJsonDocument::fromJson(levelFile.readAll());
  QJsonObject level = document.object();

  const QJsonArray objectsArray = level.value(OBJECTS).toArray();
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

  levelData.name = level.value(NAME).toString();
  levelData.objects = objects;
  levelData.backgroundImage = level.value(BACKGROUND_IMAGE).toString();
  levelData.music = level.value(MUSIC).toString();
  levelData.topWallEnabled = level.value(TOP_WALL_ENABLED).toBool();
  levelData.leftWallEnabled = level.value(LEFT_WALL_ENABLED).toBool();
  levelData.rightWallEnabled = level.value(RIGHT_WALL_ENABLED).toBool();
  levelData.groundEnabled = level.value(GROUND_ENABLED).toBool();

  return levelData;
}

bool LevelFileIO::storeLevel(const QString &path, const LevelData &data)
{
  qCInfo(lvlfio) << "storing level data at path" << path;

  QFile levelFile(path);

  if (!levelFile.open(QIODevice::WriteOnly)) {
    qCCritical(lvlfio) << "unable to write file";
    return false;
  }

  QJsonObject level;
  QJsonArray objects;

  for (const ObjectDescription &object : data.objects) {
    QJsonObject jsonObject;
    jsonObject[TYPE] = object.type;
    jsonObject[X] = object.boundingBox.x();
    jsonObject[Y] = object.boundingBox.y();
    jsonObject[WIDTH] = object.boundingBox.width();
    jsonObject[HEIGHT] = object.boundingBox.height();
    jsonObject[STATIC] = object.isStatic;
    jsonObject[ROTATION] = object.rotation;
    jsonObject[GAME_ITEM] = object.gameItem;
    jsonObject[COLOR] = object.color.name(QColor::HexArgb);

    QJsonArray pointsArrayArray;
    for (const QVariantList &pointList : object.points) {
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
    for (const QPointF &point : object.polygon) {
      QJsonObject p;
      p[X] = point.x();
      p[Y] = point.y();
      polygonArray << p;
    }
    jsonObject[POLYGON] = polygonArray;

    objects << jsonObject;
  }

  level[OBJECTS] = objects;
  level[GROUND_ENABLED] = data.groundEnabled;
  level[LEFT_WALL_ENABLED] = data.leftWallEnabled;
  level[RIGHT_WALL_ENABLED] = data.rightWallEnabled;
  level[TOP_WALL_ENABLED] = data.topWallEnabled;
  level[BACKGROUND_IMAGE] = data.backgroundImage.toString();
  level[MUSIC] = data.music.toString();
  level[NAME] = data.name;

  QJsonDocument document(level);

  levelFile.write(document.toJson());

  return true;
}
