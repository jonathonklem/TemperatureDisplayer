<?php
	// code from http://blog.worldweatheronline.com/2015/02/05/getting-started-world-weather-online-api-php/
	$queryUrl = "api.worldweatheronline.com/free/v2/weather.ashx?q=Evansville&format=json&num_of_days=1&key=ba7c662279e32364d70915bf44392";

	/**
	 * CURLOPT_RETURNTRANSFER - Return the response as a string instead of outputting it to the screen
	 * CURLOPT_CONNECTTIMEOUT - Number of seconds to spend attempting to connect
	 * CURLOPT_TIMEOUT - Number of seconds to allow cURL to execute
	 * CURLOPT_USERAGENT - Useragent string to use for request
	 * CURLOPT_URL - URL to send request to
	 * CURLOPT_POST - Send request as POST
	 * CURLOPT_POSTFIELDS - Array of data to POST in request
	 */
	$options = array(
	    CURLOPT_RETURNTRANSFER => true,
	    CURLOPT_HTTPHEADER => array('Content-type: application/json'),
	    CURLOPT_URL => $queryUrl
	);
	
	// Setting curl options
	$curl = curl_init();
	curl_setopt_array( $curl, $options );
	
	if (!$result = curl_exec($curl)){
	    die('Error: "' . curl_error($curl) . '" - Code: ' . curl_errno($curl));
	}
	curl_close($curl);
	
	$response = json_decode($result, true);
	
	// only print the necessary information	
	echo "|Temp: " . $response['data']['current_condition'][0]['temp_F'] . "F|Humidity: " . $response['data']['current_condition'][0]['humidity']  . "|Descr: " . $response['data']['current_condition'][0]['weatherDesc'][0]['value'];
