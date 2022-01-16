#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QQuickItemGrabResult>

#include "leveldatamodel.h"
#include "physicsobjectoptimizer.h"
#include "constants.h"

class Editor : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QList<Constants::ShapeType> availableShapes READ availableShapes CONSTANT)
  Q_PROPERTY(QList<EditOperationType> availableEditOperations READ availableEditOperations CONSTANT)
  Q_PROPERTY(Constants::ShapeType currentShape READ currentShape WRITE setCurrentShape NOTIFY currentShapeChanged)
  Q_PROPERTY(EditOperationType currentEditOperation READ currentEditOperation WRITE setCurrentEditOperation NOTIFY
                 currentEditOperationChanged)
  Q_PROPERTY(LevelDataModel *levelData READ levelData CONSTANT)
  QML_ELEMENT

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

  LevelDataModel *levelData() const;

  Q_INVOKABLE void loadLevel(const QString &path);
  Q_INVOKABLE void saveLevel(const QString &name, QQuickItemGrabResult *screenshot);
  Q_INVOKABLE void reset();

  Q_INVOKABLE void addSimpleObject(int type, const QRectF &boundingRect, bool isStatic = false, int rotation = 0) const;
  Q_INVOKABLE void addPolygonObject(int type, const OptimizerResult &optimizerResult, bool isStatic = false,
                                    int rotation = 0) const;
  Q_INVOKABLE void removeObject(int index) const;

signals:
  void currentShapeChanged();
  void currentEditOperationChanged();
  void levelSavedSuccessfully();
  void levelLoadedSuccessfully();

private:
  Constants::ShapeType m_currentShape = Constants::ShapeType_Circle;
  EditOperationType m_currentEditOperation = EditOperationType_Draw;
  LevelDataModel *m_levelData = nullptr;
};
