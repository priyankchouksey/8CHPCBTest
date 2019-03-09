function setPWM(pin, action) {
    currentval = parseInt($('#count_' + pin).text(), 10);
    switch (action) {
        case '0':
            currentval = 0;
            break;
        case '-':
            currentval = currentval - 10;
            break;
        case '+':
            currentval = currentval + 10;
            break;
        case '255':
            currentval = 255;
            break;
    }
    currentval = (currentval < 0 ? 0 : currentval);
    currentval = (currentval > 255 ? 255 : currentval);
    $('#count_' + pin).text(currentval);
    var data = {};
    data = "{'command': 'set', 'pin': '" + pin + "', 'datatype': 'bool', 'value':'" + currentval + "'}";
    wsSend(data);
}

function param(name) {
    return (location.search.split(name + '=')[1] || '').split('&')[0];
}

function wsConnect() {
    if ('WebSocket' in window) {

        // accept ?target=10.0.0.123 to make a WS connection to another device
        if (target = param('target')) {
            // 
        } else {
            target = document.location.host;
        }
        ws = new WebSocket('ws://' + target + '/ws');
        ws.binaryType = 'arraybuffer';
        ws.onmessage = function(event) {
            //Handle responses here
        };
    }
}

function wsSend(data) {
    ws.send(data);
}