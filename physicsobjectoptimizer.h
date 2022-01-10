#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QPointF>
#include <QPolygonF>

class PhysicsObjectOptimizer : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit PhysicsObjectOptimizer(QObject *parent = nullptr);

  Q_INVOKABLE QPolygonF optimizeLine(const QList<QPointF> &input);

private:
  // calculate the distance of a point to a line
  float calculateDistance(const QPointF &lineStart, const QPointF &lineEnd, const QPointF &point);
  // calculate the distance between two points
  bool minDistance(const QPointF &pointOne, const QPointF &pointTwo);
};
