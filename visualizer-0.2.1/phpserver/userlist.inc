<?php

print ("<h3>User List</h3><center>");

$query = "SELECT * FROM tusers ORDER BY FLevel DESC, FNickName ASC";
		
$result = mysql_query($query)
     or die("Query failed : " . mysql_error());

// $color = array(0 => "white", 1 => "lightgreen", 2 => "green", 3 => "darkgreen", 4 => "lightblue", 5 => "blue", 6 => "lightred", 7 => "red", 8 => "darkred", 9 => "lightgray", 10 => "gray");
$color = array(0 => "oldlace", 1 => "linen", 2 => "floralwhite", 3 => "ivory", 4 => "lightyellow", 5 => "cornsilk", 6 => "lemonchiffon", 7 => "lightgoldenrodyellow", 8 => "honeydew", 9 => "mintcream", 10 => "aliceblue");

print("<table border=1 cellspacing=0>\n");
print("<tr align=center><td>Lvl</td><td>Nickname</td><td>Realname</td><td>Email</td><td>Date joined</td><td>Last Login</td></tr>\n");

while($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
  print ("<tr bgcolor=" . $color[$line['FLevel']] . ">");
  print ("<td>" . $line['FLevel'] . "</td>");

  if ($_SESSION['user_lvl'] == 10) {
    print ("<td><a href=index.php?request=editprofile&id=${line['FID']}>" . $line['FNickName'] . "</a></td>");
  } else {
    print ("<td>" . $line['FNickName'] . "</td>");
  }

  print ("<td>" . $line['FRealName'] . "</td>");
  print ("<td>" . $line['FEmail'] . "</td>");
  print ("<td>" . substr($line['FDateJoined'],0,10) . "</td>");
  print ("<td>" . substr($line['FDateLastLogin'],0,10) . "</td>");		
  print ("</tr>\n");
}
print ("</table></center><br>");

/* Free resultset */
mysql_free_result($result);

?>
