<?php

function execInBackground($path, $exe, $args = "") {
  global $conf;
  
  if (file_exists($path . $exe)) {
    chdir($path);
    if (substr(php_uname(), 0, 7) == "Windows"){
      pclose(popen("start \"bla\" \"" . $exe . ".exe\" " . $args, "r"));   
    } else {
      exec("./" . $exe . " " . $args . " >& /dev/null &");   
    }
  }
}

function execAndWait($path, $exe, $args = "") {
  if (file_exists($path . $exe)) {
    chdir($path);
    if (substr(php_uname(), 0, 7) == "Windows"){
      pclose(popen("\"" . $exe . ".exe\" " . $args, "r"));   
    } else {
      exec("./" . $exe . " " . $args . " >& /dev/null ");   
    }
  }
}

?>