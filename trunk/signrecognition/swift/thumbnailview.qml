import QtQuick 1.0

 ListView {
     width: 100; height: 100
     anchors.fill: parent

     model: SwiftModel
     delegate: Rectangle {
         height: 25
         width: 100
         color: model.modelData.color
         Text { text: name }
     }
 }