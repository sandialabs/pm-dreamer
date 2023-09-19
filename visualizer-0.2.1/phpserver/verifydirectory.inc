<?php

include_once "dbparam.inc";

// First, go to the file directory.
if (!file_exists($fmroot)) {
  mkdir($fmroot, 0700);
}

// Then check if received exist.
if (!file_exists($fmreceivedfile)) {
  mkdir($fmreceivedfile, 0700);
}

// What about generated ?
if (!file_exists($fmgeneratedfile)) {
  mkdir($fmgeneratedfile, 0700);
}

// Now we need to create the user directory if it's missing.
if (!file_exists($fmreceivedfile.$_SESSION['user_id'])) {
  mkdir($fmreceivedfile.$_SESSION['user_id'], 0700);
}

if (!file_exists($fmgeneratedfile.$_SESSION['user_id'])) {
  mkdir($fmgeneratedfile.$_SESSION['user_id'], 0700);
}
  
?>