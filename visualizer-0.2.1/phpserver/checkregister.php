<?php
// First thing to do is to start the session
session_start();

include "dbconnect.inc";

// Create the SQL request to find the user ID and level
$query = "SELECT FID FROM tusers WHERE FNickName='" . $_POST['username'] . "' OR FEmail='" . $_POST['email'] . "'";
// DEBUG: echo $query;

// Check if the user is OK
$result = mysql_query($query)
     or die("Query failed : " . mysql_error());

     // Get the result we found
     if ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
       print ("Username or email already used.<br>\n");

       /* Free resultset */
       mysql_free_result($result);
	
       /* Closing connection */
       mysql_close($link);

       include "register.php";        
     } else {
       if ($_POST['password1'] == $_POST['password2']) {
	 $query = "INSERT INTO tusers ( FLevel , FNickName , FPassword , FRealName , FEmail , FDateJoined , FDateLastLogin )" . 
	   " VALUES ( 2, '".$_POST['username']."', '".md5($_POST['password1'])."', '".$_POST['realname']."', '". 
	   $_POST['email']."', NOW( ) , NOW( ))";

	 mysql_query($query)
	   or die("Query failed : " . mysql_error());

	 // print("User created successfully.<br>\n");

	 /* Free resultset */
	 mysql_free_result($result);

	 /* Closing connection */
	 mysql_close($link);
		
	 // load main page
	 header("Location: index.php");
	 exit;
       } else {
	 print("Passwords don't match.<br>\n");

	 /* Free resultset */
	 mysql_free_result($result);
	
	 /* Closing connection */
	 mysql_close($link);

	 include "register.php";        
       }
     }
	

?>
