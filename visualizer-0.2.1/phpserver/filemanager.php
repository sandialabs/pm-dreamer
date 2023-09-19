<?php
// First thing to do is to start the session
session_start();

include_once "verifydirectory.inc";
include_once "execution.inc";
include_once "dbparam.inc";


function removeDirectory($directory) {
  // this function must remove the whole content and then the directory.
  $d = opendir($directory);
  while (false !== ($entry = readdir($d))) {
    if ($entry != "." && $entry != "..") {
      // Is that a directory ?
      if (is_dir("${directory}/${entry}")) {
        // Yes so we remove that entry !
        removeDirectory("${directory}/${entry}");
      } else {
        // Just a file, delete it quickly!
        unlink("${directory}/${entry}");
      }
    }
  }
  closedir($d);

  // Finally remove the directory
  rmdir($directory);
}

if (!isset($_SESSION['user_lvl']) || ($_SESSION['user_lvl'] < 2) || !isset($_SESSION['user_recdir'])) {
  $include_file = "index.inc";
  include "main.inc";
  exit;
}

if (isset($_FILES['userfile'])) {
  $directory = $fmreceivedfile.$_SESSION['user_id'].$_SESSION['user_recdir'];
  $uploadfile = $directory . $_FILES['userfile']['name'];

  if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) {
    header("Location: index.php?request=filemanager");
    exit;   
  } else {
    print ("<pre>");	
    print ("Error with the file upload : file may have been bigger than the size allowed.");
    print ("Possible file upload attack!  Here's some debugging info:\n");
    print_r($_FILES);
    print ("</pre>");
    exit;
  }

} else if (isset($_GET['action'])) {
  $action = $_GET['action'];

  if ($action == "chdirrec") {
    // Change the received file directory
    if (!isset($_GET['dir']) || strpos($_GET['dir'], "..") !== false || strpos($_GET['dir'], "/") !== false || strpos($_GET['dir'], "\\") !== false) {
      print ("Error : directory name is invalid !");
      exit;
    } else {
      $temp = $_SESSION['user_recdir'] . $_GET['dir'] . "/";			
      $dir = $fmreceivedfile . $_SESSION['user_id'] . $temp;
      if (is_dir($dir)) {
        $_SESSION['user_recdir'] = $temp;
      } else {
        print ("Error : invalid directory!<br>");
        print ($dir);
        exit;
      }
    }
  } else if ($action == "updirrec") {
    // Get back one level in the received file directory
    $pos = strrpos(substr($_SESSION['user_recdir'], 0, strlen($_SESSION['user_recdir'])-1), "/");
    $_SESSION['user_recdir'] = substr($_SESSION['user_recdir'], 0, $pos+1);
  } else if ($action == "chdirgen") {
    // Change the generated file directory
    if (!isset($_GET['dir']) || strpos($_GET['dir'], "..") !== false || strpos($_GET['dir'], "/") !== false || strpos($_GET['dir'], "\\") !== false) {
      print ("Error : directory name is invalid !");
      exit;
    } else {
      $temp = $_SESSION['user_gendir'] . $_GET['dir'] . "/";			
      $dir = $fmgeneratedfile . $_SESSION['user_id'] . $temp;
      if (is_dir($dir)) {
        $_SESSION['user_gendir'] = $temp;
      } else {
        print ("Error : invalid directory!<br>");
        print ($dir);
        exit;
      }
    }
  } else if ($action == "updirgen") {
    // Get back one level in the received file directory
    $pos = strrpos(substr($_SESSION['user_gendir'], 0, strlen($_SESSION['user_gendir'])-1), "/");
    $_SESSION['user_gendir'] = substr($_SESSION['user_gendir'], 0, $pos+1);
  } else if ($action == "downloadrecfile" || $action == "downloadgenfile") {
    // must send the requested file.
    if (isset($_GET['file']) && strpos($_GET['file'], "..") === false && strpos($_GET['file'], "/") === false && strpos($_GET['file'], "\\") === false) {
      if ($action == "downloadrecfile")
      $directory = $fmreceivedfile.$_SESSION['user_id'].$_SESSION['user_recdir'];
      else
      $directory = $fmgeneratedfile.$_SESSION['user_id'].$_SESSION['user_gendir'];

      $filename = $_GET['file'];
      $ext = substr( $filename,-3 );
      if (!file_exists($directory.$filename)) {
        print("ERROR: File not found.");
        exit;
      }

      switch( $ext ){
      case "xml": $ctype="application/xml";	  	  break;
      case "pdf": $ctype="application/pdf";		  break;
      case "exe": $ctype="application/octet-stream";      break;
      case "zip": $ctype="application/zip";		  break;
      case "doc": $ctype="application/msword";            break;
      case "xls": $ctype="application/vnd.ms-excel";      break;
      case "ppt": $ctype="application/vnd.ms-powerpoint"; break;
      case "gif": $ctype="image/gif";			  break;
      case "png": $ctype="image/png";			  break;
      case "jpg": $ctype="image/jpg";			  break;
      default:    $ctype="application/force-download";	  break;
      }

      header("Pragma: public");
      header("Expires: 0");
      header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
      header("Content-Type: $ctype");

      $user_agent = strtolower($_SERVER["HTTP_USER_AGENT"]);
      if ((is_integer(strpos($user_agent, "msie"))) && (is_integer(strpos($user_agent, "win")))) {
        header( "Content-Disposition: filename=".basename($directory.$filename).";" );
      } else {
        header( "Content-Disposition: attachment; filename=".basename($directory.$filename).";" );
      }
      header("Content-Transfer-Encoding: binary");
      header("Content-Length: ".filesize($directory.$filename));
      readfile($directory.$filename);
      exit;
    } else {
      print ("Error: invalid file request [" . $_GET['file'] . "]");
      exit;
    }

  } else {
    print ("Error: unsupported command : " . $action);
    exit;
  }

  header("Location: index.php?request=filemanager");
  exit;   

} else if (isset($_POST['action'])) {
  $action = $_POST['action'];
  if ($action == "Create Directory") {
    if (!isset($_POST['dir'])) {
      print ("Error: missing parameter.");
      exit;
    } else {
      $dir = $_POST['dir'];
      if (preg_match("/[a-z,A-Z,0-9,_, ,-][a-z,A-Z,0-9,_, ,-]*/", $dir, $matches)) {
        if ($matches[0] == $dir) {
          if (mkdir($fmreceivedfile.$_SESSION['user_id'].$_SESSION['user_recdir'].$dir)) {
            header("Location: index.php?request=filemanager");
            exit;   
          } else {
            print ("Error creating the requested directory.");
            exit;
          }
        } else {
          print ("Some caracters where not allowed. Use only letters, numbers, spaces and - or _");
          exit;
        }
      } else {
        print ("Error while matching with the regular expression -> invalid directory name");
        exit;
      }
    }
  } else if ($action == "Create Directory") {
    if (!isset($_POST['dir'])) {
      print ("Error: missing parameter.");
      exit;
    } else {
      $dir = $_POST['dir'];
      if (preg_match("/[a-z,A-Z,0-9,_, ,-][a-z,A-Z,0-9,_, ,-]*/", $dir, $matches)) {
        if ($matches[0] == $dir) {
          if (mkdir($fmgeneratedfile.$_SESSION['user_id'].$_SESSION['user_gendir'].$dir)) {
            header("Location: index.php?request=filemanager");
        exit;   
      } else {
        print ("Error creating the requested directory.");
        exit;
      }
    } else {
      print ("Some caracters where not allowed. Use only letters, numbers, spaces and - or _");
      exit;
    }
      } else {
        print ("Error while matching with the regular expression -> invalid directory name");
        exit;
      }
    }
  }  
  else if ($action == "Generate Report") {
    // Must find which file is selected
    if (isset($_POST['recfile'])) {
      if (is_array($_POST['recfile'])) {
        if (count($_POST['recfile']) > 0) {
          $params = "";
          $sep = "";
          foreach ($_POST['recfile'] as $record) {
            if (is_dir($record)) {
              print ("Src can't be a directory!");
              exit;
            }
            $params = $params . $sep . "\"" . $fmreceivedfile.$_SESSION['user_id'].$_SESSION['user_recdir'].$record . "\"";
            $sep = " ";
          }
          
          $params = "-b $SDServerName -o " . getcwd() . "/" . $fmgeneratedfile.$_SESSION['user_id'].$_SESSION['user_gendir'] . " " . $params;

          execAndWait($execroot, "beagleviz", $params);
	  
	  header("Location: index.php?request=filemanager&action=removepopup");
          exit;
        } else {
          print ("At least one file must be selected, and you selected none!<br>");
          foreach ($_POST['recfile'] as $record) {
            print ($record . "<br>");
          }
          exit;
        }
      } else {
	print ("No file selected or not an array representation!");
	exit;
      }
    } else {
      print ("No file selected!");
      exit;
    }
  } else {
    print ("Error: unsupported command : " . $action);
    exit;
  }
} else if (isset($_POST['actionrec'])) {
  $action = $_POST['actionrec'];
  if ($action == "Create Directory") {
    if (!isset($_POST['dirrec'])) {
      print ("Error: missing parameter.");
      exit;
    } else {
      $dir = $_POST['dirrec'];
      if (preg_match("/[a-z,A-Z,0-9,_, ,-][a-z,A-Z,0-9,_, ,-]*/", $dir, $matches)) {
	if ($matches[0] == $dir) {
	  if (mkdir($fmreceivedfile.$_SESSION['user_id'].$_SESSION['user_recdir'].$dir)) {
	    header("Location: index.php?request=filemanager");
	    exit;   
	  } else {
	    print ("Error creating the requested directory.");
	    exit;
	  }
	} else {
	  print ("Some caracters where not allowed. Use only letters, numbers, spaces and - or _");
	  exit;
	}
      } else {
	print ("Error while matching with the regular expression -> invalid directory name");
	exit;
      }
    }
  } else if ($action == "Delete Selected") {
    $rootrec = $fmreceivedfile.$_SESSION['user_id'].$_SESSION['user_recdir'];
    if (isset($_POST['recfile'])) {
      if (is_array($_POST['recfile'])) {
	foreach ($_POST['recfile'] as $directory) {
	  if (is_dir($rootrec.$directory)) {
	    // Skip the file (directory)!
	  } else {
	    // Remove this file
	    unlink($rootrec.$directory);
	  }
	}
      } else {
	print ("No directory selected or not an array representation!");
	exit;
      }
    }

    header("Location: index.php?request=filemanager");
    exit; 
  } else if ($action == "Remove Directory") {
    // Must remove every directory named in the list, and all their content
    $rootrec = $fmreceivedfile.$_SESSION['user_id'].$_SESSION['user_recdir'];
    if (isset($_POST['recfile'])) {
      if (is_array($_POST['recfile'])) {
	foreach ($_POST['recfile'] as $directory) {
	  if (is_dir($rootrec.$directory)) {
	    // Remove this directory
	    removeDirectory($rootrec.$directory);
	  } else {
	    // Skip the file!
	  }
	}
      } else {
	print ("No directory selected or not an array representation!");
	exit;
      }
    }

    header("Location: index.php?request=filemanager");
    exit; 
  }
} else if (isset($_POST['actiongen'])) {
  $action = $_POST['actiongen'];
  if ($action == "Create Directory") {
    if (!isset($_POST['dirgen'])) {
      print ("Error: missing parameter.");
      exit;
    } else {
      $dir = $_POST['dirgen'];
      if (preg_match("/[a-z,A-Z,0-9,_, ,-][a-z,A-Z,0-9,_, ,-]*/", $dir, $matches)) {
	if ($matches[0] == $dir) {
	  if (mkdir($fmgeneratedfile.$_SESSION['user_id'].$_SESSION['user_gendir'].$dir)) {
	    header("Location: index.php?request=filemanager");
	    exit;   
	  } else {
	    print ("Error creating the requested directory.");
	    exit;
	  }
	} else {
	  print ("Some caracters where not allowed. Use only letters, numbers, spaces and - or _");
	  exit;
	}
      } else {
	print ("Error while matching with the regular expression -> invalid directory name");
	exit;
      }
    }
  } else if ($action == "Delete Selected") {
    // Must remove every files named in the list
    $rootgen = $fmgeneratedfile.$_SESSION['user_id'].$_SESSION['user_gendir'];
    if (isset($_POST['genfile'])) {
      if (is_array($_POST['genfile'])) {
	foreach ($_POST['genfile'] as $directory) {
	  if (is_dir($rootgen.$directory)) {
	    // Skip the file!
	  } else {
	    // Remove this file
	    unlink($rootgen.$directory);
	  }
	}
      } else {
	print ("No directory selected or not an array representation!");
	exit;
      }
    }

    header("Location: index.php?request=filemanager");
    exit; 
  } else if ($action == "Download Report") {
    require_once "zip.inc";
    $d = dir($fmgeneratedfile.$_SESSION['user_id'].$_SESSION['user_gendir']);
    $zip = new zipfile();
    
    if (@function_exists('gzcompress')) {
      while (false !== ($entry = $d->read())) {
        if ($entry != "." && $entry != "..") {
          if (!is_dir($d->path . $entry)) {
            if ($entry != ".htaccess" && $entry != ".htpasswd") {
              $zip->addHandle($d->path . $entry, $entry, 0);
            }
          }
        }
      }
    } else {
      print ("Missing function gzcompress<br>\r\n");
    }

    header("Content-Type: application/zip");
    header('Expires: ' . gmdate('D, d M Y H:i:s') . ' GMT');
    header('Content-Disposition: attachment; filename=report.zip');
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");

    echo ($zip->file());

    exit;
  
  } else if ($action == "Remove Directory") {
    // Must remove every directory named in the list, and all their content
    $rootgen = $fmgeneratedfile.$_SESSION['user_id'].$_SESSION['user_gendir'];
    if (isset($_POST['genfile'])) {
      if (is_array($_POST['genfile'])) {
	foreach ($_POST['genfile'] as $directory) {
	  if (is_dir($rootgen.$directory)) {
	    // Remove this directory
	    removeDirectory($rootgen.$directory);
	  } else {
	    // Skip the file!
	  }
	}
      } else {
	print ("No directory selected or not an array representation!");
	exit;
      }
    }

    header("Location: index.php?request=filemanager");
    exit; 
  } else {
    print ("Unrecognized action : $action");
  }    
}

?>

