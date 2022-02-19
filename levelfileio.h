#pragma once

#include <QObject>

#include "leveldata.h"

class LevelFileIO : public QObject
{
  Q_OBJECT

public:
  explicit LevelFileIO(QObject *parent = nullptr);

  static LevelData loadLevel(const QString &path);
  static bool storeLevel(const QString &path, const LevelData &data);
};
