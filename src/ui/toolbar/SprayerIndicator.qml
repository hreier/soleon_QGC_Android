import QtQuick          2.11
import QtQuick.Layouts  1.11
import QtGraphicalEffects   1.0

import QGroundControl                       1.0
import QGroundControl.Controls              1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.Palette               1.0
import MAVLink                              1.0

//-------------------------------------------------------------------------
//-- Battery Indicator
Item {
    id:             _root
    anchors.top:    parent.top
    anchors.bottom: parent.bottom
    width:          sprayerIndicatorRow.width

    property bool showIndicator: true

    property var _activeVehicle:  QGroundControl.multiVehicleManager.activeVehicle
    // We receive fill level in liters, and our tank is 30 liters
    property var _tankCapacity:   30
    property var _fillPercentage: _activeVehicle ? (_activeVehicle.sprayer.filllevel.value * 100 / _tankCapacity) : 0

    // Only visible if any of the flags is visible, there is no point otherwise
    property var _statusVisible:  _activeVehicle &&
                                  _activeVehicle.sprayer.sprayingRight ||
                                  _activeVehicle.sprayer.sprayingLeft ||
                                  _activeVehicle.sprayer.sprayingReady ||
                                  _activeVehicle.sprayer.sprayingPumpError ||
                                  _activeVehicle.sprayer.sprayingNozzlesError

    Row {
        id:             sprayerIndicatorRow
        anchors.top:    parent.top
        anchors.bottom: parent.bottom

        Repeater {
            model: _activeVehicle ? _activeVehicle.batteries : 0

            Loader {
                anchors.top:        parent.top
                anchors.bottom:     parent.bottom
                sourceComponent:    sprayerVisual

                property var battery: object
            }
        }
    }
    MouseArea {
        anchors.fill:   parent
        onClicked: {
            mainWindow.showIndicatorPopup(_root, sprayerPopup)
        }
    }

    Component {
        id: sprayerVisual

        Row {
            anchors.top:    parent.top
            anchors.bottom: parent.bottom
            spacing:        ScreenTools.defaultFontPixelWidth

            QGCColoredImage {
                id:                 tankFillPermanentContour
                anchors.top:        parent.top
                anchors.bottom:     parent.bottom
                width:              height
                sourceSize.width:   width
                source:             "/soleonIcons/tank_outline.svg"
                fillMode:           Image.PreserveAspectFit
                color:              qgcPal.text

                QGCColoredImage {
                    id:                 tankFillContour
                    anchors.fill:       parent
                    width:              height
                    sourceSize.width:   width
                    source:             "/soleonIcons/tank_fill_100.svg"
                    fillMode:           Image.PreserveAspectFit
                    color:              "red"
                    visible:            false
                    z:                  tankFillPermanentContour.z - 1
                }

                Rectangle {
                    id:                 tankFillInside
                    height:             tankFillContour.height
                    width:              tankFillContour.width * _fillPercentage * 0.01
                    anchors.left:       tankFillContour.left
                    color:              "green"
                    visible:            false
                    z:                  tankFillPermanentContour.z - 1
                }

                OpacityMask {
                    anchors.fill:       tankFillInside
                    source:             tankFillInside
                    maskSource:         tankFillContour
                    z:                  tankFillPermanentContour.z - 1
                }
            }

            QGCLabel {
                text:                   _fillPercentage.toFixed(0) + "%"
                font.pointSize:         ScreenTools.mediumFontPointSize
                color:                  qgcPal.text
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Component {
        id: sprayerPopup

        Rectangle {
            width:          mainLayout.width   + mainLayout.anchors.margins * 2
            height:         mainLayout.height  + mainLayout.anchors.margins * 2
            radius:         ScreenTools.defaultFontPixelHeight / 2
            color:          qgcPal.window
            border.color:   qgcPal.text

            ColumnLayout {
                id:                 mainLayout
                anchors.margins:    ScreenTools.defaultFontPixelWidth
                anchors.top:        parent.top
                anchors.right:      parent.right
                spacing:            ScreenTools.defaultFontPixelWidth

                QGCLabel {
                    Layout.alignment:   Qt.AlignCenter
                    text:               qsTr("Sprayer Info")
                    font.family:        ScreenTools.demiboldFontFamily
                }

                RowLayout {
                    spacing: ScreenTools.defaultFontPixelWidth

                    ColumnLayout {
                        QGCLabel { text: qsTr("fill Level: "); }
                        QGCLabel { text: qsTr("flow: "); }
                        QGCLabel { text: qsTr(" "); }
                        QGCLabel { text: qsTr("distance lines: "); }
                        QGCLabel { text: qsTr("speed: "); }
                    }

                    ColumnLayout {
                        QGCLabel { text: _activeVehicle.sprayer.filllevel.value.toFixed(1) + " " + _activeVehicle.sprayer.filllevel.units }
                        QGCLabel { text: _activeVehicle.sprayer.flowliter.value.toFixed(1) + " " + _activeVehicle.sprayer.flowliter.units }
                        QGCLabel { text: _activeVehicle.sprayer.flowha.value.toFixed(1) + " " + _activeVehicle.sprayer.flowha.units }
                        QGCLabel { text: _activeVehicle.sprayer.distlines.value.toFixed(1) + " " + _activeVehicle.sprayer.distlines.units }
                        QGCLabel { text: _activeVehicle.sprayer.speed.value.toFixed(1) + " " + _activeVehicle.sprayer.speed.units }
                    }
                }

                QGCLabel {
                    Layout.alignment:   Qt.AlignCenter
                    text:               qsTr("Status")
                    font.family:        ScreenTools.demiboldFontFamily
                    visible:            _statusVisible
                }
                QGCLabel {
                    Layout.alignment:   Qt.AlignCenter
                    text:               qsTr("Spraying Right")
                    visible:            _activeVehicle.sprayer.sprayingRight
                }
                QGCLabel {
                    Layout.alignment:   Qt.AlignCenter
                    text:               qsTr("Spraying Left")
                    visible:            _activeVehicle.sprayer.sprayingLeft
                }
                QGCLabel {
                    Layout.alignment:   Qt.AlignCenter
                    text:               qsTr("Spraying Ready")
                    visible:            _activeVehicle.sprayer.sprayingReady
                }
                QGCLabel {
                    Layout.alignment:   Qt.AlignCenter
                    text:               qsTr("Pump Error")
                    visible:            _activeVehicle.sprayer.sprayingPumpError
                }
                QGCLabel {
                    Layout.alignment:   Qt.AlignCenter
                    text:               qsTr("Nozzles Error")
                    visible:            _activeVehicle.sprayer.sprayingNozzlesError
                }
            }
        }
    }
}
