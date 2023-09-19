<?php
session_start();

if (!isset($_SESSION['user_lvl'])) {
  $_SESSION['user_lvl'] = 0;
  $_SESSION['user_id'] = -1;
  $_SESSION['user_nick'] = "";
}

$userlvl = $_SESSION['user_lvl'];
$include_file = "index.inc";

// check if we support that action since we don't want the user to send us bad or dangerous command.
if (isset($_GET['request'])) {
  $action = $_GET['request'];

  if ($action == "login") {
    $include_file = "login.inc";
  } 
  else if ($action == "board") {
    $include_file = "board.inc";
  } 
  else if ($action == "help") {
    $include_file = "help.inc";
  } 
  else if ($action == "readboard") {
    $include_file = "readboard.inc";
  } 
  else if ($action == "readthread") {
    $include_file = "readthread.inc";
  } 
  else if ($action == "register") {
    $include_file = "register.inc";
  } 
  else if ($action == "filemanager" && $userlvl >= 2) {
    $include_file = "filemanager.inc";
  } 
  else if ($action == "userlist" && $userlvl >= 3) {
    $include_file = "userlist.inc";
  } 
  else if ($action == "createboard" && $userlvl >= 10) {
    $include_file = "createboard.inc";
  } 
  else if ($action == "editprofile" && $userlvl >= 1) {
    $include_file = "profile.inc";
  } 
  else {
    $include_file = "badlink.inc";
  }

}

if (file_exists("dbparam.inc")) {
  include "main.inc";
} else {
  include "initdb.inc";
}                        

?>