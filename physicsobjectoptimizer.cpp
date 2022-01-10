#include "physicsobjectoptimizer.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(phoo, "app.physicsobjectoptimizer")

namespace {
  const int DISTANCE = 3;
  const int SKIP = 2;
  const int POINTDISTANCE = 2;
}

PhysicsObjectOptimizer::PhysicsObjectOptimizer(QObject *parent)
  : QObject { parent }
{
}

QPolygonF PhysicsObjectOptimizer::optimizeLine(const QList<QPointF> &points)
{
  qCDebug(phoo) << "optimizing line with points" << points.count();

  QPolygonF result;

  // if vector is shorter than skip distance
  if (points.count() < SKIP) {
    result = points;

    return result;
  }

  int outerIndex = 0;
  int innerIndex = 0;

  // TODO: Falsches Inkrement, aber sobald dort "+=" steht, wird die Linie zu stark vereinfacht!
  for (outerIndex = 0; outerIndex < points.count() - SKIP; outerIndex += SKIP) {
    // take two points for a line
    QPointF lineStart = points.at(outerIndex);
    QPointF lineEnd = points.at(outerIndex + SKIP);

    // the first point is in the result set anyway
    result << lineStart;

    // check all points inbetween if they are within a defined range from the line
    for (innerIndex = outerIndex + 1; innerIndex < outerIndex + SKIP; innerIndex++) {
      QPointF point = points.at(innerIndex);

      if (!minDistance(lineStart, point)) {
        continue;
      }

      if (calculateDistance(lineStart, lineEnd, point) > DISTANCE) {
        break;
      }
    }

    outerIndex = innerIndex;
  }

  // assure that the last point or at least a point close to it is taken, too
  if (result.size() == 1 && minDistance(result.last(), points.at(points.size() - 1))) {
    result << points.at(points.size() - 1);
  } else if (result.size() > 1 && minDistance(result.last(), points.at(points.size() - 1))) {
    result << points.at(points.size() - 1);
  }

  // a polygon is closed when the first and last points are equal - so we check the distance and make them equal if needed to
  // force a closed polygon
  if (minDistance(result.first(), result.last())) {
    qCDebug(phoo) << "start and end points too close together, forcing closed polygon";
    result.last() = result.first();
  }

  qCDebug(phoo) << "optimized polygon has" << result.count() << "vertices";

  return result;
}

float PhysicsObjectOptimizer::calculateDistance(const QPointF &lineStart, const QPointF &lineEnd, const QPointF &point)
{
  return (qAbs((lineEnd.x() - lineStart.x()) * (lineStart.y() - point.y())
               - (lineStart.x() - point.x()) * (lineEnd.y() - lineStart.y()))
          / qSqrt((lineEnd.x() - lineStart.x()) * (lineEnd.x() - lineStart.x())
                  + (lineEnd.y() - lineStart.y()) * (lineEnd.y() - lineStart.y())));
}

bool PhysicsObjectOptimizer::minDistance(const QPointF &pointOne, const QPointF &pointTwo)
{
  int xDistance = qAbs(pointOne.x() - pointTwo.x());
  int yDistance = qAbs(pointOne.y() - pointTwo.y());

  if (xDistance < POINTDISTANCE && yDistance < POINTDISTANCE) {
    return false;
  }

  return true;
}
