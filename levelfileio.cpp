#include "levelfileio.h"
#include "levelhandler.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlfio, "app.levelfileio")

LevelFileIO::LevelFileIO(QObject *parent)
  : QObject { parent }
{
}

LevelData LevelFileIO::loadLevel(const QString &path)
{
  qCInfo(lvlfio) << "reading level data from path" << path;

  QFile levelFile(path);

  if (!levelFile.open(QIODevice::ReadOnly)) {
    qCCritical(lvlfio) << "unable to read file";
    return {};
  }

  QJsonDocument document = QJsonDocument::fromJson(levelFile.readAll());

  return LevelData::fromJson(document.object());
}

bool LevelFileIO::storeLevel(const QString &path, const LevelData &data)
{
  qCInfo(lvlfio) << "storing level data at path" << path;

  QFile levelFile(path);

  if (!levelFile.open(QIODevice::WriteOnly)) {
    qCCritical(lvlfio) << "unable to write file";
    return false;
  }

  QJsonDocument document(data.toJson());

  levelFile.write(document.toJson());

  return true;
}
