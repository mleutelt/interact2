import QtQuick
import Qt.labs.qmlmodels

import App

import Box2D

Instantiator {
    id: factory

    required property Item level
    required property World world
    property var interactionHandler: function(index) {}
    property LoggingCategory log: LoggingCategory {
        name: "app.qml.physicsobjectfactory"
    }

    model: []
    delegate: DelegateChooser {
        role: "type"

        DelegateChoice {
            roleValue: Editor.ShapeType_Circle

            delegate: PCircle {
                world: factory.world
                bodyType: model.static ? Body.Static : Body.Dynamic

                parent: factory.level
                interactionHandler: factory.interactionHandler
                x: model.boundingBox.x
                y: model.boundingBox.y
                width: Math.min(model.boundingBox.width, model.boundingBox.height)
                height: Math.min(model.boundingBox.height, model.boundingBox.width)
                item.color: Qt.rgba(Math.random(), Math.random(), Math.random())

                Component.onCompleted: console.log(log, "Circle created")
            }
        }
        DelegateChoice {
            roleValue: Editor.ShapeType_Rectangle

            delegate: PRectangle {
                world: factory.world
                bodyType: model.static ? Body.Static : Body.Dynamic

                parent: factory.level
                interactionHandler: factory.interactionHandler
                x: model.boundingBox.x
                y: model.boundingBox.y
                width: model.boundingBox.width
                height: model.boundingBox.height

                item.color: Qt.rgba(Math.random(), Math.random(), Math.random())

                Component.onCompleted: console.log(log, "Rectangle created")
            }
        }
        DelegateChoice {
            roleValue: Editor.ShapeType_Polygon

            delegate: PPolygon {
                world: factory.world
                bodyType: model.static ? Body.Static : Body.Dynamic

                parent: factory.level
                interactionHandler: factory.interactionHandler
                x: model.boundingBox.x
                y: model.boundingBox.y
                width: Math.min(model.boundingBox.width, model.boundingBox.height)
                height: Math.min(model.boundingBox.height, model.boundingBox.width)
                item.color: Qt.rgba(Math.random(), Math.random(), Math.random())

                Component.onCompleted: console.log(log, "Polygon created")
            }
        }
    }
}
