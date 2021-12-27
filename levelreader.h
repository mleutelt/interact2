#pragma once

#include <QObject>

#include "leveldata.h"

class LevelReader : public QObject
{
  Q_OBJECT

public:
  explicit LevelReader(QObject *parent = nullptr);

  QList<ObjectDescription> loadLevelFromPath(const QString &path);
};
