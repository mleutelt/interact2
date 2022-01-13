/*
 *  ConcaveChopper.cpp
 *  interact
 *
 *  Created by Daniel Kunz on 23.04.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "concavechopper.h"

#include <iostream>
#include <iterator>
#include <math.h>
#include <iterator>

ConcaveChopper::ConcaveChopper( QList< QPointF >* vertices )
{
    originalQPointFVector = vertices;
    numberOfSegments = originalQPointFVector->size();

    resultVector = new QList< QPointF >();

    // check if polygon is simple
    m_IsSimplePolygon = !findIntersection();
}

ConcaveChopper::~ConcaveChopper()
{
    delete resultVector;
}

QList< QPointF >* ConcaveChopper::checkAllIntersections()
{
    // if polygon is non-simple
    if( !m_IsSimplePolygon )
    {
        // create a closed line and return it
        originalQPointFVector->push_back( originalQPointFVector->at( 0 ) );
        return originalQPointFVector;
    }
    else
    {
        // if polygon is simple, triangulate before returning it
        process( originalQPointFVector, resultVector );
        return resultVector;
    }
}

bool ConcaveChopper::findIntersection() {

    QPointF *intersection = Q_NULLPTR;

    // check all linesegments against each other
    // no need to check last pair
    for( int i = 0; i < ( int ) originalQPointFVector->size() - 1; i++ )
    {
        for( int j = 0; j < ( int ) originalQPointFVector->size() - 1; j++ )
        {
            if( j == ( int ) originalQPointFVector->size() )
            {
                intersection = calculateLineIntersection( &(originalQPointFVector->at( i )) , &(originalQPointFVector->at( i + 1 )),
                                                          &(originalQPointFVector->at( j )), &(originalQPointFVector->at( 0 )) );
            }
            else
            {
                intersection = calculateLineIntersection( &(originalQPointFVector->at( i )) , &(originalQPointFVector->at( i + 1 )),
                                                          &(originalQPointFVector->at( j )), &(originalQPointFVector->at( j + 1 )) );
            }

            // if an intersection was found, the polygon is non-simple
            if( intersection != Q_NULLPTR )
            {
                delete intersection;
                return true;
            }
        }
    }

    // simple polygon
    return false;
}

QPointF *ConcaveChopper::calculateLineIntersection( const QPointF *one, const QPointF *two, const QPointF *three, const QPointF *four )
{
    x1 = one->x();
    y1 = one->y();

    x2 = two->x();
    y2 = two->y();

    x3 = three->x();
    y3 = three->y();

    x4 = four->x();
    y4 = four->y();

    // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

    // same for both equations
    float denominator = ( y4 - y3 ) * ( x2 - x1 ) - ( x4 - x3 ) * ( y2 - y1 );

    if( denominator == 0.0f )
    {
        // coincidence of points or lines are parallel
        return Q_NULLPTR;
    }

    float numerator1 = ( x4 - x3 ) * ( y1 - y3 ) - ( y4 - y3 ) * ( x1 - x3 );
    float numerator2 = ( x2 - x1 ) * ( y1 - y3 ) - ( y2 - y1 ) * ( x1 - x3 );

    float ua = numerator1 / denominator;
    float ub = numerator2 / denominator;

    // If ua and ub lie within the range of 0 to 1 then the intersection point is within both line segments.
    if( ua > 0.0f && ua < 1.0f && ub > 0.0f && ub < 1.0f ) {

        // intersection found
        intersectX = x1 + ( ua * ( x2 - x1 ) );
        intersectY = y1 + ( ua * ( y2 - y1 ) );

        // check if intersection equals one of the 4 input vertices
        if( (( intersectX == x1 ) && ( intersectY == y1 )) ||
                (( intersectX == x2 ) && ( intersectY == y2 )) ||
                (( intersectX == x3 ) && ( intersectY == y3 )) ||
                (( intersectX == x4 ) && ( intersectY == y4 )) ) {

            return Q_NULLPTR;
        }

        return ( new QPointF( intersectX, intersectY ) );

    } else {

        // no intersection
        return Q_NULLPTR;
    }
}

// TRIANGULATION BEGIN

// ** THIS IS A CODE SNIPPET WHICH WILL EFFICIEINTLY TRIANGULATE ANY
// ** POLYGON/CONTOUR (without holes) AS A STATIC CLASS.  THIS SNIPPET
// ** IS COMPRISED OF 3 FILES, TRIANGULATE.H, THE HEADER FILE FOR THE
// ** TRIANGULATE BASE CLASS, TRIANGULATE.CPP, THE IMPLEMENTATION OF
// ** THE TRIANGULATE BASE CLASS, AND TEST.CPP, A SMALL TEST PROGRAM
// ** DEMONSTRATING THE USAGE OF THE TRIANGULATOR.  THE TRIANGULATE
// ** BASE CLASS ALSO PROVIDES TWO USEFUL HELPER METHODS, ONE WHICH
// ** COMPUTES THE AREA OF A POLYGON, AND ANOTHER WHICH DOES AN EFFICENT
// ** POINT IN A TRIANGLE TEST.
// ** SUBMITTED BY JOHN W. RATCLIFF (jratcliff@verant.com) July 22, 2000
// ** http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml

static const float EPSILON = 0.0000000001f;

float ConcaveChopper::area( QList< QPointF > *contour ) {

    int n = contour->size();

    float A = 0.0f;

    for( int p = n - 1, q = 0; q < n; p = q++) {

        A += ( contour->at( p ).x() ) * ( contour->at( q ).y() ) - ( contour->at( q ).x() ) * ( contour->at( p ).y() );

    }

    return A * 0.5f;

}

/*
 InsideTriangle decides if a point P is Inside of the triangle
 defined by A, B, C.
 */
