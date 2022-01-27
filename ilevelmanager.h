#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QQuickItemGrabResult>

class LevelDataModel;

class ILevelManager : public QObject
{
  Q_OBJECT
  QML_INTERFACE

public:
  ILevelManager(QObject *parent = nullptr)
    : QObject(parent)
  {
  }

  Q_INVOKABLE virtual void loadLevel(const QString &path) = 0;
  Q_INVOKABLE virtual void saveLevel(const QString &name, QQuickItemGrabResult *screenshot) = 0;
  Q_INVOKABLE virtual void resetLevel() = 0;

  virtual LevelDataModel *levelData() const = 0;

signals:
  void levelSavedSuccessfully();
  void levelLoadedSuccessfully();
  void levelDataChanged();
};

Q_DECLARE_INTERFACE(ILevelManager, "org.Wurstcrew.ILevelManager")
