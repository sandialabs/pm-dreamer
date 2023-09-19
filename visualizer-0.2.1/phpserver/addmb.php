<?php
// First thing to do is to start the session
session_start();
if (!isset($_SESSION['user_lvl']) || $_SESSION['user_lvl'] < 10) {
  print ("Access level too low.<BR>");

  $include_file = "index.inc";
  include "main.inc";         
} else if (!isset($_POST['subject']) || (!isset($_POST['read'])) || (!isset($_POST['write'])) || (!isset($_POST['admin']))) {
  print ("You tried to access a form directly and that's not allowed! ;)<br>");
	
  $include_file = "board.inc";
  include "main.inc";
} else {
  include "dbconnect.inc";

  $query = "INSERT INTO tboards( FSubject , FLvlRead , FLvlWrite, FLvlAdmin )" . " VALUES ( '". $_POST['subject'] . "', '" . $_POST['read'] . "', '" . $_POST['write'] . " ', '". $_POST['admin']."')";

  // Check if the user is OK
  $result = mysql_query($query)
    or die("Query failed : " . mysql_error());

  header("Location: index.php?request=board");
  exit;       
}
	

?>
