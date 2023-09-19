<?php
include "dbparam.inc";

// Connect to the database
$link = mysql_connect($dbhost, $dbuser, $dbpass)
	or die("Can't connect to mysql : " . mysql_error());

// Select the database associated with the project
mysql_select_db($database)
	or die("Can't find Beagle Visualizer's database!");
?>