<?php

include "dbparam.inc";
include "execution.inc";

if (isset($_POST['data'])) {
  // print("Data: ${_POST['data']}<br>\n");

  $res = true;
  $tmpfname = str_replace("\\", "", tempnam($fmroot, "REQ"));
  $handle = fopen($tmpfname, "w");
  if (!fwrite($handle, $_POST['data'])) $res=false;
  fclose($handle);

  if ($res == false) {
    print("No space left on device or another device related problem!");
    exit;
  } 
  else {
    $tmpfname2 = str_replace("\\", "", tempnam($fmroot, "RES")) . ".svg";
    execAndWait($execroot, "tree2svg", $tmpfname . " " . $tmpfname2);

    // Then output the result to the user
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Content-Type: image/svg+xml");
    readfile($tmpfname2);

    // Delete both files
    unlink($tmpfname2);
    unlink($tmpfname);
  }
} else{ 
  print("Invalid request!");
}
?>