<?php
session_start();

$_SESSION['user_lvl'] = 0;
$_SESSION['user_id'] = -2;

$_SESSION['user_nick'] = "";

$include_file = "index.inc";
include "main.inc";


?>