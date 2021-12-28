#pragma once

#include <QObject>

#include "leveldata.h"

class LevelFileIO : public QObject
{
  Q_OBJECT

public:
  explicit LevelFileIO(QObject *parent = nullptr);

  QList<ObjectDescription> loadLevelFromPath(const QString &path);
};