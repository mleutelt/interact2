#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QTimer>

#include "levelmodel.h"
#include "leveldatamodel.h"

class LevelReader;

class LevelHandler : public QObject
{
  Q_OBJECT
  Q_PROPERTY(LevelModel *availableLevels READ availableLevels CONSTANT)
  Q_PROPERTY(LevelModel *userLevels READ userLevels CONSTANT)
  Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
  Q_PROPERTY(LevelDataModel *currentLevelData READ currentLevelData NOTIFY currentLevelDataChanged)
  QML_ELEMENT

public:
  explicit LevelHandler(QObject *parent = nullptr);

  LevelModel *availableLevels() const;
  LevelModel *userLevels() const;

  Q_INVOKABLE void loadLevel(const QString &path);

  bool loading() const;

  LevelDataModel *currentLevelData() const;

signals:
  void loadingChanged();
  void currentLevelDataChanged();

private:
  void setLoading(bool flag);

  LevelModel *m_availableLevels = nullptr;
  LevelModel *m_userLevels = nullptr;
  bool m_loading = false;
  QTimer m_loadingTimer;
  LevelDataModel *m_currentLevelData = nullptr;
  LevelReader *m_levelReader = nullptr;
};
