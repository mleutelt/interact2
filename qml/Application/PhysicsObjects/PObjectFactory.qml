import QtQuick
import Qt.labs.qmlmodels

import App

import Box2D

Instantiator {
    id: factory

    required property Item level
    required property World world
    property bool clickEnabled: false
    property var clickHandler: function(item, index, button) {}
    property bool hoverEnabled: false
    property var hoverHandler: function(item, hovered) {}
    property bool dragEnabled: false
    property var dragHandler: function(item, transition, point) {}
    property bool wheelEnabled: false
    property var wheelHandler: function(item, event, handler) {}
    property bool pinchEnabled: false
    property var pinchHandler: function(item) {}
    property var contactHandler: function(item, otherItem) {}

    property LoggingCategory log: LoggingCategory {
        name: "app.qml.physicsobjectfactory"
    }

    asynchronous: true
    model: []
    delegate: DelegateChooser {
        role: "type"

        DelegateChoice {
            roleValue: Constants.ShapeType_Circle

            delegate: PCircle {
                world: factory.world
                bodyType: model.static ? Body.Static : Body.Dynamic

                parent: factory.level
                clickEnabled: factory.clickEnabled
                clickHandler: factory.clickHandler
                hoverEnabled: factory.hoverEnabled
                hoverHandler: factory.hoverHandler
                dragEnabled: factory.dragEnabled
                dragHandler: factory.dragHandler
                wheelEnabled: factory.wheelEnabled
                wheelHandler: factory.wheelHandler
                pinchEnabled: factory.pinchEnabled
                pinchHandler: factory.pinchHandler
                contactHandler: factory.contactHandler
                isGameItem: model.gameItem
                x: model.boundingBox.x
                y: model.boundingBox.y
                width: Math.min(model.boundingBox.width, model.boundingBox.height)
                height: Math.min(model.boundingBox.height, model.boundingBox.width)
                invisible: model.invisible
                itemColor: model.invisible ? "transparent" : Qt.rgba(Math.random(), Math.random(), Math.random())

                Component.onCompleted: console.log(log, "Circle created")
            }
        }
        DelegateChoice {
            roleValue: Constants.ShapeType_Rectangle

            delegate: PRectangle {
                world: factory.world
                bodyType: model.static ? Body.Static : Body.Dynamic

                parent: factory.level
                clickEnabled: factory.clickEnabled
                clickHandler: factory.clickHandler
                hoverEnabled: factory.hoverEnabled
                hoverHandler: factory.hoverHandler
                dragEnabled: factory.dragEnabled
                dragHandler: factory.dragHandler
                wheelEnabled: factory.wheelEnabled
                wheelHandler: factory.wheelHandler
                pinchEnabled: factory.pinchEnabled
                pinchHandler: factory.pinchHandler
                contactHandler: factory.contactHandler
                isGameItem: model.gameItem
                x: model.boundingBox.x
                y: model.boundingBox.y
                width: model.boundingBox.width
                height: model.boundingBox.height
                invisible: model.invisible
                itemColor: model.invisible ? "transparent" : Qt.rgba(Math.random(), Math.random(), Math.random())

                Component.onCompleted: console.log(log, "Rectangle created")
            }
        }
        DelegateChoice {
            roleValue: Constants.ShapeType_Polygon

            delegate: PPolygon {
                world: factory.world
                bodyType: model.static ? Body.Static : Body.Dynamic

                parent: factory.level
                clickEnabled: factory.clickEnabled
                clickHandler: factory.clickHandler
                hoverEnabled: factory.hoverEnabled
                hoverHandler: factory.hoverHandler
                dragEnabled: factory.dragEnabled
                dragHandler: factory.dragHandler
                wheelEnabled: factory.wheelEnabled
                wheelHandler: factory.wheelHandler
                pinchEnabled: factory.pinchEnabled
                pinchHandler: factory.pinchHandler
                contactHandler: factory.contactHandler
                isGameItem: model.gameItem
                width: model.boundingBox.width
                height: model.boundingBox.height
                invisible: model.invisible
                itemColor: model.invisible ? "transparent" : t.rgba(Math.random(), Math.random(), Math.random())

                Component.onCompleted: console.log(log, "Polygon created")
            }
        }
        DelegateChoice {
            roleValue: Constants.ShapeType_Line

            delegate: PLine {
                world: factory.world
                bodyType: model.static ? Body.Static : Body.Dynamic

                parent: factory.level
                clickEnabled: factory.clickEnabled
                clickHandler: factory.clickHandler
                hoverEnabled: factory.hoverEnabled
                hoverHandler: factory.hoverHandler
                dragEnabled: factory.dragEnabled
                dragHandler: factory.dragHandler
                wheelEnabled: factory.wheelEnabled
                wheelHandler: factory.wheelHandler
                pinchEnabled: factory.pinchEnabled
                pinchHandler: factory.pinchHandler
                contactHandler: factory.contactHandler
                isGameItem: model.gameItem
                width: model.boundingBox.width
                height: model.boundingBox.height
                invisible: model.invisible
                itemColor: model.invisible ? "transparent" : Qt.rgba(Math.random(), Math.random(), Math.random())

                Component.onCompleted: console.log(log, "Line created")
            }
        }
        DelegateChoice {
            roleValue: Constants.ShapeType_SpecialStar

            delegate: PStar {
                world: factory.world
                bodyType: model.static ? Body.Static : Body.Dynamic

                parent: factory.level
                clickEnabled: factory.clickEnabled
                clickHandler: factory.clickHandler
                hoverEnabled: factory.hoverEnabled
                hoverHandler: factory.hoverHandler
                dragEnabled: factory.dragEnabled
                dragHandler: factory.dragHandler
                wheelEnabled: factory.wheelEnabled
                wheelHandler: factory.wheelHandler
                pinchEnabled: factory.pinchEnabled
                pinchHandler: factory.pinchHandler
                contactHandler: factory.contactHandler
                x: model.boundingBox.x
                y: model.boundingBox.y
                width: Math.min(model.boundingBox.width, model.boundingBox.height)
                height: Math.min(model.boundingBox.height, model.boundingBox.width)

                Component.onCompleted: console.log(log, "Star created")
            }
        }
    }
}
