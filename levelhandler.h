#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QTimer>
#include <QDir>

#include "levelmodel.h"
#include "leveldatamodel.h"

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

  bool loading() const;

  LevelDataModel *currentLevelData() const;

  static QDir userLevelsDirectory();
  static QString levelPreviewFileName();
  static QString levelDataFileName();

  Q_INVOKABLE void loadLevel(const QString &path);
  Q_INVOKABLE void updateUserLevelsModel();

signals:
  void loadingChanged();
  void currentLevelDataChanged();

private:
  void setLoading(bool flag);
  void updateGameLevelsModel();

  LevelModel *m_availableLevels = nullptr;
  LevelModel *m_userLevels = nullptr;
  bool m_loading = false;
  QTimer m_loadingTimer;
  LevelDataModel *m_currentLevelData = nullptr;
};
