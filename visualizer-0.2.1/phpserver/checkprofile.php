<?php
// First thing to do is to start the session
session_start();

include "dbconnect.inc";

function checkPassword() {
  if (isset($_POST['FPassword1']) && isset($_POST['FPassword2'])) {
    if ($_POST['FPassword1'] == "" && $_POST['FPassword2'] == "") {
      return 1; // NO PASSWORD CHANGE
    } else if ($_POST['FPassword1'] == $_POST['FPassword2']) {
      return 2; // CHANGE PASSWORD
    } else {
      return 0; // INVALID COMBINATION
    }
  } else {
    return 0; // INVALID COMBINATION
  }
	
}

function modify() {
  $validPass = checkPassword();

  if ($validPass == 0) {
    print ("Error with the passwords!");
    return false;
  }

  // check if it is a valid request.
  if (isset($_SESSION['user_lvl'])
      && isset($_POST['FID']) 
      && isset($_POST['FNickName'])
      && isset($_POST['FRealName'])
      && isset($_POST['FEmail'])) {

    // Verify that NickName isn't colliding =)
    $query = "SELECT FID FROM tusers WHERE FNickName='${_POST['FNickName']}' AND FID <> '${_POST['FID']}'";
    $result = mysql_query($query)
      or die("Query failed : " . mysql_error());

    if ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
      print("Already someone else with that user name!");

      /* Free resultset */
      mysql_free_result($result);

      return false;
    }

    /* Free resultset */
    mysql_free_result($result);

    $query = "";

    if ($_POST['FID'] == $_SESSION['user_id']) {
      if (isset($_POST['FLevel'])) {
	if ($_SESSION['user_lvl'] == $_POST['FLevel'] || ($_SESSION['user_lvl'] != $_POST['FLevel'] && $_SESSION['user_lvl'] == 10 && $_POST['FLevel'] > 0 && $_POST['FLevel'] <= 10)) {
	  // This is a valid request for the level (may downgrade his access if he wish)
	  if ($validPass == 2) {
	    $query = "UPDATE tusers SET FLevel='${_POST['FLevel']}', FNickName='${_POST['FNickName']}', FRealName='${_POST['FRealName']}', FEmail='${_POST['FEmail']}', FPassword='" . md5($_POST['FPassword1']) . "' WHERE FID='${_POST['FID']}'";
	  } else {
	    $query = "UPDATE tusers SET FLevel='${_POST['FLevel']}', FNickName='${_POST['FNickName']}', FRealName='${_POST['FRealName']}', FEmail='${_POST['FEmail']}' WHERE FID='${_POST['FID']}'";
	  }
	} else {
	  print("Error with the user level!");
	  return false;
	}
      } else {
	// No level asked, valid as long as password are valid ! :)
	if ($validPass == 2) {
	  $query = "UPDATE tusers SET FNickName='${_POST['FNickName']}', FRealName='${_POST['FRealName']}', FEmail='${_POST['FEmail']}', FPassword='" . md5($_POST['FPassword1']) . "' WHERE FID='${_POST['FID']}'";
	} else {
	  $query = "UPDATE tusers SET FNickName='${_POST['FNickName']}', FRealName='${_POST['FRealName']}', FEmail='${_POST['FEmail']}' WHERE FID='${_POST['FID']}'";
	}
      }
    } else {
      // Not ourself. UserLevel must be admin and we must have other level ;)
      if ($_SESSION['user_lvl'] == 10 && isset($_POST['FLevel']) && $_POST['FLevel'] > 0 && $_POST['FLevel'] <= 10) {
	if ($validPass == 2) {
	  $query = "UPDATE tusers SET FLevel='${_POST['FLevel']}', FNickName='${_POST['FNickName']}', FRealName='${_POST['FRealName']}', FEmail='${_POST['FEmail']}', FPassword='" . md5($_POST['FPassword1']) . "' WHERE FID='${_POST['FID']}'";
	} else {
	  $query = "UPDATE tusers SET FLevel='${_POST['FLevel']}', FNickName='${_POST['FNickName']}', FRealName='${_POST['FRealName']}', FEmail='${_POST['FEmail']}' WHERE FID='${_POST['FID']}'";
	}
      } else {
	print("Error with the user level!");
	return false;
      }

    }

    mysql_query($query)
      or die("Query failed : " . mysql_error());

    return true;
  } else {
    print("Hello, you are new here?");
    return false;
  }
}

function remove() {
  if (isset($_SESSION['user_lvl']) && (($_SESSION['user_id'] == $_POST['FID']) || ($_SESSION['user_lvl'] == 10))) {
    $query = "DELETE FROM tusers WHERE FID='${_POST['FID']}'";

    mysql_query($query)
      or die("Query failed : " . mysql_error());

    return true;
  } else {
    print("Error with the request!");
    return false;
  }
}

if (isset($_POST['action'])) {
  if ($_POST['action'] == Modify) {
    $res = modify();
  } else if ($_POST['action'] == Delete) {
    $res = remove();
  }

  /* Closing connection */
  mysql_close($link);

  if ($res)
    header("Location: index.php");

}
?>
