#include "physicsobjectoptimizer.h"
#include "concavechopper.h"

#include <QLoggingCategory>
#include <QTransform>

Q_LOGGING_CATEGORY(phoo, "app.physicsobjectoptimizer")

namespace {
  const int DISTANCE = 3;
  const int SKIP = 2;
  const int POINTDISTANCE = 20;
  // all values that enter the physicsengine will be divided by the scalingfactor
  const float SCALINGFACTOR = 10.0f;
  const float lineThickness = 0.15f;
}

PhysicsObjectOptimizer::PhysicsObjectOptimizer(QObject *parent)
  : QObject { parent }
{
}

OptimizerResult PhysicsObjectOptimizer::optimizePoints(const QList<QPointF> &points)
{
  m_resultLineOrPolygonList.clear();
  m_OptimizedPointList.clear();

  optimizeLine(points);

  OptimizerResult result;
  result.originalPoints = points;

  if (isPolygon()) {
    ConcaveChopper concaveChopper(&m_OptimizedPointList);
    QList<QPointF> tempList = *(concaveChopper.checkAllIntersections());
    m_OptimizedPointList = tempList;

    if (!concaveChopper.isSimplePolygon()) {
      result.isLine = true;
      result.selfIntersecting = true;
      createLine();
    } else {
      createPolygon();
    }
  } else {
    result.isLine = true;
    createLine();
  }

  result.optimizedPoints = m_resultLineOrPolygonList;

  return result;
}

void PhysicsObjectOptimizer::optimizeLine(const QList<QPointF> &points)
{
  qCDebug(phoo) << "optimizing line with points" << points.count();

  // if vector is shorter than skip distance
  if (points.count() < SKIP) {
    qCDebug(phoo) << "optimizeLine: Vector is shorter than skip distance, abort." << points.count();
    m_OptimizedPointList = points;
    return;
  }

  int outerIndex = 0;
  int innerIndex = 0;

  for (outerIndex = 0; outerIndex < points.count() - SKIP; outerIndex += SKIP) {
    // take two points for a line
    QPointF lineStart = points.at(outerIndex);
    QPointF lineEnd = points.at(outerIndex + SKIP);

    qCDebug(phoo) << "optimizeLine: Comparing point " << outerIndex << "(" << lineStart << ") and " << (outerIndex + SKIP) << "("
                  << lineEnd << ")";

    // the first point is in the m_OptimizedPolygon set anyway
    qCDebug(phoo) << "optimizeLine: Storing point in optimized result" << lineStart;
    m_OptimizedPointList << lineStart;

    // check all points inbetween if they are within a defined range from the line
    for (innerIndex = outerIndex + 1; innerIndex < outerIndex + SKIP; innerIndex++) {
      QPointF point = points.at(innerIndex);

      if (!minDistance(lineStart, point)) {
        continue;
      }

      if (calculateDistance(lineStart, lineEnd, point) > DISTANCE) {
        qCDebug(phoo) << "calculateDistance: Distance acceptable (threshold: " << DISTANCE << ")";
        break;
      }

      qCDebug(phoo) << "calculateDistance: Distance too close (threshold: " << DISTANCE << ")";
    }

    outerIndex = innerIndex;
  }

  QPointF lastPoint = points.at(points.size() - 1);

  // assure that the last point or at least a point close to it is taken, too
  if (m_OptimizedPointList.size() >= 1 && minDistance(m_OptimizedPointList.last(), lastPoint)) {
    qCDebug(phoo) << "optimizeLine: Adding final point " << lastPoint << " to optimized list";
    m_OptimizedPointList << lastPoint;
  }

  // a polygon is closed when the first and last points are equal - so we check the distance and make them equal if needed to
  // force a closed polygon
  if (!minDistance(m_OptimizedPointList.first(), m_OptimizedPointList.last())) {
    qCDebug(phoo) << "start and end points too close together, forcing closed polygon";
    m_OptimizedPointList.last() = m_OptimizedPointList.first();
  }

  qCDebug(phoo) << "optimized polygon has" << m_OptimizedPointList.count() << "vertices";

  for (int o = 0; o < m_OptimizedPointList.count(); o++) {
    qCDebug(phoo) << "optimized point list at " << o << ": " << m_OptimizedPointList.at(o);
  }
}

float PhysicsObjectOptimizer::calculateDistance(const QPointF &lineStart, const QPointF &lineEnd, const QPointF &point)
{
  float result = (qAbs((lineEnd.x() - lineStart.x()) * (lineStart.y() - point.y())
                       - (lineStart.x() - point.x()) * (lineEnd.y() - lineStart.y()))
                  / qSqrt((lineEnd.x() - lineStart.x()) * (lineEnd.x() - lineStart.x())
                          + (lineEnd.y() - lineStart.y()) * (lineEnd.y() - lineStart.y())));

  qCDebug(phoo) << "calculateDistance: Point " << point << "distance to line between " << lineStart << " and " << lineEnd
                << " is " << result;

  return result;
}

bool PhysicsObjectOptimizer::minDistance(const QPointF &pointOne, const QPointF &pointTwo)
{
  int xDistance = qAbs(pointOne.x() - pointTwo.x());
  int yDistance = qAbs(pointOne.y() - pointTwo.y());

  if (xDistance < POINTDISTANCE && yDistance < POINTDISTANCE) {
    qCDebug(phoo) << "minDistance: Point " << pointOne << "and " << pointTwo << " too close to each other with distance ("
                  << xDistance << ", " << yDistance << ") (threshold: " << POINTDISTANCE << "))";
    return false;
  }

  qCDebug(phoo) << "minDistance: Point " << pointOne << "and " << pointTwo << " distant enough with (" << xDistance << ", "
                << yDistance << ") (threshold: " << POINTDISTANCE << "))";
  return true;
}

