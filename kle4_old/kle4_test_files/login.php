<?php
function login_page($error_message)
{
?>
<html LANG="el">
        <head>
        <title>
        Login
        </title>
         <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-7">
        </head>
        <body>
        <form method="POST" action="validate.php">
        <?php
          if (isset($error_message))
          echo "<h3><font color=ref> $error_message</font></h3>";
          ?>
          <table>
                  <tr><td>Enter username:</td></tr>
                  <tr><td><input type ="text" size=10 maxlength=10
                 name="formUsername"></td></tr>
                  <tr><td>Enter Password:</td></tr>
                  <td><input type ="password" size=10 maxlength=10 name="formPassword"></td>
          </table>
        <p><input type ="submit" value="Log in"></p>
        </form>
        </body>
</html>
<?
}
function logged_on_page($currentLoginName)
{
?>
<!DOCTYPE HTML PUBLIC
"-//W3C//DTD HTML 4.0 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
<head><title>Login Page</title></head>
<body>
<h2>CdCol</h2>
<h2>You are currently logged in as <?=$currentLoginName ?> </h2>
<!-- <b>< Your IP Address is : <?=$loginIpAddress ?></b> -->
<a href=logout.php>Logout</a>
</body>
</html>
<?
}
//Main
session_start();
if (isset($_SESSION["authenticatedUser"]))
{
  logged_on_page($_SESSION["authenticatedUser"]);
  $id=session_id();
  echo "session id =$id ";
 }
else
{
  echo "You have to Login to move to next pages";
  login_page($_SESSION["loginMessage"]);
  session_destroy();
 }
?>