<?php

$_SESSION['FIRST-TIME-INIT'] = true;

?>

This is the first time you are accessing this web page. Please fill in the blanks (and replace the wrong information) so the database will be created and configured properly. Missing directories should be created automatically when required.<br>
<br>
<center>
<form action="initdb.php" method=post>
<table border=0>
<tr>
<td>
<table border=1 cellspacing=1>
<tr>
<th colspan=2>MySQL Parameters</th>
</tr>

<tr>
<td>MySQL Database Name</td>
<td><input type=text name=mysqldatabase value="visualizer"></td>
</tr>

<tr>
<td>MySQL Server</td>
<td><input type=text name=mysqlhost value="localhost:3306"></td>
</tr>

<tr>
<td>MySQL User</td>
<td><input type=text name=mysqluser value="root"></td>
</tr>

<tr>
<td>MySQL Password</td>
<td><input type=password name=mysqlpass></td>
</tr>
</table>
</td>
<td>
<table border=1 cellspacing=1>
<tr>
<th colspan=2>File Manager Parameters</th>
</tr>

<tr>
<td>Visualizer Binaries Installation Directory</td>
<td><input type=text name=fileprogram value="/usr/local/visualizer/bin/"></td>
</tr>

<tr>
<td>Data Storage Root</td>
<td valign=center><input type=text name=fileroot value="/usr/local/visualizer/serverdata/"></td>
</tr>

<tr>
<td>Received File Subdirectory</td>
<td><input type=text name=filereceived value="/usr/local/visualizer/serverdata/received/"></td>
</tr>

<tr>
<td>Generated File Subdirectory<br>(of the directory this script is runned from)</td>
<td valign=center><input type=text name=filegenerated value="generated/"></td>
</tr>
      
</table>
</td>
</tr>

<tr>
<td colspan=2 align=center>
<table border=1 cellspacing=1>
<tr>
<th colspan=2>Master User & Server Access</th>
</tr>

<tr>
<td>Nickname</td>
<td><input type=text name=username></td>
</tr>
<tr>
<td>Password</td>
<td><input type=password name=password></td>
</tr>

<tr>
<td>Email</td>
<td><input type=text name=email></td>
</tr>

<tr>
<td>Realname</td>
<td><input type=text name=realname></td>
</tr>

<tr>
<td>This site <b>complete</b> URL</td>
<td><input type=text name=servername value="http://localhost/"></td>
</tr>

<tr>
<td>Allow .htaccess protection on files and direction</td>
<td align=center><input type=checkbox name=htaccess checked</td>
</tr>

</table>
</td>
</tr>

<tr>
<td colspan=2 align=center>
<input type=submit name=action value="Submit">
</td>
</tr>
</table>
</form>
</center>
