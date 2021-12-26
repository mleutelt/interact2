#pragma once

#include <QAbstractListModel>

class ObjectDescription {
    Q_GADGET
};

class LevelData {
    Q_GADGET

public:
    QString name;
    QList<ObjectDescription> objects;
};

class LevelDataModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

public:
    enum Roles {

    };
    Q_ENUM(Roles)

    explicit LevelDataModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString name() const;

    void setLevelData(const LevelData &data);

signals:
    void nameChanged();

private:
    void setName(const QString &name);

    QList<LevelData> m_data;
    QString m_name;
};
