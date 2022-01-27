#pragma once

#include <QObject>
#include <QtQml/qqml.h>

#include "leveldatamodel.h"
#include "physicsobjectoptimizer.h"
#include "constants.h"
#include "ilevelmanager.h"

class QQuickItemGrabResult;

class Editor : public ILevelManager
{
  Q_OBJECT
  Q_PROPERTY(QList<Constants::ShapeType> availableShapes READ availableShapes CONSTANT)
  Q_PROPERTY(QList<EditOperationType> availableEditOperations READ availableEditOperations CONSTANT)
  Q_PROPERTY(Constants::ShapeType currentShape READ currentShape WRITE setCurrentShape NOTIFY currentShapeChanged)
  Q_PROPERTY(EditOperationType currentEditOperation READ currentEditOperation WRITE setCurrentEditOperation NOTIFY
                 currentEditOperationChanged)
  Q_PROPERTY(LevelDataModel *levelData READ levelData CONSTANT)
  QML_ELEMENT
  QML_IMPLEMENTS_INTERFACES(ILevelManager)

public:
  enum EditOperationType
  {
    EditOperationType_Draw,
    EditOperationType_Move,
    EditOperationType_Delete,
  };
  Q_ENUM(EditOperationType)

  explicit Editor(QObject *parent = nullptr);

  Constants::ShapeType currentShape() const;
  void setCurrentShape(Constants::ShapeType shape);

  QList<Constants::ShapeType> availableShapes() const;
  QList<EditOperationType> availableEditOperations() const;

  EditOperationType currentEditOperation() const;
  void setCurrentEditOperation(EditOperationType operation);

protected:
  void loadLevel(const QString &path) override;
  void saveLevel(const QString &name, QQuickItemGrabResult *screenshot) override;
  void resetLevel() override;

  LevelDataModel *levelData() const override;

signals:
  void currentShapeChanged();
  void currentEditOperationChanged();

private:
  Constants::ShapeType m_currentShape = Constants::ShapeType_Circle;
  EditOperationType m_currentEditOperation = EditOperationType_Draw;
  LevelDataModel *m_levelData = nullptr;
};
