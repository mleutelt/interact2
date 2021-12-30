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
  Q_PROPERTY(bool debugMode READ debugMode WRITE debugMode NOTIFY debugModeChanged)
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit App(QObject *parent = nullptr);

  Editor *editor() const;
  LevelHandler *levelHandler() const;

  bool debugMode() const;
  void debugMode(bool flag);

signals:
  void debugModeChanged();

private:
  void initialize();

  LevelHandler *m_levelHandler = nullptr;
  Editor *m_editor = nullptr;
  bool m_debugMode = false;
};
