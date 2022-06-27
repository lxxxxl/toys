import QtQuick 1.0

Rectangle {
    id: screen
    width: 240
    height: 320

    function getKeys(){
        var doc = new XMLHttpRequest();
        doc.onreadystatechange = function() {
            if (doc.readyState == XMLHttpRequest.DONE) {
                var re = /"url":"(.*?)"/ig;
                var matches = doc.responseText.match(re);
                //ar match = re.exec(doc.responseText);
                if (matches == null) {
                    screen.state = "ERROR";
                    console.log(doc.responseText);
                    return;
                }
                var match = matches[Math.floor((Math.random()*matches.length))].replace(/\"/g,"").replace("url:","");
                console.log(match);
                img.source = match;
            }
        }
        var words = ["keys", "ключи", "door keys", "门钥匙", "дверной ключ", "llave", "clés", "鍵"];
        var i = Math.floor((Math.random()*50)+1);
        var word = words[Math.floor((Math.random()*words.length))];
        var uri = "http://ajax.googleapis.com/ajax/services/search/images?v=10.0&q="+word+"&start="+i;
        console.log(uri);
        doc.open("GET", uri);
        doc.send();
        screen.state = "LOADING";
    }

    Column{
        Rectangle{
            id: header
            width: screen.width
            height: screen.height / 10
            color: "transparent"

            Text{
                id:txt
                anchors.centerIn: parent
                text: "Tap screen to find your keys."
            }

            Image {
                id: close
                source: "close.png"
                width: screen.width / 12
                x: parent.width - width - 5
                y: screen.height / 11 - height
                fillMode: Image.PreserveAspectFit
                MouseArea{
                    anchors.fill: parent
                    onClicked: Qt.quit();
                }
            }
        }
        Rectangle{
            id: content
            width: screen.width
            height: screen.height - header.height

            AnimatedImage {
                id: loadingimg
                visible: false
                source: "loading.gif"
                anchors.centerIn: content
                fillMode: Image.PreserveAspectFit
                x: 0
                y: screen.height - height
            }

            Image {
                id: img
                visible: true
                source: "key.png"
                anchors.fill: content
                fillMode: Image.PreserveAspectFit
                x: 0
                y: screen.height - height
                onStatusChanged: {
                    if (img.status == Image.Error) {screen.getKeys();}
                    else if (img.status == Image.Ready) screen.state = "DONE"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {screen.getKeys()}
                }
            }
        }
    }

    states: [
        State{
            name: "LOADING"
            PropertyChanges {target: loadingimg; visible: true}
            PropertyChanges {target: img; visible: false}
			PropertyChanges {target: txt; text: "Searching for your keys..."}
        },
        State{
            name: "DONE"
            PropertyChanges {target: loadingimg; visible: false}
            PropertyChanges {target: img; visible: true}
            PropertyChanges {target: txt; text: "Here they are! Not your keys?\nTap again to find another."}
        },
        State{
            name: "ERROR"
            PropertyChanges {target: loadingimg; visible: false}
			PropertyChanges {target: txt; text: "Your keys not found...\nSorry."}
        }
    ]
    Component.onCompleted: txt.text = "Tap screen to find your keys."
}
