#include "levelfileio.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlr, "app.levelfileio")

LevelFileIO::LevelFileIO(QObject *parent) : QObject { parent }
{
}

QList<ObjectDescription> LevelFileIO::loadLevelFromPath(const QString &path)
{
  qCInfo(lvlr) << "reading level data from path" << path;

  // TODO: implement reading json file in folder
  return QList<ObjectDescription>();
}
