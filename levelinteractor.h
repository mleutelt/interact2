#pragma once

#include <QObject>
#include <QtQml/qqml.h>

#include "ilevelmanager.h"
#include "physicsobjectoptimizer.h"

class LevelInteractor : public QObject
{
  Q_OBJECT
  Q_PROPERTY(ILevelManager *levelManager READ levelManager WRITE setLevelManager NOTIFY levelManagerChanged)
  QML_ELEMENT

public:
  explicit LevelInteractor(QObject *parent = nullptr);

  ILevelManager *levelManager() const;

  Q_INVOKABLE void addSimpleObject(int type, const QRectF &boundingRect, bool isStatic = false, bool invisible = false,
                                   bool gameItem = false) const;
  Q_INVOKABLE void addPolygonObject(int type, const OptimizerResult &optimizerResult, bool isStatic = false,
                                    bool invisible = false, bool gameItem = false) const;
  Q_INVOKABLE void removeObject(int index) const;

signals:
  void levelManagerChanged();

private:
  void setLevelManager(ILevelManager *manager);

  ILevelManager *m_levelManager = nullptr;
};
