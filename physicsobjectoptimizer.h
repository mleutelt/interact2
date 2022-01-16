#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QPointF>
#include <QPolygonF>

#include <float.h>

struct OptimizerResult
{
  Q_GADGET
  Q_PROPERTY(QList<QPolygonF> optimizedPoints MEMBER optimizedPoints)
  Q_PROPERTY(bool isLine MEMBER isLine)
  Q_PROPERTY(bool selfIntersecting MEMBER selfIntersecting)
  Q_PROPERTY(QPolygonF originalPoints MEMBER originalPoints)

public:
  QList<QPolygonF> optimizedPoints;
  bool isLine = false;
  bool selfIntersecting = false;
  QPolygonF originalPoints;
};

class PhysicsObjectOptimizer : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit PhysicsObjectOptimizer(QObject *parent = nullptr);

  Q_INVOKABLE OptimizerResult determineAndOptimizeObject(const QList<QPointF> &points);

private:
  /// Perform the cross product on a vector and a scalar. In 2D this produces
  /// a vector.
  inline QPointF qCross(const QPointF &a, float s)
  {
    return QPointF(s * a.y(), -s * a.x());
  }

  /// Convert this vector into a unit vector. Returns the length.
  inline float normalize(QPointF &a)
  {
    float length = sqrtf(a.x() * a.x() + a.y() * a.y());
    if (length < FLT_EPSILON) {
      return 0.0f;
    }
    float invLength = 1.0f / length;
    a.setX(a.x() * invLength);
    a.setY(a.y() * invLength);

    return length;
  }

  void optimizeLine(const QList<QPointF> &input);

  // calculate the distance of a point to a line
  float calculateDistance(const QPointF &lineStart, const QPointF &lineEnd, const QPointF &point);
  // calculate the distance between two points
  bool minDistance(const QPointF &pointOne, const QPointF &pointTwo);

  bool isPolygon();
  void createLine();
  void createPolygon();

  QList<QPointF> m_OptimizedPointList;
  QList<QPolygonF> m_resultLineOrPolygonList;
};
