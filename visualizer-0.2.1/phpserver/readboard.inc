<?php

$fail = false;

// check if the params are ok
if (!isset($_GET['boardid'])) {
  print ("You tried to access a form directly and that's not allowed! ;)<br>");
  $fail = true;
} else {
  // check if he has enough security to do so
  $query = "SELECT * FROM tboards WHERE FID='" . $_GET['boardid'] . "'";

  $result = mysql_query($query)
    or die("Query failed : " . mysql_error());

  $line = mysql_fetch_array($result, MYSQL_ASSOC);
	
  if ($line) {
    if ($line['FLvlRead'] > $_SESSION['user_lvl']) {
      print ("Can't read this message board : your access level is too low!<br>");
      $fail = true;		
    } else {
      print ("<h3>" . $line['FSubject'] . " Message Board</h3>");
      $lvlwrite = $line['FLvlWrite'];
    }
  } else {
    print ("Can't find the requested message board!<br>");
    $fail = true;
  }
	
  /* Free resultset */
  mysql_free_result($result);
	
  if (!$fail) {
    // Then we can display the requested message board.
    $query = "SELECT * FROM tthreads WHERE FBoardID='" . $_GET['boardid'] . "' ORDER BY FDateLastMsg DESC";
		
    $result = mysql_query($query)
      or die("Query failed : " . mysql_error());
		
    $line = mysql_fetch_array($result, MYSQL_ASSOC);
    if ($line) {
      print ("<center><table border=1 cellspacing=0><tr><td>Title</td><td>Starter</td><td colspan=2>Last Write</td><td>Replies</td></tr>");
      do {
	print ("<tr>");
	print ("<td><a href=\"./index.php?request=readthread&threadid=" . $line['FID'] . "\">" . $line['FTitle'] . "</a></td>");
	print ("<td>" . $line['FNickStarted'] . "</td>");
	print ("<td>" . $line['FNickLastMsg'] . "</td>");				
	print ("<td>" . $line['FDateLastMsg'] . "</td>");
	print ("<td>" . $line['FReplies'] . "</td>");
	print ("</tr>");
			
      } while ($line = mysql_fetch_array($result, MYSQL_ASSOC));		
      print ("</table></center>");
    } else {
      print ("No thread found.<br>");		
    }
		
    /* Free resultset */
    mysql_free_result($result);		
	
    if ($_SESSION['user_lvl'] >= $lvlwrite) {
  ?>
 <br>
    <center>
    <form action=addthread.php method=post>
    <input type=hidden name=boardid value=<?php echo $_GET['boardid']; ?>>
    <table border=1 cellspacing=0>
       <th colspan=2>Start a new thread</th>
       <tr>
       <td>Title</td>
       <td><input type=text name=title size=60></td>
       </tr>
       <tr>
       <td>Content</td>
       <td><textarea name=content rows=7 cols=80 wrap=virtual></textarea></td>
       </tr>
       <tr>
       <td colspan=2 align=right><input type=submit name=action value=Post></td>
       </tr>
       </table>	
       </form>
       </center>
       <?php
       }
	
  }
}


if ($fail) {
  include "board.inc";
}

?>
