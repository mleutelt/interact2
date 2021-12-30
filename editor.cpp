#include "editor.h"
#include "levelhandler.h"

#include <QDir>
#include <QStandardPaths>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(editor, "app.editor")

Editor::Editor(QObject *parent) : QObject { parent }
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

  screenshot->saveToFile(QDir(LevelHandler::userLevelsDirectory().filePath(name)).filePath(LevelHandler::levelPreviewFileName()));

  emit levelSavedSuccessfully();
}
