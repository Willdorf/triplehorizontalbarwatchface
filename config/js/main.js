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

	localStorage.willdorftriplehorizontalbartopBarColor = options.topBarColor;
	localStorage.willdorftriplehorizontalbarmiddleBarColor = options.middleBarColor;
	localStorage.willdorftriplehorizontalbarbottomBarColor = options.bottomBarColor;
	localStorage.willdorftriplehorizontalbarbackgroundColor = options.backgroundColor;
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
		$topBarColorPicker[0].value = localStorage.willdorftriplehorizontalbartopBarColor;
		$middleBarColorPicker[0].value = localStorage.willdorftriplehorizontalbarmiddleBarColor;
		$bottomBarColorPicker[0].value = localStorage.willdorftriplehorizontalbarbottomBarColor;
		$backgroundColorPicker[0].value = localStorage.willdorftriplehorizontalbarbackgroundColor;
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
