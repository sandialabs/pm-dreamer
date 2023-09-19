<?php

if($_SESSION['user_lvl'] == 0) {
  print ("<a href=\"./index.php?request=login\">Login</a><br>");
  print ("<a href=\"./index.php?request=register\">Create a new user</a><br>");
} else {
  print ("<a href=\"./logout.php\">Logout</a> (<a href=\"./index.php?request=editprofile&id=${_SESSION['user_id']}\">${_SESSION['user_nick']}</a>)<br>");
}

print ("<br><a href=\"./index.php\">Main</a><br>");
print ("<a href=\"./index.php?request=board\">Message board</a><br>");

if($_SESSION['user_lvl'] > 2) {
  print ("<a href=\"./index.php?request=userlist\">User list</a><br>");
}

if ($_SESSION['user_lvl'] >= 2) {
  print ("<a href=\"./index.php?request=filemanager\">File manager</a><br>");
}

print ("<br><a href=\"./index.php?request=help\">Help</a><br>");
?>

