#include "editor.h"
#include "levelhandler.h"
#include "levelfileio.h"

#include <QDir>
#include <QStandardPaths>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(editor, "app.editor")

Editor::Editor(QObject *parent)
  : QObject { parent }
  , m_levelData { new LevelDataModel(this) }
{
}

Editor::ShapeType Editor::currentShape() const
{
  return m_currentShape;
}

void Editor::setCurrentShape(ShapeType shape)
{
  if (m_currentShape == shape) {
    return;
  }

  m_currentShape = shape;
  emit currentShapeChanged();
}

QList<Editor::ShapeType> Editor::availableShapes() const
{
  return {
    ShapeType_Circle,
    ShapeType_Rectangle,
    ShapeType_Polygon,
    ShapeType_SpecialStar,
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
  levelData.objects = m_levelData->objects();

  screenshot->saveToFile(levelDirectory.filePath(LevelHandler::levelPreviewFileName()));
  LevelFileIO::storeLevelAtPath(levelDirectory.filePath(LevelHandler::levelDataFileName()), levelData);

  emit levelSavedSuccessfully();
}

void Editor::reset()
{
  m_levelData->clear();
}

void Editor::addObject(int type, const QRect &boundingRect, bool isStatic, int rotation) const
{
  m_levelData->addObject(type, boundingRect, isStatic, rotation);
}

void Editor::removeObject(int index) const
{
  m_levelData->removeObject(index);
}

LevelDataModel *Editor::levelData() const
{
  return m_levelData;
}
