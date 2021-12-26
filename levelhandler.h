#pragma once

#include <QObject>
#include <QQmlEngine>

#include "levelmodel.h"

class LevelHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LevelModel* availableLevels READ availableLevels CONSTANT)
    Q_PROPERTY(LevelModel* userLevels READ userLevels CONSTANT)
    QML_ELEMENT

public:
    explicit LevelHandler(QObject *parent = nullptr);

    LevelModel *availableLevels() const;
    LevelModel *userLevels() const;

    Q_INVOKABLE void loadLevel(const QString &path);

signals:

private:
    LevelModel *m_availableLevels = nullptr;
    LevelModel *m_userLevels = nullptr;
};
