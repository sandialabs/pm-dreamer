<?php
include "dbconnect.inc";
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>BEAGLE Visualizer Web Interface</title>
</head>
<body style="background-color: #FFFFFF" text="#000000" link="#4B66D0" vlink="#F09519" alink="#F09519">
<center>
<table border=1 cellspacing=0 cellpadding=5>
<tr>
<td width=190 valign=top>
<!-- BEGIN INCLUDE LEFTCOL.INC -->
<?php
include "leftcol.inc"
?>     
<!-- END INCLUDE LEFTCOL.INC -->
</td>
<td width=810 valign=top>
<!-- BEGIN INCLUDE <?php echo strtoupper($include_file) ?> -->
<?php
include $include_file;
?>
<!-- END INCLUDE <?php echo strtoupper($include_file) ?> -->
</td>
</tr>
</table>
</center>
<br>
</body>
</html>

<?php
/* Closing connection */
mysql_close($link);
?>