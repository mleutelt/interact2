/*
 *  ConcaveChopper.h
 *  interact
 *
 *  Created by Daniel Kunz on 23.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <QPointF>
#include <QList>

/// A class for triangulating polygons.
class ConcaveChopper
{

public:
  // triangulate a contour/polygon, places results in STL vector
  // as series of triangles.
  static QList<QPointF> triangulate(QList<QPointF> &contour);

  // compute area of a contour/polygon
  static float area(const QList<QPointF> &contour);

  // decide if point Px/Py is inside triangle defined by
  // (Ax,Ay) (Bx,By) (Cx,Cy)
  static bool insideTriangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Px, float Py);

  static bool snip(QList<QPointF> &contour, int u, int v, int w, int n, int *V);
};
