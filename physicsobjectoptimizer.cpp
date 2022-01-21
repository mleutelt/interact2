#include "physicsobjectoptimizer.h"
#include "concavechopper.h"

#include <QLoggingCategory>
#include <QTransform>

Q_LOGGING_CATEGORY(phoo, "app.physicsobjectoptimizer")

namespace {
  const int POINT_TO_LINE_DISTANCE = 2;
  const int SKIP = 2;
  const int LINE_SEGMENT_MIN_POINT_DISTANCE = 2; // for line segments
  const int START_END_POINT_DISTANCE = 20; // distance to decide wether the line is closed or not
  // TODO: should be specified by the UI
  const float LINE_THICKNESS = 3.f;
}

PhysicsObjectOptimizer::PhysicsObjectOptimizer(QObject *parent)
  : QObject { parent }
{
}

OptimizerResult PhysicsObjectOptimizer::determineAndOptimizeObject(const QList<QPointF> &points)
{
  m_resultLineOrPolygonList.clear();
  m_OptimizedPointList.clear();

  // NOTE: make sure all points are in local coordinates
  QPolygonF tmp(points);
  tmp.translate(-tmp.boundingRect().x(), -tmp.boundingRect().y());

  optimizeLine(tmp);

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

      if (!minDistance(lineStart, point, LINE_SEGMENT_MIN_POINT_DISTANCE)) {
        continue;
      }

      if (calculateDistance(lineStart, lineEnd, point) > POINT_TO_LINE_DISTANCE) {
        qCDebug(phoo) << "calculateDistance: Distance acceptable (threshold: " << POINT_TO_LINE_DISTANCE << ")";
        break;
      }

      qCDebug(phoo) << "calculateDistance: Distance too close (threshold: " << POINT_TO_LINE_DISTANCE << ")";
    }

    outerIndex = innerIndex;
  }

  QPointF lastPoint = points.at(points.size() - 1);

  // assure that the last point or at least a point close to it is taken, too
  if (m_OptimizedPointList.size() >= 1 && minDistance(m_OptimizedPointList.last(), lastPoint, LINE_SEGMENT_MIN_POINT_DISTANCE)) {
    qCDebug(phoo) << "optimizeLine: Adding final point " << lastPoint << " to optimized list";
    m_OptimizedPointList << lastPoint;
  }

  // a polygon is closed when the first and last points are equal - so we check the distance and make them equal if needed to
  // force a closed polygon
  if (!minDistance(m_OptimizedPointList.first(), m_OptimizedPointList.last(), START_END_POINT_DISTANCE)) {
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

bool PhysicsObjectOptimizer::minDistance(const QPointF &pointOne, const QPointF &pointTwo, int iDistance)
{
  int xDistance = qAbs(pointOne.x() - pointTwo.x());
  int yDistance = qAbs(pointOne.y() - pointTwo.y());

  if (xDistance < iDistance && yDistance < iDistance) {
    qCDebug(phoo) << "minDistance: Point " << pointOne << "and " << pointTwo << " too close to each other with distance ("
                  << xDistance << ", " << yDistance << ") (threshold: " << iDistance << "))";
    return false;
  }

  qCDebug(phoo) << "minDistance: Point " << pointOne << "and " << pointTwo << " distant enough with (" << xDistance << ", "
                << yDistance << ") (threshold: " << iDistance << "))";
  return true;
}

bool PhysicsObjectOptimizer::isPolygon()
{
  if (m_OptimizedPointList.size() > 2) {
    return m_OptimizedPointList.first() == m_OptimizedPointList.last();
  }

  return false;
}

void PhysicsObjectOptimizer::createLine()
{
  if (m_OptimizedPointList.count() < 2) {
    m_resultLineOrPolygonList << m_OptimizedPointList;
    return;
  }

  for (int i = 0; i < m_OptimizedPointList.count() - 1; i++) {

    QPointF firstPoint(m_OptimizedPointList.at(i + 0).x(), m_OptimizedPointList.at(i + 0).y());
    QPointF secondPoint(m_OptimizedPointList.at(i + 1).x(), m_OptimizedPointList.at(i + 1).y());

    // middle point of line and center of line segment rectangle
    QPointF midPoint((firstPoint.x() + secondPoint.x()) / 2.f, (firstPoint.y() + secondPoint.y()) / 2.f);

    float dx = secondPoint.x() - firstPoint.x();
    float dy = secondPoint.y() - firstPoint.y();

    // calculate angle between points
    float m = dy / dx;
    float arctan = atan(m); // Radians
    // float angle = (arctan * 180.f) / M_PI; // Deg

    // rotate both points so they are in a horizontal line through midPoint
    QPointF firstPointTmpRot = rotateVertex(firstPoint, midPoint, -arctan);
    QPointF secondPointTmpRot = rotateVertex(secondPoint, midPoint, -arctan);

    // span rectangle with desired lineThickness (counterclockwise)
    QPointF tmpBoxBottomLeft = QPointF(firstPointTmpRot.x(), firstPointTmpRot.y() - (LINE_THICKNESS / 2.f));
    QPointF tmpBoxBottomRight = QPointF(secondPointTmpRot.x(), secondPointTmpRot.y() - (LINE_THICKNESS / 2.f));
    QPointF tmpBoxTopRight = QPointF(secondPointTmpRot.x(), secondPointTmpRot.y() + (LINE_THICKNESS / 2.f));
    QPointF tmpBoxTopLeft = QPointF(firstPointTmpRot.x(), firstPointTmpRot.y() + (LINE_THICKNESS / 2.f));

    // qCDebug(phoo) << "createLine: Current line shape before transformation: " << tmpBoxBottomLeft << tmpBoxBottomRight <<
    // tmpBoxTopRight << tmpBoxTopLeft;

    QPolygonF lineShape;

    // rotate box back at destination around midPoint
    lineShape << rotateVertex(tmpBoxBottomLeft, midPoint, arctan);
    lineShape << rotateVertex(tmpBoxBottomRight, midPoint, arctan);
    lineShape << rotateVertex(tmpBoxTopRight, midPoint, arctan);
    lineShape << rotateVertex(tmpBoxTopLeft, midPoint, arctan);

    // qCDebug(phoo) << "createLine: Current line shape after transformation: " << lineShape.at(0) << lineShape.at(1) <<
    // lineShape.at(2) << lineShape.at(3);

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

    if ((fabs(d.x()) >= 0.01) && (fabs(d.y()) >= 0.01)) {
      // qCDebug(phoo) << "createLine: Adding line segment " << lineShape.at(0) << lineShape.at(1) << lineShape.at(2) <<
      // lineShape.at(3);
      m_resultLineOrPolygonList << lineShape;
    }
  }
}

void PhysicsObjectOptimizer::createPolygon()
{
  for (int i = 0; i < (int)m_OptimizedPointList.size() - 2; i += 3) {
    QPolygonF polygonShape;

    polygonShape << QPointF(m_OptimizedPointList.at(i + 0).x(), m_OptimizedPointList.at(i + 0).y());
    polygonShape << QPointF(m_OptimizedPointList.at(i + 1).x(), m_OptimizedPointList.at(i + 1).y());
    polygonShape << QPointF(m_OptimizedPointList.at(i + 2).x(), m_OptimizedPointList.at(i + 2).y());

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
