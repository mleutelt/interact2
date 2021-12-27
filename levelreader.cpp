#include "levelreader.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlr, "app.levelreader")

LevelReader::LevelReader(QObject *parent) : QObject { parent }
{
}

QList<ObjectDescription> LevelReader::loadLevelFromPath(const QString &path)
{
  qCInfo(lvlr) << "reading level data from path" << path;

  // TODO: implement reading json file in folder
  return QList<ObjectDescription>();
}
