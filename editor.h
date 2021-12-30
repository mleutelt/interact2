#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QQuickItemGrabResult>

class Editor : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QList<ShapeType> availableShapes READ availableShapes CONSTANT)
  Q_PROPERTY(QList<EditOperationType> availableEditOperations READ availableEditOperations CONSTANT)
  Q_PROPERTY(ShapeType currentShape READ currentShape WRITE setCurrentShape NOTIFY currentShapeChanged)
  Q_PROPERTY(EditOperationType currentEditOperation READ currentEditOperation WRITE setCurrentEditOperation NOTIFY
                 currentEditOperationChanged)
  QML_ELEMENT

public:
  enum ShapeType
  {
    ShapeType_Circle,
    ShapeType_Rectangle,
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

  Q_INVOKABLE void saveLevel(const QString &name, QQuickItemGrabResult *screenshot);

signals:
  void currentShapeChanged();
  void currentEditOperationChanged();
  void levelSavedSuccessfully();

private:
  ShapeType m_currentShape = ShapeType_Circle;
  EditOperationType m_currentEditOperation = EditOperationType_Draw;
};
