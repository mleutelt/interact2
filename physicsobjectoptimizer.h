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

  inline QPointF rotateVertex(const QPointF &inputPoint, const QPointF &centerPoint, float fAngleRadians)
  {
      QPointF rotatedPoint
      ((inputPoint.x() - centerPoint.x()) * cos(fAngleRadians) - (inputPoint.y() - centerPoint.y()) * sin(fAngleRadians) + centerPoint.x(),
      (inputPoint.x() - centerPoint.x()) * sin(fAngleRadians) + (inputPoint.y() - centerPoint.y()) * cos(fAngleRadians) + centerPoint.y());

      return rotatedPoint;
  }

  inline float floatRand(float fMin, float fMax)
  {
      float fScale = rand() / (float) RAND_MAX; /* range between [0, 1.f] */
      return fMin + fScale * ( fMax - fMin );      /* range between [fMin, fMax] */
  }

  QList<QPointF> naturalizeLine(const QList<QPointF> &input);
  void optimizeLine(const QList<QPointF> &input);

  // calculate the distance of a point to a line
  float calculateDistance(const QPointF &lineStart, const QPointF &lineEnd, const QPointF &point);
  // calculate the distance between two points
  bool minDistance(const QPointF &pointOne, const QPointF &pointTwo, int iDistance);

  bool isPolygon();
  void createLine();
  void createPolygon();

  QList<QPointF> m_OptimizedPointList;
  QList<QPolygonF> m_resultLineOrPolygonList;
};
