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

QList<ObjectDescription> LevelFileIO::loadLevelFromPath(const QString &path)
{
  qCInfo(lvlfio) << "reading level data from path" << path;

  QFile levelFile(path);

  if (!levelFile.open(QIODevice::ReadOnly)) {
    qCCritical(lvlfio) << "unable to read file";
    return QList<ObjectDescription>();
  }

  QList<ObjectDescription> result;

  QJsonDocument document = QJsonDocument::fromJson(levelFile.readAll());
  QJsonObject level = document.object();

  const QJsonArray objectsArray = level.value(u"objects"_qs).toArray();
  for (const QJsonValue &object : objectsArray) {
    QJsonObject o = object.toObject();

    ObjectDescription objectDescription;
    objectDescription.type = o.value(u"type"_qs).toInt();
    objectDescription.boundingBox = {
      o.value(u"x"_qs).toInt(),
      o.value(u"y"_qs).toInt(),
      o.value(u"width"_qs).toInt(),
      o.value(u"height"_qs).toInt(),
    };
    objectDescription.isStatic = o.value(u"static"_qs).toBool();
    objectDescription.rotation = o.value(u"rotation"_qs).toInt();

    result << objectDescription;
  }

  return result;
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

    objects << jsonObject;
  }

  level[u"objects"_qs] = objects;
  level[u"groundEnabled"_qs] = data.groundEnabled;
  level[u"leftWallEnabled"_qs] = data.leftWallEnabled;
  level[u"rightWallEnabled"_qs] = data.rightWallEnabled;
  level[u"topWallEnabled"_qs] = data.topWallEnabled;

  QJsonDocument document(level);

  levelFile.write(document.toJson());

  return true;
}
