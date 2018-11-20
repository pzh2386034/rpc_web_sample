<html>
    <?php
        if (isset($this->session->userdata['logged_in'])) {
            $redir_login = base_url("user_authentication/user_login_process");
            header("location: ".$redir_login);
        }
    ?>
    <head>
        <title>Admin Login</title>
        <link rel="stylesheet" type="text/css" href="<?php echo base_url(); ?>css/style.css">
        <link rel="stylesheet" type="text/css" href="<?php echo base_url(); ?>css/basic.css" >
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
            if (isset($message_display))
            {
                echo "<div class='message'>";
                echo $message_display;
                echo "</div>";
            }
        ?>
        <div id="main">
            <div id="login">
                <h2 name="LoginMethod">User Login</h2>
                <hr/>
                <div class="error_msg">
                    <?php
                        if (isset($error_message)) {
                            echo $error_message;
                        }
                    ?>
                </div>
                <?php  echo form_open('user_authentication/user_login_process');?>
                <br/>
                    <label>Username :</label>
                    <input type="text" name="username" id="name" placeholder="username"/>
                    <br/>
                    <br/>
                    <label>Password :</label>
                    <input type="password" name="password" id="password" placeholder="************">
                    <br/>
                    <br/>
                    <div>
                        <table>
                            <tr>
                                <td>
                                    <input type="radio" name="LoginMethod" value="user" checked>User login
                                </td>
                                <td>
                                    <input type="radio" name="LoginMethod" value="admin">Admin login
                                </td>
                            </tr>
                        </table>
                    </div>
                    <br/>
                    <input type="submit" value=" Login " name="submit"/>
                    <br/>
                </form>
                <div>
                    <table class="divtable">
                        <tr>
                            <td class="tdleft">
                                <a href="<?php echo base_url() ?>user_authentication/new_user_registration">SignUp</a>
                            </td>
                        </tr>
                    </table>
                </div>
            </div>
        </div>
    </body>
    <script type="text/javascript" src="<?php echo base_url() ?>extJS/jquery/jquery-3.3.1.min.js"> </script>
    <script type="text/javascript">
        $("input[name=LoginMethod][type=radio]").change(function()
            {
                if (this.value == 'user')
                    {
                        $("h2[name=LoginMethod]").text('User Login');
                    }
                else if (this.value=='admin')
                    {
                        $("h2[name=LoginMethod]").text('Admin Login');
                    }
            })
    </script>
</html>
