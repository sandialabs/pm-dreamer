<?php

$fail = false;

// check if the params are ok
if (!isset($_GET['threadid'])) {
  print ("You tried to access a form directly and that's not allowed! ;)<br>");
  $fail = true;
} else {
  // check if he has enough security to do so
  $query = "SELECT * FROM tthreads WHERE FID='" . $_GET['threadid'] . "'";
	
  $result = mysql_query($query)
    or die("Query failed : " . mysql_error());

  $lineThread = mysql_fetch_array($result, MYSQL_ASSOC);
	
  if ($lineThread) {
    $query = "SELECT * FROM tboards WHERE FID='" . $lineThread['FBoardID'] . "'";

    $result2 = mysql_query($query)
      or die("Query failed : " . mysql_error());
		
    $line = mysql_fetch_array($result2, MYSQL_ASSOC);
		
    if ($line['FLvlRead'] > $_SESSION['user_lvl']) {
      print ("Can't read this thread : your access level is too low!<br>");
      $fail = true;		
    } else {
      print ("<h3>" . $line['FSubject'] . " Message Board</h3>");
      $lvlwrite = $line['FLvlWrite'];
      $boardid = $line['FID'];
    }
		
    /* Free resultset */
    mysql_free_result($result2);		
  } else {
    print ("Can't find the requested thread !<br>");
    $fail = true;
  }
	

  if (!$fail) {
    // Then we can display the requested message board.
    $query = "SELECT * FROM tmessages WHERE FThreadID='" . $_GET['threadid'] . "' ORDER BY FDate ASC";
				
    $result2 = mysql_query($query)
      or die("Query failed : " . mysql_error());
		
    print("<table border=1 cellspacing=0><th colspan=2>" . $lineThread['FTitle'] . "</th></tr>");
		
    while($line = mysql_fetch_array($result2, MYSQL_ASSOC)) {
      print ("<tr bgcolor=lightgray>");
      print ("<td>" . $line['FNick'] . "</td>");
      print ("<td align=right width=150>" . $line['FDate'] . "</td>");
      print ("</tr><tr>");
      print ("<td colspan=2>" . $line['FContent'] . "</td>"); // <pre> </pre>
      print ("</tr>");
    }
    print ("</table>");
	
    if ($_SESSION['user_lvl'] >= $lvlwrite) {
  ?>

 <br>
    <form action=replythread.php method=post>
    <input type=hidden name=threadid value=<?php echo $_GET['threadid']; ?>>
    <table border=1 cellspacing=0>
       <th colspan=2>Reply to this thread</th>
       <tr>
       <td><textarea name=content rows=7 cols=80 wrap=virtual></textarea></td>
       </tr>
       <tr>
       <td align=right><input type=submit name=action value=Post></td>
       </tr>
       </table>	
       </form>
       <a href="./index.php?request=readboard&boardid=<?php echo $boardid; ?>">Back to the message board</a>
       <?php
       }
		
    /* Free resultset */
    mysql_free_result($result2);
  }
	
  /* Free resultset */
  mysql_free_result($result);

}


if ($fail) {
  include "board.inc";
}

?>
