#include "app.h"

#include <QGuiApplication>
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

bool App::debugBoundingBoxes() const
{
  return m_debugBoundingBoxes;
}

void App::setDebugBoundingBoxes(bool flag)
{
  if (m_debugBoundingBoxes == flag) {
    return;
  }

  m_debugBoundingBoxes = flag;
  emit debugBoundingBoxesChanged();
}

void App::initialize()
{
  qCInfo(app) << "initializing";

  m_levelHandler = new LevelHandler(this);
  m_editor = new Editor(this);

  connect(m_editor, &Editor::levelSavedSuccessfully, m_levelHandler, &LevelHandler::updateUserLevelsModel);
}

bool App::debugBox2d() const
{
  return m_debugBox2d;
}

void App::setDebugBox2d(bool flag)
{
  if (m_debugBox2d == flag) {
    return;
  }

  m_debugBox2d = flag;
  emit debugBox2dChanged();
}

QString App::musicPath() const
{
#if defined(Q_OS_IOS)
  return QString("file://%1/").arg(qGuiApp->applicationDirPath());
#else
  return QString("file://%1/%2/").arg(qGuiApp->applicationDirPath()).arg("music");
#endif
}
