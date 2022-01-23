import QtQuick

import App

import Box2D

Rectangle {
    id: container

    property color itemColor: "black"
    property bool interactive: true
    property bool hovered: false
    property bool clickEnabled: false
    property var clickHandler
    property bool hoverEnabled: false
    property var hoverHandler
    property bool dragEnabled: false
    property var dragHandler
    property bool wheelEnabled: false
    property var wheelHandler
    property bool pinchEnabled: false
    property var pinchHandler
    property alias visualItem: childContainer.children
    property bool soundsEnabled: true
    property bool showBoundingBox: false

    property alias physicalObject: body.fixtures
    property Body body: Body {
        id: body

        target: container
    }

    // Body properties
    property alias world: body.world
    property alias linearDamping: body.linearDamping
    property alias angularDamping: body.angularDamping
    property alias bodyType: body.bodyType
    property alias bullet: body.bullet
    property alias sleepingAllowed: body.sleepingAllowed
    property alias fixedRotation: body.fixedRotation
    property alias active: body.active
    property alias awake: body.awake
    property alias linearVelocity: body.linearVelocity
    property alias angularVelocity: body.angularVelocity
    property alias fixtures: body.fixtures
    property alias gravityScale: body.gravityScale

    signal beginContact(other: Fixture)
    signal endContact(other: Fixture)

    color: App.debugBoundingBoxes || showBoundingBox ? "red" : "transparent"
    opacity: App.debugBoundingBoxes || showBoundingBox ? 0.5 : 1

    implicitWidth: childContainer.childrenRect.width
    implicitHeight: childContainer.childrenRect.height

    Item {
        id: childContainer

        anchors.fill: parent
    }

    Component.onCompleted: {
        if (soundsEnabled)
            Sound.playSound(Sound.Plopp)
    }
    Component.onDestruction: {
        if (soundsEnabled)
            Sound.playSound(Sound.PaperCrumple)
    }
}
