<?php
session_start();

if (!isset($_SESSION['FIRST-TIME-INIT']) || !$_SESSION['FIRST-TIME-INIT']) {
  print ("Access denied.");	
  exit;
}

if (isset($_POST['mysqldatabase']) &&
    isset($_POST['mysqlhost']) &&
    isset($_POST['mysqluser']) &&
    isset($_POST['mysqlpass']) &&
    isset($_POST['mysqlhost']) &&
    isset($_POST['servername']) &&
    isset($_POST['fileroot']) &&
    isset($_POST['filereceived']) &&
    isset($_POST['filegenerated']) &&
    isset($_POST['fileprogram'])) {

  $dbhost = $_POST['mysqlhost'];
  $dbuser = $_POST['mysqluser'];
  $dbpass = $_POST['mysqlpass'];

  $database = $_POST['mysqldatabase'];
	
  $SDServerName = $_POST['servername'];

  $fmroot = $_POST['fileroot'];
  $fmreceivedfile = $fmroot . $_POST['filereceived'];
  $fmgeneratedfile = $_POST['filegenerated'];

  $execroot = $_POST['fileprogram'];
} else {
  print ("Missing parameter. Access denied!");
  exit;
}

// Connect to the database
$link = mysql_connect($dbhost, $dbuser, $dbpass)
     or die("Can't connect to mysql : " . mysql_error());

$file = fopen("dbparam.inc", "w")
     or die("Can't open dbparam.inc for writing!");

fwrite($file, "<?php\r\n\r\n");
fwrite($file, "\$database = \"" . $database . "\";\r\n");
fwrite($file, "\$dbhost = \"" . $dbhost . "\";\r\n");
fwrite($file, "\$dbuser = \"" . $dbuser . "\";\r\n");
fwrite($file, "\$dbpass = \"" . $dbpass . "\";\r\n");
fwrite($file, "\r\n");
fwrite($file, "\$fmroot = \"" . $fmroot . "\";\r\n");
fwrite($file, "\$fmreceivedfile = \"" . $fmreceivedfile . "\";\r\n");
fwrite($file, "\$fmgeneratedfile = \"" . $fmgeneratedfile . "\";\r\n");
fwrite($file, "\r\n");
fwrite($file, "\$execroot = \"" . $execroot . "\";\r\n");
fwrite($file, "\r\n");
fwrite($file, "\$SDServerName = \"" . $SDServerName . "\";\r\n");
fwrite($file, "\r\n");

if (isset($_POST['htaccess'])) {
  fwrite($file, "\$usehtaccess = \"ON\";\r\n");
} else {
  fwrite($file, "\$usehtaccess = \"OFF\";\r\n");
}

fwrite($file, "?>\r\n");
fclose($file);

print ("dbparam.inc created successfully.<br>");

if (mysql_select_db($database)) {
  print ("Error, DB already exist!");
  exit;
}

mysql_query("CREATE DATABASE `" . $database . "`")
     or die("Creation failed : " . mysql_error());

mysql_query("USE " . $database)
     or die("Can't select the new database!");

// Board table
$query_tboards =															
"CREATE TABLE `tboards` ( `FID` int(10) unsigned NOT NULL auto_increment, `FSubject` tinytext NOT NULL, `FLvlRead` int(11) NOT NULL default '0', `FLvlWrite` int(11) NOT NULL default '0', `FLvlAdmin` int(11) NOT NULL default '0',  PRIMARY KEY  (`FID`) ) TYPE=MyISAM;";

mysql_query($query_tboards)
     or die("TBoards creation failed : " . mysql_error());

$query_tboards_ins = "INSERT INTO `tboards` VALUES (1, 'Member area', 2, 3, 9), (2, 'Help', 0, 2, 9);";

mysql_query($query_tboards_ins)
     or die("TBoards insert failed : " . mysql_error());

// Messages table
$query_tmessages = "CREATE TABLE `tmessages` ( `FID` int(10) unsigned NOT NULL auto_increment, `FThreadID` int(11) NOT NULL default '0', `FContent` text NOT NULL, `FNick` tinytext NOT NULL, `FDate` datetime NOT NULL default '0000-00-00 00:00:00', UNIQUE KEY `FID` (`FID`) ) TYPE=MyISAM;";

mysql_query($query_tmessages)
     or die("TMessages creation failed : " . mysql_error());

// Threads table
$query_tthreads = "CREATE TABLE `tthreads` ( `FID` int(10) unsigned NOT NULL auto_increment, `FBoardID` int(11) NOT NULL default '0', `FTitle` tinytext NOT NULL, `FNickStarted` tinytext NOT NULL, `FDateStarted` datetime NOT NULL default '0000-00-00 00:00:00', `FNickLastMsg` tinytext NOT NULL, `FDateLastMsg` datetime NOT NULL default '0000-00-00 00:00:00', `FReplies` int(11) NOT NULL default '0', `FSticky` tinyint(1) NOT NULL default '0', KEY `FID` (`FID`) ) TYPE=MyISAM;";

mysql_query($query_tthreads)
     or die("TThreads creation failed : " . mysql_error());

// Users table
$query_tusers = "CREATE TABLE `tusers` ( `FID` int(10) unsigned NOT NULL auto_increment, `FLevel` int(11) NOT NULL default '0', `FNickName` tinytext NOT NULL, `FPassword` tinytext NOT NULL, `FRealName` tinytext NOT NULL, `FEmail` tinytext NOT NULL, `FDateJoined` datetime NOT NULL default '0000-00-00 00:00:00', `FDateLastLogin` datetime NOT NULL default '0000-00-00 00:00:00', PRIMARY KEY  (`FID`) ) TYPE=MyISAM;";

mysql_query($query_tusers)
     or die("TUsers creation failed : " . mysql_error());
	

print ("Database created successfully!<br>");

$query = "INSERT INTO tusers ( FLevel , FNickName , FPassword , FRealName , FEmail , FDateJoined , FDateLastLogin )" . " VALUES ( 10, '".$_POST['username']."', '".md5($_POST['password'])."', '".$_POST['realname']."', '" . $_POST['email']."', NOW( ) , NOW( ))";

mysql_query($query)
     or die("Query failed : " . mysql_error());

print ("User " . $_POST['username'] . " with password " . $_POST['password'] . " created successfully.<br>");

print ("<a href=\"./index.php\">Use it now!");


?>