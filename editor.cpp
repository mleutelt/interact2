#include "editor.h"
#include "levelhandler.h"
#include "levelfileio.h"

#include <QDir>
#include <QStandardPaths>
#include <QQuickItemGrabResult>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(editor, "app.editor")

Editor::Editor(QObject *parent)
  : ILevelManager { parent }
  , m_levelData { new LevelDataModel(this) }
{
  connect(this, &Editor::currentShapeChanged, this, [this]() {
    setCurrentEditOperation(EditOperationType_Draw);
  });
}

Constants::ShapeType Editor::currentShape() const
{
  return m_currentShape;
}

void Editor::setCurrentShape(Constants::ShapeType shape)
{
  if (m_currentShape == shape) {
    return;
  }

  m_currentShape = shape;
  emit currentShapeChanged();
}

QList<Constants::ShapeType> Editor::availableShapes() const
{
  return {
    Constants::ShapeType_Circle,
    Constants::ShapeType_Rectangle,
    Constants::ShapeType_Polygon,
    Constants::ShapeType_SpecialStar,
  };
}

QList<Editor::EditOperationType> Editor::availableEditOperations() const
{
  return {
    EditOperationType_Draw,
    EditOperationType_Move,
    EditOperationType_Delete,
  };
}

Editor::EditOperationType Editor::currentEditOperation() const
{
  return m_currentEditOperation;
}

void Editor::setCurrentEditOperation(EditOperationType operation)
{
  if (m_currentEditOperation == operation) {
    return;
  }

  m_currentEditOperation = operation;
  emit currentEditOperationChanged();
}

void Editor::saveLevel(const QString &name, QQuickItemGrabResult *screenshot)
{
  qCDebug(editor) << "saving level with name:" << name;

  LevelHandler::userLevelsDirectory().mkdir(name);

  QDir levelDirectory(LevelHandler::userLevelsDirectory().filePath(name));
  LevelData levelData;
  levelData.name = name;
  levelData.backgroundImage = m_levelData->backgroundImage();
  levelData.music = m_levelData->music();
  levelData.objects = m_levelData->objects();

  screenshot->saveToFile(levelDirectory.filePath(LevelHandler::levelPreviewFileName()));
  LevelFileIO::storeLevel(levelDirectory.filePath(LevelHandler::levelDataFileName()), levelData);

  emit levelSavedSuccessfully();
}

void Editor::resetLevel()
{
  m_levelData->clear();
  setCurrentShape(Constants::ShapeType_Circle);
  setCurrentEditOperation(EditOperationType_Draw);
}

LevelDataModel *Editor::levelData() const
{
  return m_levelData;
}

void Editor::loadLevel(const QString &path)
{
  qCDebug(editor) << "loading level at path" << path;

  QDir levelPath(path);

  if (levelPath.exists(LevelHandler::levelDataFileName())) {
    m_levelData->setLevelData(LevelFileIO::loadLevel(levelPath.filePath(LevelHandler::levelDataFileName())));

    emit levelLoadedSuccessfully();
  } else {
    qCCritical(editor) << "unable to find level data";
    // TODO: emit signal to display UI message
  }
}
