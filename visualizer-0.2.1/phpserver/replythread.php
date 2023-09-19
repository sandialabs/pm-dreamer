<?php
// First thing to do is to start the session
session_start();

if (!isset($_POST['threadid']) || (!isset($_POST['content']))) {
  print ("You tried to access a form directly and that's not allowed! ;)<br>");
	
  $include_file = "board.inc";
  include "main.inc";
} else {
  include "dbconnect.inc";
  $query = "SELECT tboards.FLvlWrite FROM tboards, tthreads WHERE tboards.FID=tthreads.FBoardID AND tthreads.FID='" . $_POST['threadid'] . "'";

  $result = mysql_query($query)
    or die("Query failed : " . mysql_error());

  $line = mysql_fetch_array($result, MYSQL_ASSOC);
	
  if (!$line) {
    print ("Requested board (from thread) doesn't exist!<BR>");
		
    /* Free resultset */
    mysql_free_result($result);
		
    /* Closing connection */
    mysql_close($link);	
		
    $include_file = "board.inc";
    include "main.inc";
  } else if (!isset($_SESSION['user_lvl']) || $_SESSION['user_lvl'] < $line['tboards.FLvlWrite']) {
    print ("Access level too low.<BR>");

    /* Free resultset */
    mysql_free_result($result);
		
    /* Closing connection */
    mysql_close($link);	
	
    $include_file = "board.inc";
    include "main.inc";         
  } else {
    /* Free resultset */
    mysql_free_result($result);

    $query = "INSERT INTO tmessages( FThreadID , FContent , FNick, FDate )" . 
      " VALUES ( '". $_POST['threadid'] . "', '" . str_replace("\n", "<br>",str_replace("\r", "", str_replace("<", "&lt;", $_POST['content']))) . "', '" . $_SESSION['user_nick'] . " ', NOW())";
				 
    mysql_query($query)
      or die("Query failed : " . mysql_error());
			
    $query = "UPDATE tthreads SET FNickLastMsg='" . $_SESSION['user_nick'] . "', FDateLastMsg=NOW(), FReplies=FReplies+1 WHERE FID='" . $_POST['threadid'] . "'";

    mysql_query($query)
      or die("Query failed : " . mysql_error());

    /* Closing connection */
    mysql_close($link);	

    header("Location: index.php?request=readthread&threadid=". $_POST['threadid']);
    exit;   
  }    
}

?>