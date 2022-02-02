#include "levelfileio.h"
#include "levelhandler.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlfio, "app.levelfileio")

LevelFileIO::LevelFileIO(QObject *parent)
  : QObject { parent }
{
}

LevelData LevelFileIO::loadLevelFromPath(const QString &path)
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

  const QJsonArray objectsArray = level.value(u"objects"_qs).toArray();
  for (const QJsonValue &object : objectsArray) {
    QJsonObject o = object.toObject();

    ObjectDescription objectDescription;
    objectDescription.type = o.value(u"type"_qs).toInt();
    objectDescription.boundingBox = {
      o.value(u"x"_qs).toDouble(),
      o.value(u"y"_qs).toDouble(),
      o.value(u"width"_qs).toDouble(),
      o.value(u"height"_qs).toDouble(),
    };
    objectDescription.isStatic = o.value(u"static"_qs).toBool();
    objectDescription.rotation = o.value(u"rotation"_qs).toInt();
    objectDescription.invisible = o.value(u"invisible"_qs).toBool();
    objectDescription.gameItem = o.value(u"gameItem"_qs).toBool();

    QJsonArray pointsArrayArray = o.value(u"points"_qs).toArray();
    QList<QVariantList> points;
    for (const QJsonValue &pointList : pointsArrayArray) {
      QVariantList pointsArray;
      for (const QJsonValue &point : pointList.toArray()) {
        QPointF p(point[u"x"_qs].toDouble(), point[u"y"_qs].toDouble());
        pointsArray << p;
      }
      points << pointsArray;
    }
    objectDescription.points = points;

    QJsonArray polygonArray = o.value(u"polygon"_qs).toArray();
    QPolygonF polygon;
    for (const QJsonValue &point : polygonArray) {
      QPointF p(point[u"x"_qs].toDouble(), point[u"y"_qs].toDouble());
      polygon << p;
    }
    objectDescription.polygon = polygon;

    objects << objectDescription;
  }

  levelData.name = QFileInfo(path).dir().dirName();
  levelData.objects = objects;
  levelData.backgroundImage = level.value(u"backgroundImage"_qs).toString();
  levelData.music = level.value(u"music"_qs).toString();
  levelData.topWallEnabled = level.value(u"topWallEnabled"_qs).toBool();
  levelData.leftWallEnabled = level.value(u"leftWallEnabled"_qs).toBool();
  levelData.rightWallEnabled = level.value(u"rightWallEnabled"_qs).toBool();
  levelData.groundEnabled = level.value(u"groundEnabled"_qs).toBool();

  return levelData;
}

bool LevelFileIO::storeLevelAtPath(const QString &path, const LevelData &data)
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
    jsonObject[u"type"_qs] = object.type;
    jsonObject[u"x"_qs] = object.boundingBox.x();
    jsonObject[u"y"_qs] = object.boundingBox.y();
    jsonObject[u"width"_qs] = object.boundingBox.width();
    jsonObject[u"height"_qs] = object.boundingBox.height();
    jsonObject[u"static"_qs] = object.isStatic;
    jsonObject[u"rotation"_qs] = object.rotation;
    jsonObject[u"invisible"_qs] = object.invisible;
    jsonObject[u"gameItem"_qs] = object.gameItem;

    QJsonArray pointsArrayArray;
    for (const QVariantList &pointList : object.points) {
      QJsonArray pointsArray;
      for (const QVariant &point : pointList) {
        QJsonObject p;
        p[u"x"_qs] = point.toPointF().x();
        p[u"y"_qs] = point.toPointF().y();
        pointsArray << p;
      }
      pointsArrayArray << pointsArray;
    }
    jsonObject[u"points"_qs] = pointsArrayArray;

    QJsonArray polygonArray;
    for (const QPointF &point : object.polygon) {
      QJsonObject p;
      p[u"x"_qs] = point.x();
      p[u"y"_qs] = point.y();
      polygonArray << p;
    }
    jsonObject[u"polygon"_qs] = polygonArray;

    objects << jsonObject;
  }

  level[u"objects"_qs] = objects;
  level[u"groundEnabled"_qs] = data.groundEnabled;
  level[u"leftWallEnabled"_qs] = data.leftWallEnabled;
  level[u"rightWallEnabled"_qs] = data.rightWallEnabled;
  level[u"topWallEnabled"_qs] = data.topWallEnabled;
  level[u"backgroundImage"_qs] = data.backgroundImage.toString();
  level[u"music"_qs] = data.music.toString();

  QJsonDocument document(level);

  levelFile.write(document.toJson());

  return true;
}