bool ConcaveChopper::insideTriangle( float Ax, float Ay,
                                     float Bx, float By,
                                     float Cx, float Cy,
                                     float Px, float Py ) {

    float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
    float cCROSSap, bCROSScp, aCROSSbp;

    ax = Cx - Bx;  ay = Cy - By;
    bx = Ax - Cx;  by = Ay - Cy;
    cx = Bx - Ax;  cy = By - Ay;
    apx= Px - Ax;  apy= Py - Ay;
    bpx= Px - Bx;  bpy= Py - By;
    cpx= Px - Cx;  cpy= Py - Cy;

    aCROSSbp = ax*bpy - ay*bpx;
    cCROSSap = cx*apy - cy*apx;
    bCROSScp = bx*cpy - by*cpx;

    return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));

}

bool ConcaveChopper::snip( QList< QPointF > *contour, int u, int v, int w, int n, int *V ) {

    int p;
    float Ax, Ay, Bx, By, Cx, Cy, Px, Py;

    Ax = (*contour)[V[u]].x();
    Ay = (*contour)[V[u]].y();

    Bx = (*contour)[V[v]].x();
    By = (*contour)[V[v]].y();

    Cx = (*contour)[V[w]].x();
    Cy = (*contour)[V[w]].y();

    if ( EPSILON > ((( Bx - Ax ) * ( Cy - Ay )) - (( By - Ay ) * ( Cx - Ax ))) ) return false;

    for ( p = 0; p < n; p++)
    {
        if( (p == u) || (p == v) || (p == w) ) continue;
        Px = (*contour)[V[p]].x();
        Py = (*contour)[V[p]].y();
        if ( insideTriangle( Ax, Ay, Bx, By, Cx, Cy, Px, Py )) return false;
    }

    return true;
}

bool ConcaveChopper::process( QList< QPointF > *contour, QList< QPointF > *result) {

    /* allocate and initialize list of Vertices in polygon */

    int n = contour->size();
    if ( n < 3 ) return false;

    int *V = new int[ n ];

    /* we want a counter-clockwise polygon in V */

    if ( 0.0f < area( contour ) )
        for ( int v = 0; v < n; v++ ) V[v] = v;
    else
        for( int v = 0; v < n; v++ ) V[v] = ( n - 1 ) - v;

    int nv = n;

    /*  remove nv-2 Vertices, creating 1 triangle every time */
    int count = 2 * nv;   /* error detection */

    for( int m = 0, v = nv - 1; nv > 2; ) {
        /* if we loop, it is probably a non-simple polygon */
        if ( 0 >= ( count-- ) ) {
            //** Triangulate: ERROR - probable bad polygon!
            return false;
        }

        /* three consecutive vertices in current polygon, <u,v,w> */
        int u = v  ; if ( nv <= u ) u = 0;     /* previous */
        v = u + 1; if ( nv <= v ) v = 0;     /* new v    */
        int w = v + 1; if ( nv <= w ) w = 0;     /* next     */

        if ( snip( contour, u, v, w, nv, V ) )
        {
            int a, b, c, s, t;

            /* true names of the vertices */
            a = V[u]; b = V[v]; c = V[w];

            /* output Triangle */
            result->push_back( (*contour)[a] );
            //std::cout<< (*contour)[a].x() << ", " << (*contour)[a].y() << std::endl;
            result->push_back( (*contour)[b] );
            //std::cout<< (*contour)[b].x() << ", " << (*contour)[b].y() << std::endl;
            result->push_back( (*contour)[c] );
            //std::cout<< (*contour)[c].x() << ", " << (*contour)[c].y() << std::endl;
            //std::cout<< result->size()<< std::endl;


            m++;

            /* remove v from remaining polygon */
            for( s = v, t = v + 1; t < nv ; s++, t++) V[s] = V[t]; nv--;

            /* resest error detection counter */
            count = 2 * nv;

        }

    }

    delete[] V;

    return true;

}
