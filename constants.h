#pragma once

#include <QObject>
#include <QtQml/qqml.h>

namespace Constants {
  Q_NAMESPACE
  QML_ELEMENT

  enum ShapeType
  {
    ShapeType_Circle,
    ShapeType_Rectangle,
    ShapeType_Polygon,
    ShapeType_Line,
    ShapeType_SpecialStar,
  };
  Q_ENUM_NS(ShapeType)
}
