#include "physicsobjectoptimizer.h"
#include "concavechopper.h"

#include <QLoggingCategory>
#include <QTransform>
#include <QRandomGenerator>

Q_LOGGING_CATEGORY(phoo, "app.physicsobjectoptimizer")

namespace {
  // for optimizeLine
  const int POINT_TO_LINE_DISTANCE = 2;
  const int SKIP = 2;
  const int LINE_SEGMENT_MIN_POINT_DISTANCE = 2; // for line segments
  const int START_END_POINT_DISTANCE = 20; // distance to decide wether the line is closed or not

  // for naturalizeLine
  const int INPUT_COORDINATES_MAX_DISTANCE = 4; // max. allowed distance between consecutive mouse coordinates
  const float MIN_RANDOM_OFFSET = 1.5f; // min value for random point offset
  const float MAX_RANDOM_OFFSET = 1.5f; // max value for random point offset

  // TODO: should be specified by the UI
  const float LINE_THICKNESS = 3.f;
}

PhysicsObjectOptimizer::PhysicsObjectOptimizer(QObject *parent)
  : QObject { parent }
{
}

OptimizerResult PhysicsObjectOptimizer::determineAndOptimizeObject(const QList<QPointF> &points)
{
  OptimizerResult result;
  result.originalPoints = points;

  // NOTE: make sure all points are in local coordinates
  QPolygonF tmp(points);
  tmp.translate(-tmp.boundingRect().x(), -tmp.boundingRect().y());

  // NOTE: step 1: simplify line, optimally reducing the number of points
  QList<QPointF> optimizedLine = optimizeLine(tmp);

  // NOTE: this will contain the result, in case of a
  // - simple closed polygon: a list of triangles, based on the triangulation of the contour
  // - line (self-intersecting/non-intersecting): a list of line segments, where each segment
  // is represented by 4 vertices
  QList<QPolygonF> polygonList;

  // NOTE: step 2: check characteristics of the optimized line
  if (isPolygon(optimizedLine) && !lineSelfIntersecting(optimizedLine)) {
    // NOTE: step 2a: treat the line as a contour of a polygon and triangulate it
    polygonList = createPolygon(ConcaveChopper::triangulate(optimizedLine));
  } else {
    // NOTE: step 2b: treat the line as is and create line segments from the input
    result.isLine = true;
    polygonList = createLine(optimizedLine);
  }

  // NOTE: step 3: convert the result
  // Box2D works on QVariantList so we convert QList<QPolygonF> to QList<QVariantList> here
  result.optimizedPoints = polygonListToVariantList(polygonList);

  return result;
}

QPointF PhysicsObjectOptimizer::qCross(const QPointF &a, float s)
{
  return QPointF(s * a.y(), -s * a.x());
}

float PhysicsObjectOptimizer::normalize(QPointF &a)
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

QPointF PhysicsObjectOptimizer::rotateVertex(const QPointF &inputPoint, const QPointF &centerPoint, float fAngleRadians)
{
  QPointF rotatedPoint((inputPoint.x() - centerPoint.x()) * cos(fAngleRadians)
                           - (inputPoint.y() - centerPoint.y()) * sin(fAngleRadians) + centerPoint.x(),
                       (inputPoint.x() - centerPoint.x()) * sin(fAngleRadians)
                           + (inputPoint.y() - centerPoint.y()) * cos(fAngleRadians) + centerPoint.y());

  return rotatedPoint;
}

float PhysicsObjectOptimizer::floatRand(float fMin, float fMax)
{
  float fScale = rand() / (float)RAND_MAX; /* range between [0, 1.f] */
  return fMin + fScale * (fMax - fMin); /* range between [fMin, fMax] */
}

QList<QVariantList> PhysicsObjectOptimizer::polygonListToVariantList(const QList<QPolygonF> &list)
{
  QList<QVariantList> result;

  for (const QPolygonF &polygon : list) {
    QVariantList pointList;

    for (const QPointF &point : polygon) {
      pointList << point;
    }

    result << pointList;
  }

  return result;
}

