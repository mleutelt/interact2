#pragma once

#include <QObject>

#include "leveldata.h"

class LevelFileIO : public QObject
{
  Q_OBJECT

public:
  explicit LevelFileIO(QObject *parent = nullptr);

  static QList<ObjectDescription> loadLevelFromPath(const QString &path);
  static bool storeLevelAtPath(const QString &path, const LevelData &data);
};
