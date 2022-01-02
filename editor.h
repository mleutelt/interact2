#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QQuickItemGrabResult>

#include "leveldatamodel.h"

class Editor : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QList<ShapeType> availableShapes READ availableShapes CONSTANT)
  Q_PROPERTY(QList<EditOperationType> availableEditOperations READ availableEditOperations CONSTANT)
  Q_PROPERTY(ShapeType currentShape READ currentShape WRITE setCurrentShape NOTIFY currentShapeChanged)
  Q_PROPERTY(EditOperationType currentEditOperation READ currentEditOperation WRITE setCurrentEditOperation NOTIFY
                 currentEditOperationChanged)
  Q_PROPERTY(LevelDataModel *levelData READ levelData CONSTANT)
  QML_ELEMENT

public:
  enum ShapeType
  {
    ShapeType_Circle,
    ShapeType_Rectangle,
    ShapeType_Polygon,
  };
  Q_ENUM(ShapeType)

  enum EditOperationType
  {
    EditOperationType_Draw,
    EditOperationType_Move,
    EditOperationType_Delete,
  };
  Q_ENUM(EditOperationType)

  explicit Editor(QObject *parent = nullptr);

  ShapeType currentShape() const;
  void setCurrentShape(ShapeType shape);

  QList<ShapeType> availableShapes() const;
  QList<EditOperationType> availableEditOperations() const;

  EditOperationType currentEditOperation() const;
  void setCurrentEditOperation(EditOperationType operation);

  LevelDataModel *levelData() const;

  Q_INVOKABLE void saveLevel(const QString &name, QQuickItemGrabResult *screenshot);
  Q_INVOKABLE void reset();

  Q_INVOKABLE void addObject(int type, const QRect &boundingRect, bool isStatic = false, int rotation = 0) const;
  Q_INVOKABLE void removeObject(int index) const;

signals:
  void currentShapeChanged();
  void currentEditOperationChanged();
  void levelSavedSuccessfully();

private:
  ShapeType m_currentShape = ShapeType_Circle;
  EditOperationType m_currentEditOperation = EditOperationType_Draw;
  LevelDataModel *m_levelData = nullptr;
};
