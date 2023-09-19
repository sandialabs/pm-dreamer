<?php
$query = "SELECT * FROM tboards WHERE FLvlRead <='" . $_SESSION['user_lvl'] . "'";
// DEBUG: echo $query;


$result = mysql_query($query)
  or die("Query failed : " . mysql_error());

// Get the result we found
if ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
  print("<h3>Message Board</h3>");
  print("<center><table border=1 cellspacing=0>");
  print("<tr><td>Subject</td><td colspan=2>Last Reply</td><td>Read</td><td>Write</td><td>Admin</td></tr>");
  
  do {
	  print("<tr>");
	  
	  $query = "SELECT * FROM tthreads WHERE FBoardID=" . $line['FID'] . " ORDER BY FDateLastMsg DESC LIMIT 0,1";
	  $result2 = mysql_query($query)
	    or die ("Query failed : " . mysql_error());
		
	  $line2 = mysql_fetch_array($result2, MYSQL_ASSOC);

	  print("<td><a href=\"./index.php?request=readboard&boardid=" . $line['FID'] . "\">".$line['FSubject']."</a></td>");
	  if ($line2) {
	    print ("<td>" . $line2['FNickLastMsg'] . "</td>");
	    print ("<td>" . $line2['FDateLastMsg'] . "</td>");			
	  } else {
	    print ("<td colspan=2>&nbsp</td>");
	  }
    
	  print("<td align=right>".$line['FLvlRead']."</td>");
	  print("<td align=right>".$line['FLvlWrite']."</td>");
	  print("<td align=right>".$line['FLvlAdmin']."</td>");

	  print("</tr>");
  } while ($line = mysql_fetch_array($result, MYSQL_ASSOC));
  
  print("</table></center><br>");
} else {
  print("No message board found.<br>");
}

/* Free resultset */
mysql_free_result($result);

if (isset($_SESSION['user_lvl']) && $_SESSION['user_lvl']>=10){
  print("<a href=\"./index.php?request=createboard\">Create new message board</a>");

}

?>
