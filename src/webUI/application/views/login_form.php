<html>
<?php
if (isset($this->session->userdata['logged_in'])) {

header("location: http://localhost/webUI/user_authentication/user_login_process");
}
?>

<head>
    <title>Login Form</title>
    <link rel="stylesheet" type="text/css" href="<?php echo base_url(); ?>css/style.css">
    <link rel="stylesheet" type="text/css" href="<?php echo base_url(); ?>css/basic.css">
    <link href='http://fonts.googleapis.com/css?family=Source+Sans+Pro|Open+Sans+Condensed:300|Raleway' rel='stylesheet' type='text/css'>
</head>

<body>
    <?php
     if (isset($logout_message))
    {
        echo "<div class='message'>";
        echo $logout_message;
        echo "</div>";
    }
    ?>
    <?php
    if (isset($message_display)) {
        echo "<div class='message'>";
        echo $message_display;
        echo "</div>";
    }
    ?>
    <div id="main">
        <div id="login">
            <h2>Login Form</h2>
            <hr />
            <?php echo form_open('user_authentication/user_login_process')?>
            <?php
            echo "<div class='error_msg'>";
            if(isset($error_message))
            {
                echo $error_message;
            }
            echo validation_errors();
            echo "</div>";
            ?>
            <label>Username :</label>
            <input type="text" name="username" id="name" placeholder="username"/><br /><br />
            <label>Password :</label>
            <input type="password" name="password" id="password" placeholder="*******"/><br /><br />
            <input type="submit" value=" Login " name="submit"/><br />
            <div>
                <table class="divtable">
                    <tr>
                        <td class="tdleft">
                            <a href="<?php echo base_url() ?>user_authentication/new_user_registration">SignUp</a>
                        </td>
                        <td class="tdright">
                            <a href="javascript:void(0)"  onclick="switchAdmin()">Admin login</a>
                        </td>
                    </tr>
                </table>
            </div>
            <?php echo form_close();?>
        </div>
    </div>
</body>
</html>
