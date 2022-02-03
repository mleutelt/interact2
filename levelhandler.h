#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QTimer>
#include <QDir>

#include "levelmodel.h"
#include "leveldatamodel.h"
#include "ilevelmanager.h"

class QQuickItemGrabResult;

class LevelHandler : public ILevelManager
{
  Q_OBJECT
  Q_PROPERTY(LevelModel *availableLevels READ availableLevels CONSTANT)
  Q_PROPERTY(LevelModel *userLevels READ userLevels CONSTANT)
  Q_PROPERTY(LevelDataModel *currentLevelData READ levelData NOTIFY levelDataChanged)
  QML_ELEMENT
  QML_IMPLEMENTS_INTERFACES(ILevelManager)

public:
  explicit LevelHandler(QObject *parent = nullptr);

  LevelModel *availableLevels() const;
  LevelModel *userLevels() const;

  static QDir userLevelsDirectory();
  static QString levelPreviewFileName();
  static QString levelDataFileName();

  Q_INVOKABLE void updateUserLevelsModel();
  Q_INVOKABLE void nextLevel();
  Q_INVOKABLE void deleteLevel(int index);

protected:
  void loadLevel(const QString &path) override;
  void saveLevel(const QString &name, QQuickItemGrabResult *screenshot) override;
  void resetLevel() override;

  LevelDataModel *levelData() const override;

private:
  void updateGameLevelsModel();

  LevelModel *m_availableLevels = nullptr;
  LevelModel *m_userLevels = nullptr;
  LevelDataModel *m_currentLevelData = nullptr;
};
