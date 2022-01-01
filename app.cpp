#include "app.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(app, "app")

App::App(QObject *parent)
  : QObject { parent }
{
  initialize();
}

Editor *App::editor() const
{
  return m_editor;
}

LevelHandler *App::levelHandler() const
{
  return m_levelHandler;
}

bool App::debugMode() const
{
  return m_debugMode;
}

void App::debugMode(bool flag)
{
  if (m_debugMode == flag) {
    return;
  }

  m_debugMode = flag;
  emit debugModeChanged();
}

void App::initialize()
{
  qCInfo(app) << "initializing";

  m_levelHandler = new LevelHandler(this);
  m_editor = new Editor(this);

  connect(m_editor, &Editor::levelSavedSuccessfully, m_levelHandler, &LevelHandler::updateUserLevelsModel);
}
