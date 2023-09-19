<?php
$d = dir($root.$dir);
// echo "Handle: " . $d->handle . "<br />\n";
// echo "Path: " . $d->path . "<br />\n";

print ("<table border=0 cellspacing=0 cellpadding=2>");
print ("<th colspan=4 width=380>" . $message . "</th>");

if ($dir != "/") {
  print ("<tr><td width=20>&nbsp;</td>");
  print ("<td width=20><a href=\"filemanager.php?action=updir$type\" title=\"Back to the previous directory\"><img src=\"./img/dir.gif\" border=0></a></td>");
  print("<td width=340 colspan=2>UP</td></tr>");	
}
while (false !== ($entry = $d->read())) {
  if ($entry != "." && $entry != "..") {
    print("<tr><td width=20><input type=checkbox name=" . $checkname . "[] value=\"" . $entry . "\"></td>");
		
    if (is_dir($d->path . $entry)) {
      print ("<td width=20><a href=\"filemanager.php?action=chdir$type&dir=" . $entry . "\" title=\"Change to this directory\"><img src=\"./img/dir.gif\" border=0></a></td>");
      print("<td width=340 colspan=2>" . $entry . "</td>");
    } else {
      if ($type == "rec") {
	print ("<td width=20><a href=\"filemanager.php?action=download" . $checkname . "&file=" . $entry . "\" title=\"Download this file\"><img src=\"./img/file.gif\" border=0></td>");
      } else if ($type == "gen") {
	print ("<td width=20><a href=\"$root$dir" . $entry . "\" title=\"Download this file\" target=_new><img src=\"./img/file.gif\" border=0></td>");				
      } else {
	print ("Invalid type in showdirectory!");
	exit;
      }

      if ($entry == "index.html") $index = true;

      print("<td width=300>" . $entry . "</td>");
      print("<td width=40 align=right>" . filesize($d->path . $entry) . "</tr>");   
    }

		
  }
}
$d->close();

print("</table>");

?>
