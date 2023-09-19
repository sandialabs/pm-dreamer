<?php
// First thing to do is to start the session
session_start();

if (!isset($_POST['username']) || (!isset($_POST['password']))) {
  print ("You tried to access a form directly and that's not allowed! ;)<br>");
  $include_file = "index.inc";
  include "main.inc";
} else {
  include "dbconnect.inc";

  // Create the SQL request to find the user ID and level
  $query = "SELECT FID, FLevel, FNickName FROM tusers WHERE FNickName='" . $_POST['username'] . "' AND FPassword='" . md5($_POST['password']) . "'";
  // DEBUG: echo $query;

  // Check if the user is OK
  $result = mysql_query($query)
    or die("Query failed : " . mysql_error());

  // Get the result we found
  if ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    $_SESSION['user_id'] = $line['FID'];
    $_SESSION['user_lvl'] = $line['FLevel'];
    $_SESSION['user_nick'] = $line['FNickName'];

    $query = "UPDATE tusers SET FDateLastLogin=NOW() WHERE FID = " . $line['FID'];
    mysql_query($query)
      or die("Query failed : " . mysql_error());
		
    /* Free resultset */
    mysql_free_result($result);
	
    /* Closing connection */
    mysql_close($link);

    header("Location: index.php");
    exit;
  } else {
    /* Free resultset */
    mysql_free_result($result);

    /* Closing connection */
    mysql_close($link);

    $include_file="login.inc";
    include "main.inc";
  }
}
	

?>
