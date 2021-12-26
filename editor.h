#pragma once

#include <QObject>
#include <QQmlEngine>

class Editor : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Editor(QObject *parent = nullptr);

signals:

private:

};
