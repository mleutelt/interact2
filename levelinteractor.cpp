#include "levelinteractor.h"
#include "constants.h"
#include "leveldatamodel.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvlia, "app.levelinteractor")

LevelInteractor::LevelInteractor(QObject *parent)
  : QObject { parent }
{
}

ILevelManager *LevelInteractor::levelManager() const
{
  return m_levelManager;
}

void LevelInteractor::setLevelManager(ILevelManager *manager)
{
  if (m_levelManager == manager) {
    return;
  }

  m_levelManager = manager;
  emit levelManagerChanged();
}

void LevelInteractor::addSimpleObject(int type, const QRectF &boundingRect, bool isStatic) const
{
  m_levelManager->levelData()->addSimpleObject(type, boundingRect, isStatic);
}

void LevelInteractor::addPolygonObject(int type, const OptimizerResult &optimizerResult, bool isStatic) const
{
  if (optimizerResult.originalPoints.count() > 2) {
    if (optimizerResult.isLine) {
      m_levelManager->levelData()->addLineObject(Constants::ShapeType_Line, optimizerResult.originalPoints,
                                                 optimizerResult.optimizedPoints, isStatic);
    } else {
      m_levelManager->levelData()->addPolygonObject(Constants::ShapeType_Polygon, optimizerResult.originalPoints,
                                                    optimizerResult.optimizedPoints, isStatic);
    }
  } else {
    qCDebug(lvlia) << "ignoring input, not enough points";
  }
}

void LevelInteractor::removeObject(int index) const
{
  m_levelManager->levelData()->removeObject(index);
}
