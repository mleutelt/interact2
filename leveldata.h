#pragma once

#include <QObject>

class ObjectDescription
{
  Q_GADGET
};

class LevelData
{
  Q_GADGET

public:
  QString name;
  QList<ObjectDescription> objects;
};
