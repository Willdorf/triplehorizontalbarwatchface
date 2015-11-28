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

var degreeOption = 0;
function tabClickHandler(value) {
	console.log(value);
	if (value == "Celsius") {
		degreeOption = 0;
	} else if (value == "Fahrenheit") {
		degreeOption = 1;
	}
}

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
		degreeOption : degreeOption
	//	twentyFourHourFormat : $timeFormatCheckbox[0].checked
	};

	localStorage.willdorftriplehorizontalbartopBarColor = options.topBarColor;
	localStorage.willdorftriplehorizontalbarmiddleBarColor = options.middleBarColor;
	localStorage.willdorftriplehorizontalbarbottomBarColor = options.bottomBarColor;
	localStorage.willdorftriplehorizontalbarbackgroundColor = options.backgroundColor;
	localStorage.willdorftriplehorizontalbardegreeOption = options.degreeOption;
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

	if (localStorage.willdorftriplehorizontalbartopBarColor) {
		$topBarColorPicker[0].value = localStorage.willdorftriplehorizontalbartopBarColor;
		$middleBarColorPicker[0].value = localStorage.willdorftriplehorizontalbarmiddleBarColor;
		$bottomBarColorPicker[0].value = localStorage.willdorftriplehorizontalbarbottomBarColor;
		$backgroundColorPicker[0].value = localStorage.willdorftriplehorizontalbarbackgroundColor;
		//$timeFormatCheckbox[0].checked = localStorage.twentyFourHourFormat === 'true';

		//set the corresponding tab to active
		degreeOption = localStorage.willdorftriplehorizontalbardegreeOption;
		if (degreeOption == 0) {
			$('#Celsius').attr('class', 'tab-button active');
		} else {
			$('#Fahrenheit').attr('class', 'tab-button active');
		}
	} else {
		$('#Celsius').attr('class', 'tab-button active');
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
