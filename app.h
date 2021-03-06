#pragma once

#include <QObject>
#include <QtQml/qqml.h>

#include "editor.h"
#include "levelhandler.h"

class App : public QObject
{
  Q_OBJECT
  Q_PROPERTY(Editor *editor READ editor CONSTANT)
  Q_PROPERTY(LevelHandler *levelHandler READ levelHandler CONSTANT)
  Q_PROPERTY(bool debugMode READ debugMode WRITE setDebugMode NOTIFY debugModeChanged)
  Q_PROPERTY(bool debugBox2d READ debugBox2d WRITE setDebugBox2d NOTIFY debugBox2dChanged)
  Q_PROPERTY(QString musicPath READ musicPath CONSTANT)
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit App(QObject *parent = nullptr);

  Editor *editor() const;
  LevelHandler *levelHandler() const;

  bool debugMode() const;
  void setDebugMode(bool flag);

  bool debugBox2d() const;
  void setDebugBox2d(bool flag);

  QString musicPath() const;

signals:
  void debugModeChanged();
  void debugBox2dChanged();

private:
  void initialize();

  LevelHandler *m_levelHandler = nullptr;
  Editor *m_editor = nullptr;
  bool m_debugMode = false;
  bool m_debugBox2d = false;
};
