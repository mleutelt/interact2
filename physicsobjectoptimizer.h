#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QPointF>
#include <QPolygonF>

#include <float.h>

struct OptimizerResult
{
  Q_GADGET
  Q_PROPERTY(QList<QVariantList> optimizedPoints MEMBER optimizedPoints)
  Q_PROPERTY(bool isLine MEMBER isLine)
  Q_PROPERTY(QPolygonF originalPoints MEMBER originalPoints)

public:
  QList<QVariantList> optimizedPoints;
  bool isLine = false;
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
  /// Perform the cross product on a vector and a scalar. In 2D this produces a vector.
  QPointF qCross(const QPointF &a, float s);

  /// Convert this vector into a unit vector. Returns the length.
  float normalize(QPointF &a);

  QPointF rotateVertex(const QPointF &inputPoint, const QPointF &centerPoint, float fAngleRadians);

  float floatRand(float fMin, float fMax);

  static QList<QVariantList> polygonListToVariantList(const QList<QPolygonF> &list);

  QList<QPointF> naturalizeLine(const QList<QPointF> &input);
  QList<QPointF> optimizeLine(const QList<QPointF> &input);

  // calculate the distance of a point to a line
  float calculateDistance(const QPointF &lineStart, const QPointF &lineEnd, const QPointF &point);
  // calculate the distance between two points
  bool minDistance(const QPointF &pointOne, const QPointF &pointTwo, int iDistance);

  bool isPolygon(const QList<QPointF> &points);
  QList<QPolygonF> createLine(const QList<QPointF> &input);
  QList<QPolygonF> createPolygon(const QList<QPointF> &input);

  bool calculateLineIntersection(const QPointF &one, const QPointF &two, const QPointF &three, const QPointF &four,
                                 QPointF &intersection);
  bool lineSelfIntersecting(const QList<QPointF> &points);
};
