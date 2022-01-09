#ifndef PHYSICSOBJECTOPTIMIZER_H
#define PHYSICSOBJECTOPTIMIZER_H

#include <QObject>
#include <QPointF>
#include <QPolygonF>

class PhysicsObjectOptimizer : public QObject
{
    Q_OBJECT
public:
    explicit PhysicsObjectOptimizer(QObject *parent = nullptr);

    Q_INVOKABLE void clear();
    Q_INVOKABLE void addVertex(qreal vertexX, qreal vertexY);
    Q_INVOKABLE void idealizeLine();
    Q_INVOKABLE QPolygonF* getPolygonOrLine();
    Q_INVOKABLE bool isClosed();

private:
    QPolygonF m_PolygonFRaw;
    QPolygonF m_PolygonFOptimized;

    QPointF qpfLineStart, qpfLineEnd, qpfPoint;

    float fLineStartX, fLineStartY, fLineEndX, fLineEndY, fPointX, fPointY;

    int iSkip;

    float calculateDistance(QPointF qpfLineStart, QPointF qpfLineEnd, QPointF qpfPoint);
    bool minDistance(QPointF pointOne, QPointF pointTwo);
signals:

};

#endif // PHYSICSOBJECTOPTIMIZER_H
