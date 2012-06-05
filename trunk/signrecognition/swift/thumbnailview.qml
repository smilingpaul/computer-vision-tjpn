import QtQuick 1.0

Flickable {
	id: swiftFlickable

	anchors.fill: parent

	Image { 
		id: thumbnail
		source: swiftFlickable.swiftView.model.thumbnail();
		anchors.fill: parent
	}

	ListView {
		id: swiftView
		anchors.fill: parent
		model: swiftModel
		delegate: swiftDelegate
	}
}