<H3>Register</H3><BR>

<form name=form1 action="checkregister.php" method=post>
<table>
<tr>
<td>User Name</td>
<td>
<input size=32 type=text name="username"<?php if (isset($_POST['username'])) print(" value=\"" . $_POST['username'] ."\""); ?>>
</td>
</tr>
<tr>
<td>Realname</td>
<td>
<input size=32 type=text name="realname"<?php if (isset($_POST['realname'])) print(" value=\"" . $_POST['realname'] ."\""); ?>>
</td>
</tr>
<tr>
<td>Email</td>
<td>
<input size=32 type=text name="email"<?php if (isset($_POST['email'])) print(" value=\"" . $_POST['email'] ."\""); ?>>
</td>
</tr>
<tr>
<td>Password</td>
<td>
<input size=32 type=password name="password1"<?php if (isset($_POST['password1'])) print(" value=\"" . $_POST['password1'] ."\""); ?>>
</td>
</tr>
<tr>
<td>Confirm password</td>
<td>
<input size=32 type=password name="password2"<?php if (isset($_POST['password2'])) print(" value=\"" . $_POST['password2'] ."\""); ?>>
</td>
</tr>
<tr align=right>
<td colspan=2>
<input size=32 type=submit name="action" value="Register"></td>
</tr>
</table>   
</form>
