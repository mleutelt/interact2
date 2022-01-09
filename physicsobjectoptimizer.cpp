#include "physicsobjectoptimizer.h"
#include <QDebug>

#define DISTANCE 5
#define SKIP 5
#define POINTDISTANCE 5

PhysicsObjectOptimizer::PhysicsObjectOptimizer(QObject *parent)
    : QObject{parent}
{
}

void PhysicsObjectOptimizer::clear()
{
    m_PolygonFRaw.clear();
    m_PolygonFOptimized.clear();
    qDebug() << "Clear: Raw vertex input has now " << m_PolygonFRaw.size() << " vertices.";
    qDebug() << "Clear: Optimized vertex input has now " << m_PolygonFOptimized.size() << " vertices.";
}

void PhysicsObjectOptimizer::addVertex(qreal vertexX, qreal vertexY)
{
    qDebug() << "Adding polygon point (" << vertexX << ", " << vertexY << ")";
    m_PolygonFRaw << QPointF(vertexX, vertexY);
}

void PhysicsObjectOptimizer::idealizeLine()
{
    qDebug() << "calling idealizeLine";
    qDebug() << "Raw vertex input has " << m_PolygonFRaw.size() << " vertices before optimization.";

    // if vector is shorter than skip distance
    if(m_PolygonFRaw.size() < SKIP)
    {
        m_PolygonFOptimized = m_PolygonFRaw;
    }

    int i,j;

    // TODO: Falsches Inkrement, aber sobald dort "+=" steht, wird die Linie zu stark vereinfacht!
    for( i = 0; i < ( int ) ( m_PolygonFRaw.size() - SKIP ); i + SKIP )
    {
        // take two points for a line
        qpfLineStart = m_PolygonFRaw.at( i );
        qpfLineEnd = m_PolygonFRaw.at( i + SKIP );

        // the first point is in the result set anyway
        m_PolygonFOptimized <<  qpfLineStart;

        // check all points inbetween if they are within a defined range from the line
        for( j = i + 1; j < i + SKIP; j++ )
        {
            qpfPoint = m_PolygonFRaw.at( j );

            if( !minDistance( qpfLineStart, qpfPoint ) )
            {
                continue;
            }

            if( calculateDistance( qpfLineStart, qpfLineEnd, qpfPoint ) > DISTANCE )
            {
                break;
            }
        }

        i = j;
    }

    // assure that the last point or at least a point close to it is taken, too
    if( m_PolygonFOptimized.size() == 1 && minDistance( m_PolygonFOptimized.back(), m_PolygonFRaw.at( m_PolygonFRaw.size() - 1 ) ) )
    {
        m_PolygonFOptimized <<  m_PolygonFRaw.at( m_PolygonFRaw.size() - 1 );
    }
    else if( m_PolygonFOptimized.size() > 1 && minDistance( m_PolygonFOptimized.back(), m_PolygonFRaw.at( m_PolygonFRaw.size() - 1 ) ) )
    {
        m_PolygonFOptimized <<  m_PolygonFRaw.at( m_PolygonFRaw.size() - 1 );
    }

    // a polygon is closed when the first and last points are equal - so we check the distance and make them equal if needed to
    // force a closed polygon
    if (minDistance(m_PolygonFOptimized.first(), m_PolygonFOptimized.last()))
    {
        qDebug() << "Polygon starting point and end point are too close together. Making them the same point.";
        m_PolygonFOptimized.last() = m_PolygonFOptimized.first();
    }

    qDebug() << "Optimized polygon now has " << m_PolygonFOptimized.size() << " vertices.";
}

QPolygonF* PhysicsObjectOptimizer::getPolygonOrLine()
{
    return &m_PolygonFOptimized;
}

bool PhysicsObjectOptimizer::isClosed()
{
    return m_PolygonFOptimized.isClosed();
}

// calculate the distance of a point to a line
float PhysicsObjectOptimizer::calculateDistance( QPointF qpfLineStart, QPointF qpfLineEnd, QPointF qpfPoint )
{
    fLineStartX = qpfLineStart.x();
    fLineStartY = qpfLineStart.y();

    fLineEndX = qpfLineEnd.x();
    fLineEndY = qpfLineEnd.y();

    fPointX = qpfPoint.x();
    fPointY = qpfPoint.y();

    return ( abs( ( fLineEndX - fLineStartX ) * ( fLineStartY - fPointY ) - ( fLineStartX - fPointX ) * ( fLineEndY - fLineStartY ) ) /
             sqrt( ( fLineEndX - fLineStartX ) * ( fLineEndX - fLineStartX ) + ( fLineEndY - fLineStartY ) * ( fLineEndY - fLineStartY ) ) );
}

// calculate the distance between two points
bool PhysicsObjectOptimizer::minDistance( QPointF pointOne, QPointF pointTwo )
{
    int xDistance = abs( pointOne.x() - pointTwo.x() );
    int yDistance = abs( pointOne.y() - pointTwo.y() );

    if( xDistance < POINTDISTANCE && yDistance < POINTDISTANCE )
    {
        return false;
    }

    return true;
}
