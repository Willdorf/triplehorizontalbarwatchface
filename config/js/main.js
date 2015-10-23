//gets run on configuration page
//note we're using zepto not jquery
//use $ prefix for zepto dom elements
//

(function() {
	loadOptions();
	submitHandler();
})();

function submitHandler() {
	var $submitButton = $('#submitButton');
	$submitButton.on('click', function() {
		console.log('Submit');

		var return_to = getQueryParam('return_to', 'pebblejs://close#');
		document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
	});
}

var $submitButton = $('#submitButton');

$submitButton.on('click', function() {
	console.log('Submit');

	var return_to = getQueryParam('return_to', 'pebblejs://close#');
	document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
});

function getAndStoreConfigData() {
	var $topBarColorPicker = $('#topBarColorPicker');
	var $middleBarColorPicker = $('#middleBarColorPicker');
	var $bottomBarColorPicker = $('#bottomBarColorPicker');
	var $backgroundColorPicker = $('#backgroundColorPicker');
	//var $timeFormatCheckbox = $('#timeFormatCheckbox');

	var options = {
		topBarColor : $topBarColorPicker.val(),
		middleBarColor: $middleBarColorPicker.val(),
		bottomBarColor: $bottomBarColorPicker.val(),
		backgroundColor : $backgroundColorPicker.val(),
	//	twentyFourHourFormat : $timeFormatCheckbox[0].checked
	};

	localStorage.topBarColor = options.topBarColor;
	localStorage.middleBarColor = options.middleBarColor;
	localStorage.bottomBarColor = options.bottomBarColor;
	localStorage.backgroundColor = options.backgroundColor;
	//localStorage.twentyFourHourFormat = options.twentyFourHourFormat;

	console.log('Got Options: ' + JSON.stringify(options));
	return options;
}

function loadOptions() {
	var $topBarColorPicker = $('#topBarColorPicker');
	var $middleBarColorPicker = $('#middleBarColorPicker');
	var $bottomBarColorPicker = $('#bottomBarColorPicker');
	var $backgroundColorPicker = $('#backgroundColorPicker');
	//var $timeFormatCheckbox = $('#timeFormatCheckbox');

	if (localStorage.topBarColor) {
		$topBarColorPicker[0].value = localStorage.topBarColor;
		$middleBarColorPicker[0].value = localStorage.middleBarColor;
		$bottomBarColorPicker[0].value = localStorage.bottomBarColor;
		$backgroundColorPicker[0].value = localStorage.backgroundColor;
		//$timeFormatCheckbox[0].checked = localStorage.twentyFourHourFormat === 'true';
	}
}

function getQueryParam(variable, defaultValue) {
	var query = location.search.substring(1);
	var vars = query.split('&');
	for (var i = 0; i < vars.length; i++) {
		var pair = vars[i].split('=');
		if (pair[0] === variable) {
			return decodeURIComponent(pair[1]);
		}
	}
	return defaultValue || false;
}
