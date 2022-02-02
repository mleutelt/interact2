#pragma once

#include <QAbstractListModel>
#include <QUrl>

class LevelDescription
{
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
  enum Roles
  {
    Name,
    Preview,
    Path
  };
  Q_ENUM(Roles)

  explicit LevelModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  void addLevel(const LevelDescription &level);
  void clear();
  LevelDescription levelAtIndex(int index) const;
  int indexOf(const QString &name) const;

private:
  QList<LevelDescription> m_data;
};
