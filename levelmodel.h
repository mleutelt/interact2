#pragma once

#include <QAbstractListModel>
#include <QUrl>

class LevelData {
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QUrl preview MEMBER preview)
    Q_PROPERTY(QString path MEMBER path)

public:
    QString name;
    QUrl preview;
    QString path;
};

class LevelModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        Name,
        Preview,
        Path
    };
    Q_ENUM(Roles)

    explicit LevelModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addLevel(const LevelData& level);

signals:

private:
    QList<LevelData> m_data;
};
