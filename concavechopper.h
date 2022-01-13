/*
 *  ConcaveChopper.h
 *  interact
 *
 *  Created by Daniel Kunz on 23.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONCAVECHOPPER_H
#define CONCAVECHOPPER_H

#include <QPointF>
#include <QList>

/// A class for triangulating polygons.
class ConcaveChopper {

public:

    /** Constructor.

   @param vertices a vector with an objects vertices.
   */
    ConcaveChopper( QList< QPointF > *vertices );

    /// Destructor.
    ~ConcaveChopper();

    /**
   @return true if the polygon is NOT simple, otherwise false.
   */

    inline bool isSimplePolygon()
    {
        return m_IsSimplePolygon;
    }

    /** Checks if a given line has any intersection.

   If an intersection is found, the method returns.
   The polygon is then not simple and is converted to a closed line.

   @return a vector containing either the original vertices or the triangulated polygon.
   */

    QList< QPointF > *checkAllIntersections();

private:

    QPointF *calculateLineIntersection( const QPointF *one, const QPointF *two, const QPointF *three, const QPointF *four );

    void addIntersections( int indexOfStartQPointF );

    void collectSimplePolygon( int index );

    bool findIntersection();

    float x1, y1, x2, y2, x3, y3, x4, y4;

    float intersectX, intersectY;

    int numberOfSegments;

    // QPointF *leftmostIntersection, *rightmostIntersection;

    QList< QPointF > *resultVector, *simplePolygon, *originalQPointFVector, *newQPointFVector;

    // triangulate a contour/polygon, places results in STL vector
    // as series of triangles.
    static bool process( QList< QPointF > *contour,
                         QList< QPointF > *result );

    // compute area of a contour/polygon
    static float area( QList< QPointF > *contour );

    // decide if point Px/Py is inside triangle defined by
    // (Ax,Ay) (Bx,By) (Cx,Cy)
    static bool insideTriangle( float Ax, float Ay,
                                float Bx, float By,
                                float Cx, float Cy,
                                float Px, float Py );

    static bool snip( QList< QPointF > *contour, int u, int v, int w, int n, int *V );

    // member variables
    bool m_IsSimplePolygon;
};

#endif // CONCAVECHOPPER_H
