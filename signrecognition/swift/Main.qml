import QtQuick 1.0


Rectangle {
	// width: SizeRootObjectToView
	// height: SizeRootObjectToView

     // Create a flickable to the thumbnails of the swiftModel
     Flickable {
         id: view
         anchors.fill: parent
         contentWidth: myImage1.width; contentHeight: myImage1.height
		 flickableDirection: Flickable.VerticalFlick

         Image {
             id: myImage1
             source: "test.jpg"
             asynchronous: true
         }

         // Only show the scrollbars when the view is moving.
         states: State {
             name: "ShowBars"
             when: view.movingVertically
             PropertyChanges { target: verticalScrollBar; opacity: 1 }
         }

         transitions: Transition {
             NumberAnimation { properties: "opacity"; duration: 400 }
         }
     }

     // Attach scrollbars to the right and bottom edges of the view.
     ScrollBar {
         id: verticalScrollBar
         width: 12; height: view.height-12
		 anchors.verticalCenter: view.verticalCenter	// center the scrollbar vertically
         anchors.rightMargin: 5							// distance to the image border
		 anchors.right: view.right						// vertical scrollbar should stick to the right

         opacity: 0
         orientation: Qt.Vertical
         position: view.visibleArea.yPosition
         pageSize: view.visibleArea.heightRatio
     }
 }