bool PhysicsObjectOptimizer::isPolygon()
{
  return ((m_OptimizedPointList.first() == m_OptimizedPointList.last()) && m_OptimizedPointList.size() > 2);
}

void PhysicsObjectOptimizer::createLine()
{
  for (int i = 0; i < m_OptimizedPointList.count() - 1; i++) {
    QPolygonF lineShape;

    QPointF firstPoint(m_OptimizedPointList.at(i + 0).x() / SCALINGFACTOR, m_OptimizedPointList.at(i + 0).y() / SCALINGFACTOR);
    QPointF secondPoint(m_OptimizedPointList.at(i + 1).x() / SCALINGFACTOR, m_OptimizedPointList.at(i + 1).y() / SCALINGFACTOR);

    float dx = secondPoint.x() - firstPoint.x();
    float dy = secondPoint.y() - firstPoint.y();

    // counterclockwise
    // bottom left
    lineShape << QPointF(firstPoint.x(), firstPoint.y() - (lineThickness / 2.f));
    // bottom right
    lineShape << QPointF(secondPoint.x(), secondPoint.y() - (lineThickness / 2.f));
    // top right
    lineShape << QPointF(secondPoint.x() + (lineThickness / 2.f), secondPoint.y());
    // top left
    lineShape << QPointF(firstPoint.x() + (lineThickness / 2.f), firstPoint.y());

    // winkel der steigung berechnen
    float m = dy / dx;
    float arctan = atan(m);
    float angle = (arctan * 180.f) / M_PI;

    // rotate box (linesegment)
    QTransform matrix;
    matrix.rotate(angle);
    lineShape = matrix.map(lineShape);

    // float lineSegmentLength = sqrt( dx * dx + dy * dy);
    // float lineSegmentLengthHalf = lineSegmentLength / 2;

    // QPointF lastPtoNextP( dx / 2, dy / 2 );
    // QPointF verticesAti( m_OptimizedPointList.at( i ).x() / SCALINGFACTOR, m_OptimizedPointList.at( i ).y() / SCALINGFACTOR );
    // lastPtoNextP = lastPtoNextP + verticesAti;

    // lineShape.SetAsBox( lineSegmentLengthHalf, lineThickness, lastPtoNextP, arctan );

    QPointF normals[4];
    QPointF centroid(0, 0);
    QPointF d;

    // Area weighted centroid
    centroid = (1.0f / 4.0f) * (lineShape.at(0) + lineShape.at(1) + lineShape.at(2) + lineShape.at(3));

    for (int i = 0; i < 4; ++i) {
      int i1 = i;
      int i2 = i + 1 < 4 ? i + 1 : 0;
      QPointF edge = lineShape.at(i2) - lineShape.at(i1);
      // b2Assert(edge.LengthSquared() > B2_FLT_EPSILON * B2_FLT_EPSILON);
      normals[i] = qCross(edge, 1.0f);
      normalize(normals[i]);
    }

    for (int i = 0; i < 4; ++i) {
      int ii1 = i - 1 >= 0 ? i - 1 : 3;
      int ii2 = i;

      QPointF n1 = normals[ii1];
      QPointF n2 = normals[ii2];
      QPointF v = lineShape.at(i) - centroid;

      d.setX(QPointF::dotProduct(n1, v)); // - b2_toiSlop;
      d.setY(QPointF::dotProduct(n2, v)); // - b2_toiSlop;
    }

    if ((d.x() >= 0.1) && (d.y() >= 0.1)) {
      m_resultLineOrPolygonList << lineShape;
    }
  }
}

void PhysicsObjectOptimizer::createPolygon()
{
  for (int i = 0; i < (int)m_OptimizedPointList.size() - 2; i += 3) {
    QPolygonF polygonShape;

    polygonShape << QPointF(m_OptimizedPointList.at(i + 0).x() / SCALINGFACTOR,
                            m_OptimizedPointList.at(i + 0).y() / SCALINGFACTOR);
    polygonShape << QPointF(m_OptimizedPointList.at(i + 1).x() / SCALINGFACTOR,
                            m_OptimizedPointList.at(i + 1).y() / SCALINGFACTOR);
    polygonShape << QPointF(m_OptimizedPointList.at(i + 2).x() / SCALINGFACTOR,
                            m_OptimizedPointList.at(i + 2).y() / SCALINGFACTOR);

    QPointF normals[3];
    QPointF centroid(0, 0);
    QPointF d;

    // Area weighted centroid
    centroid = (1.0f / 3.0f) * (polygonShape.at(0) + polygonShape.at(1) + polygonShape.at(2));

    for (int i = 0; i < 3; ++i) {
      int i1 = i;
      int i2 = i + 1 < 3 ? i + 1 : 0;
      QPointF edge = polygonShape.at(i2) - polygonShape.at(i1);
      // b2Assert( edge.LengthSquared() > FLT_EPSILON * FLT_EPSILON );
      normals[i] = qCross(edge, 1.0f);
      normalize(normals[i]);
    }

    // Compute normals. Ensure the edges have non-zero length.
    for (int i = 0; i < 3; ++i) {
      int ii1 = i - 1 >= 0 ? i - 1 : 2;
      int ii2 = i;

      QPointF n1 = normals[ii1];
      QPointF n2 = normals[ii2];
      QPointF v = polygonShape.at(i) - centroid;

      d.setX(QPointF::dotProduct(n1, v)); // - b2_toiSlop;
      d.setY(QPointF::dotProduct(n2, v)); // - b2_toiSlop;
    }

    if ((d.x() >= 0.03f) && (d.y() >= 0.03f)) {
      m_resultLineOrPolygonList << polygonShape;
    }
  }
}
