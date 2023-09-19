<script src="message.js"></script>

<?php

include "verifydirectory.inc";

if (isset($_GET['action']) && $_GET['action'] == "removepopup") {
  echo ("<script>removeText();</script>\n");
}

print("<h3>File Manager</h3>");

if (isset($_GET['echo'])) {
  print ($_GET['echo'] . "<br>");
}

print("<center><form action=filemanager.php method=post><table border=1 cellspacing=0><tr valign=top><td width=380>");

if (!isset($_SESSION['user_recdir'])) {
  $_SESSION['user_recdir'] = "/";
}

if (!isset($_SESSION['user_gendir'])) {
  $_SESSION['user_gendir'] = "/";
}


$root = $fmreceivedfile.$_SESSION['user_id'];
$dir = $_SESSION['user_recdir'];
$type = "rec";
$checkname = "recfile";

$message = $_SESSION['user_nick'] . " uploaded files [Dir=" . $dir . "]";
include "showdirectory.inc";

print("</td><td width=380>");

$root = $fmgeneratedfile.$_SESSION['user_id'];
$dir = $_SESSION['user_gendir'];
$type = "gen";
$checkname = "genfile";

$message = $_SESSION['user_nick'] . " generated files [Dir=" . $dir . "]";

$index = false;
include "showdirectory.inc";


print("</td></tr></table>");

print("<br><table border=0 cellspacing=0>");

print("<tr><td><input type=\"submit\" onclick=popText() name=action value=\"Generate Report\">");
print("</td><td>");

if ($index === true) {
  print ("<input type=button onclick=\"window.open('./${root}${dir}index.html');\" value=\"View Report\">");
  print ("<input type=submit name=actiongen value=\"Download Report\">");
} else {
  print ("&nbsp;");
}

print("</td></tr>");

// print("<tr><td><input type=submit name=action value=\"Run Selected\">&nbsp;<input type=text name=filename></td></tr>");
print("<tr><td width=380><input type=submit name=actionrec value=\"Delete Selected\"></td><td width=380><input type=submit name=actiongen value=\"Delete Selected\"></td></tr>\n");

?>

<tr>
<td >
<input type=submit value="Remove Directory" name=actionrec>
</td>
<td >
<input type=submit value="Remove Directory" name=actiongen>
</td>
</tr>
</form>
<tr>
<td >
<form action=filemanager.php method=post>
<input type=submit value="Create Directory" name=actionrec>&nbsp;<input type=text name=dirrec>
</form>
</td>
<td >
<form action=filemanager.php method=post>
<input type=submit value="Create Directory" name=actiongen>&nbsp;<input type=text name=dirgen>
</form>
</td>
</tr>


<tr><td colspan=2>&nbsp;</td></tr>
<tr>
<td colspan=2>
<form enctype="multipart/form-data" action="filemanager.php?type=upload" method="post">
<input type="hidden" name="MAX_FILE_SIZE" value="2000000" />
Send this file: <input name="userfile" type="file" />
<input type="submit" value="Send File" />
</form>
</td>
</tr>

</table>