QList<QPointF> PhysicsObjectOptimizer::naturalizeLine(const QList<QPointF> &input)
{
  // If too consecutive input points are too far away from each other,
  // the result will be a long and perfectly straightline, which looks
  // just too artificial. So we will insert some random input points
  // in between.
  QList<QPointF> output;

  if (input.size() < 2) {
    return input;
  }

  for (int i = 0; i < input.size() - 1; ++i) {
    QPointF firstPoint = input.at(i);
    QPointF secondPoint = input.at(i + 1);

    // firstPoint will be added in any case
    output << firstPoint;

    float fdiffX = secondPoint.x() - firstPoint.x();
    float fdiffY = secondPoint.y() - firstPoint.y();

    float fDist = sqrtf((fdiffX * fdiffX) + (fdiffY * fdiffY));

    int iNumSubSegments = (int)((fDist / INPUT_COORDINATES_MAX_DISTANCE) + 1);

    if (fDist > INPUT_COORDINATES_MAX_DISTANCE) {
      float fIntervalX = fdiffX / (iNumSubSegments + 1);
      float fIntervalY = fdiffY / (iNumSubSegments + 1);

      for (int j = 1; j < iNumSubSegments; j++) {
        // calculate new x and y coordinates for each point in between
        // and also add some random value to have it look less artificial
        float fNewX = firstPoint.x() + fIntervalX * j + floatRand(MIN_RANDOM_OFFSET, MAX_RANDOM_OFFSET);
        float fNewY = firstPoint.y() + fIntervalY * j + floatRand(MIN_RANDOM_OFFSET, MAX_RANDOM_OFFSET);

        output << QPointF(fNewX, fNewY);
      }
    }
  }

  // finally don't forget to take the last point also
  output << input.last();

  return output;
}

QList<QPointF> PhysicsObjectOptimizer::optimizeLine(const QList<QPointF> &points)
{
  qCDebug(phoo) << "Number of raw input points before optimzing: " << points.count();

  QList<QPointF> result;

  // if vector is shorter than skip distance
  if (points.count() < SKIP) {
    qCDebug(phoo) << "optimizeLine: Vector is shorter than skip distance, abort." << points.count();

    return points;
  }

  // get a non-const copy
  QList<QPointF> naturalizedPoints = naturalizeLine(points);

  qCDebug(phoo) << "Number of refined input points before optimzing: " << naturalizedPoints.count();

  int outerIndex = 0;
  int innerIndex = 0;

  for (outerIndex = 0; outerIndex < naturalizedPoints.count() - SKIP; outerIndex += SKIP) {
    // take two points for a line
    QPointF lineStart = naturalizedPoints.at(outerIndex);
    QPointF lineEnd = naturalizedPoints.at(outerIndex + SKIP);

    // qCDebug(phoo) << "optimizeLine: Comparing point " << outerIndex << "(" << lineStart << ") and " << (outerIndex + SKIP) <<
    // "(" << lineEnd << ")";

    // the first point is in the m_OptimizedPolygon set anyway
    // qCDebug(phoo) << "optimizeLine: Storing point in optimized result" << lineStart;
    result << lineStart;

    // check all points inbetween if they are within a defined range from the line
    for (innerIndex = outerIndex + 1; innerIndex < outerIndex + SKIP; innerIndex++) {
      QPointF point = naturalizedPoints.at(innerIndex);

      if (!minDistance(lineStart, point, LINE_SEGMENT_MIN_POINT_DISTANCE)) {
        continue;
      }

      if (calculateDistance(lineStart, lineEnd, point) > POINT_TO_LINE_DISTANCE) {
        // qCDebug(phoo) << "calculateDistance: Distance acceptable (threshold: " << POINT_TO_LINE_DISTANCE << ")";
        break;
      }

      // qCDebug(phoo) << "calculateDistance: Distance too close (threshold: " << POINT_TO_LINE_DISTANCE << ")";
    }

    outerIndex = innerIndex;
  }

  QPointF lastPoint = naturalizedPoints.at(naturalizedPoints.size() - 1);

  // assure that the last point or at least a point close to it is taken, too
  if (result.size() >= 1 && minDistance(result.last(), lastPoint, LINE_SEGMENT_MIN_POINT_DISTANCE)) {
    // qCDebug(phoo) << "optimizeLine: Adding final point " << lastPoint << " to optimized list";
    result << lastPoint;
  }

  // a polygon is closed when the first and last points are equal - so we check the distance and make them equal if needed to
  // force a closed polygon
  if (!minDistance(result.first(), result.last(), START_END_POINT_DISTANCE)) {
    qCDebug(phoo) << "start and end points too close together, forcing closed polygon";
    result.last() = result.first();
  }

  qCDebug(phoo) << "optimized polygon has" << result.count() << "vertices";

  //  for (int o = 0; o < result.count(); o++) {
  //    qCDebug(phoo) << "optimized point list at " << o << ": " << result.at(o);
  //  }

  return result;
}

