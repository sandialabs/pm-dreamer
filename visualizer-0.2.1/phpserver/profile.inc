<?php

print ("<h3>Profile</h3><center>");

if (isset($_GET['id'])) {
  $query = "";
  $mine = true;
  if ($_GET['id'] == $_SESSION['user_id']) {
    $query = "SELECT * FROM tusers WHERE FID='${_GET['id']}'";
  } else if ($_SESSION['user_lvl'] >= 10) { 
    $mine = false;
    $query = "SELECT * FROM tusers WHERE FID='${_GET['id']}'";
  } else {
    // Unable..
    print("Unable to see that user profile!");
  }

  if ($query != "") {
    $result = mysql_query($query)
      or die("Query failed : " . mysql_error());

    if ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
      print("<form method=post action=checkprofile.php><input type=hidden name=FID value=\"${_GET['id']}\"><table border=1 cellspacing=0 cellpadding=2>\n");
      print("<tr align=center><th>Property</th><th>Value</th></tr>\n");
      if ($line['FLevel'] <= 10 && $_SESSION['user_lvl'] == 10) {
	print ("<tr><td>Level</td><td><select name=FLevel>");
	for ($i = 10; $i > 0; --$i) {
	  $selected = ($line['FLevel'] == $i) ? " selected" : "";
	  print("<option value=$i$selected>$i</option>");
	}

	print("</select></td></tr>");
      } else {
	print ("<tr><td>Level</td><td>" . $line['FLevel'] . "</td></tr>");
      }

      print ("<tr><td>Nickname</td><td><input type=text name=FNickName value=\"${line['FNickName']}\"></td></tr>");
      print ("<tr><td>Realname</td><td><input type=text name=FRealName size=40 value=\"${line['FRealName']}\"></td></tr>");
      print ("<tr><td>Email</td><td><input type=text name=FEmail size=40 value=\"${line['FEmail']}\"></td></tr>");
      print ("<tr><td>New Password</td><td><input type=password name=FPassword1></td></tr>");
      print ("<tr><td>New Password</td><td><input type=password name=FPassword2></td></tr>");
      print ("<tr><td>Date joined</td><td>" . $line['FDateJoined'] . "</td></tr>");
      print ("<tr><td>Last login</td><td>" . $line['FDateLastLogin'] . "</td></tr>");	
      print ("<tr><td colspan=2 align=center><input type=submit name=action value=Modify><input type=submit name=action value=Delete></td></tr>");
      print ("</table></form></center><br>");
    } else {
      print ("Unable to find the user!");
    }

    /* Free resultset */
    mysql_free_result($result);
  }
} else {
  // who should we edit???
  print("Error: missing parameter!");
}
?>
