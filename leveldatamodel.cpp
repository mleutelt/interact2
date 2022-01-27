#include "leveldatamodel.h"

#include <QPolygonF>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lvldm, "app.models.leveldatamodel")

LevelDataModel::LevelDataModel(QObject *parent)
  : QAbstractListModel { parent }
{
}

int LevelDataModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)

  return m_objects.count();
}

QVariant LevelDataModel::data(const QModelIndex &index, int role) const
{
  if (index.row() < 0 || index.row() >= m_objects.count())
    return QVariant();

  const ObjectDescription &data = m_objects[index.row()];

  switch (role) {
  case Type:
    return data.type;
  case BoundingBox:
    return data.boundingBox;
  case Static:
    return data.isStatic;
  case Polygon:
    return data.polygon;
  case Points:
    return QVariant::fromValue(data.points);
  }

  return QVariant();
}

QHash<int, QByteArray> LevelDataModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[Type] = "type";
  roles[BoundingBox] = "boundingBox";
  roles[Static] = "static";
  roles[Polygon] = "polygon";
  roles[Points] = "points";

  return roles;
}

QString LevelDataModel::name() const
{
  return m_name;
}

void LevelDataModel::setLevelData(const LevelData &data)
{
  setName(data.name);
  setBackgroundImage(data.backgroundImage);
  setMusic(data.music);

  beginResetModel();
  m_objects = data.objects;
  endResetModel();
}

void LevelDataModel::addSimpleObject(int type, const QRectF &boundingRect, bool isStatic)
{
  ObjectDescription objectDescription = {
    type, boundingRect, {}, {}, isStatic,
  };

  qCDebug(lvldm) << "adding simple object" << objectDescription << m_objects.count();

  beginInsertRows(QModelIndex(), m_objects.count(), m_objects.count());
  m_objects << objectDescription;
  endInsertRows();
}

void LevelDataModel::addPolygonObject(int type, const QPolygonF &originalPoints, const QList<QPolygonF> &optimizedPoints,
                                      bool isStatic)
{
  ObjectDescription objectDescription = {
    type, originalPoints.boundingRect(), originalPoints, polygonListToVariantList(optimizedPoints), isStatic,
  };

  qCDebug(lvldm) << "adding polygon object" << objectDescription << m_objects.count();

  beginInsertRows(QModelIndex(), m_objects.count(), m_objects.count());
  m_objects << objectDescription;
  endInsertRows();
}

void LevelDataModel::addLineObject(int type, const QPolygonF &originalPoints, const QList<QPolygonF> &lineSegments, bool isStatic)
{
  ObjectDescription objectDescription = {
    type, originalPoints.boundingRect(), originalPoints, polygonListToVariantList(lineSegments), isStatic,
  };

  qCDebug(lvldm) << "adding line object" << objectDescription << m_objects.count();

  beginInsertRows(QModelIndex(), m_objects.count(), m_objects.count());
  m_objects << objectDescription;
  endInsertRows();
}

void LevelDataModel::removeObject(int index)
{
  qCDebug(lvldm) << "removing object with index" << index;

  beginRemoveRows(QModelIndex(), index, index);
  m_objects.remove(index);
  endRemoveRows();
}

void LevelDataModel::clear()
{
  qCDebug(lvldm) << "clearing";

  setName(QString());
  setBackgroundImage(QUrl());
  setMusic(QUrl());

  beginResetModel();
  m_objects.clear();
  endResetModel();
}

QList<ObjectDescription> LevelDataModel::objects() const
{
  return m_objects;
}

void LevelDataModel::setName(const QString &name)
{
  if (m_name == name) {
    return;
  }

  m_name = name;
  emit nameChanged();
}

QList<QVariantList> LevelDataModel::polygonListToVariantList(const QList<QPolygonF> &list) const
{
  // NOTE: convert to list of QVariantLists, each QVariantList representing a triangle (in case of a object being
  // a closed polygon) created by triangulation or a rectangle (in case of a self-intersecting line) created by
  // line segmentation.
  // FIXME: move this code somewhere else
  QList<QVariantList> result;
  for (const QPolygonF &polygon : list) {
    QVariantList pointList;
    for (const QPointF &point : polygon) {
      pointList << point;
    }
    result << pointList;
  }

  return result;
}

QUrl LevelDataModel::backgroundImage() const
{
  return m_backgroundImage;
}

void LevelDataModel::setBackgroundImage(const QUrl &url)
{
  if (m_backgroundImage == url) {
    return;
  }

  m_backgroundImage = url;
  emit backgroundImageChanged();
}

QUrl LevelDataModel::music() const
{
  return m_music;
}

void LevelDataModel::setMusic(const QUrl &url)
{
  if (m_music == url) {
    return;
  }

  m_music = url;
  emit musicChanged();
}