float PhysicsObjectOptimizer::calculateDistance(const QPointF &lineStart, const QPointF &lineEnd, const QPointF &point)
{
  float result = (qAbs((lineEnd.x() - lineStart.x()) * (lineStart.y() - point.y())
                       - (lineStart.x() - point.x()) * (lineEnd.y() - lineStart.y()))
                  / qSqrt((lineEnd.x() - lineStart.x()) * (lineEnd.x() - lineStart.x())
                          + (lineEnd.y() - lineStart.y()) * (lineEnd.y() - lineStart.y())));

  // qCDebug(phoo) << "calculateDistance: Point " << point << "distance to line between " << lineStart << " and " << lineEnd << "
  // is " << result;

  return result;
}

bool PhysicsObjectOptimizer::minDistance(const QPointF &pointOne, const QPointF &pointTwo, int iDistance)
{
  int xDistance = qAbs(pointOne.x() - pointTwo.x());
  int yDistance = qAbs(pointOne.y() - pointTwo.y());

  if (xDistance < iDistance && yDistance < iDistance) {
    // qCDebug(phoo) << "minDistance: Point " << pointOne << "and " << pointTwo << " too close to each other with distance (" <<
    // xDistance << ", " << yDistance << ") (threshold: " << iDistance << "))";
    return false;
  }

  // qCDebug(phoo) << "minDistance: Point " << pointOne << "and " << pointTwo << " distant enough with (" << xDistance << ", " <<
  // yDistance << ") (threshold: " << iDistance << "))";
  return true;
}

bool PhysicsObjectOptimizer::isPolygon(const QList<QPointF> &points)
{
  return points.size() > 2 && points.first() == points.last();
}

QList<QPolygonF> PhysicsObjectOptimizer::createLine(const QList<QPointF> &input)
{
  QList<QPolygonF> result;

  if (input.count() < 2) {
    return result;
  }

  for (int i = 0; i < input.count() - 1; i++) {

    QPointF firstPoint(input.at(i + 0).x(), input.at(i + 0).y());
    QPointF secondPoint(input.at(i + 1).x(), input.at(i + 1).y());

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
      result << lineShape;
    }
  }

  return result;
}

QList<QPolygonF> PhysicsObjectOptimizer::createPolygon(const QList<QPointF> &input)
{
  QList<QPolygonF> result;

  for (int i = 0; i < input.count() - 2; i += 3) {
    QPolygonF polygonShape;

    polygonShape << QPointF(input.at(i + 0).x(), input.at(i + 0).y());
    polygonShape << QPointF(input.at(i + 1).x(), input.at(i + 1).y());
    polygonShape << QPointF(input.at(i + 2).x(), input.at(i + 2).y());

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
      result << polygonShape;
    }
  }

  return result;
}

bool PhysicsObjectOptimizer::lineSelfIntersecting(const QList<QPointF> &points)
{
  QPointF intersection;
  bool isIntersecting = false;

  // check all linesegments against each other no need to check last pair
  // NOTE: this can be optimized: line segments next to each other can't intersect
  for (int i = 0; i < points.count() - 1; i++) {
    for (int j = 0; j < points.count() - 1; j++) {
      if (j == points.count()) {
        isIntersecting = calculateLineIntersection(points.at(i), points.at(i + 1), points.at(j), points.at(0), intersection);
      } else {
        isIntersecting = calculateLineIntersection(points.at(i), points.at(i + 1), points.at(j), points.at(j + 1), intersection);
      }

      return isIntersecting;
    }
  }

  return false;
}

bool PhysicsObjectOptimizer::calculateLineIntersection(const QPointF &one, const QPointF &two, const QPointF &three,
                                                       const QPointF &four, QPointF &intersection)
{
  float x1 = one.x();
  float y1 = one.y();

  float x2 = two.x();
  float y2 = two.y();

  float x3 = three.x();
  float y3 = three.y();

  float x4 = four.x();
  float y4 = four.y();

  // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

  // same for both equations
  float denominator = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

  if (denominator == 0.0f) {
    // coincidence of points or lines are parallel
    return false;
  }

  float numerator1 = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
  float numerator2 = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);

  float ua = numerator1 / denominator;
  float ub = numerator2 / denominator;

  // If ua and ub lie within the range of 0 to 1 then the intersection point is within both line segments.
  if (ua > 0.0f && ua < 1.0f && ub > 0.0f && ub < 1.0f) {

    float intersectX, intersectY;

    // intersection found
    intersectX = x1 + (ua * (x2 - x1));
    intersectY = y1 + (ua * (y2 - y1));

    // check if intersection equals one of the 4 input vertices
    if (((intersectX == x1) && (intersectY == y1)) || ((intersectX == x2) && (intersectY == y2))
        || ((intersectX == x3) && (intersectY == y3)) || ((intersectX == x4) && (intersectY == y4))) {

      return false;
    }

    intersection = QPointF(intersectX, intersectY);

    return true;

  } else {
    // no intersection
    return false;
  }
}
