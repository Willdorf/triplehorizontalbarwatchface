Pebble.addEventListener('ready', function() {
	console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
	var url = 'http://d052c64a.ngrok.io';

	console.log('Showing configuration page: ' + url);

	Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
	var configData = JSON.parse(decodeURIComponent(e.response));

	console.log('Configuration page returned: ' + JSON.stringify(configData));

	if (configData.topBarColor) {
		Pebble.sendAppMessage({
			topBarColor: parseInt(configData.topBarColor, 16),
			middleBarColor: parseInt(configData.middleBarColor, 16),
			bottomBarColor: parseInt(configData.bottomBarColor, 16),
			backgroundColor : parseInt(configData.backgroundColor, 16)
		}, function() {
			console.log("send successful!");
		}, function() {
			console.log("send failed!");
		});
	}
});
