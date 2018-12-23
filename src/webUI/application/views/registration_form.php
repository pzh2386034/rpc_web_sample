<html>
    <?php
    if (isset($this->session->userdata['logged_in'])) {
        header("location: http://localhost/webUI/user_authentication/user_login_process");
    }
    ?>
    <head>
        <title>Registration Form</title>
        <link rel="stylesheet" type="text/css" href="<?php echo base_url(); ?>css/style.css">
        <link href='http://fonts.googleapis.com/css?family=Source+Sans+Pro|Open+Sans+Condensed:300|Raleway' rel='stylesheet' type='text/css'>
    </head>
    <body>
        <div id="main">
            <div id="login">
                <h2>Registration Form</h2>
                <hr/>
                <div class="error_msg">
                    <?php echo validation_errors(); ?>
                </div>
                <?php
                    $fattr = array('class' => 'form-signin');

                    echo form_open('user_authentication/new_user_registration', $fattr);?>
                    <br/>
                    <label>Create Username :</label>
                    <input type="text" name="username" value="<?php echo set_value('username'); ?>" size="50">
                    <!-- 表单验证通过后，提交数据库失败，信息展示 -->
                    <div class='error_msg'>
                        <?php
                            if (isset($message_display)) {
                                echo $message_display;
                            }
                        ?>
                    </div>
                    <br/>
                    <label>Email :</label>
                    <input type="text" name="email_value" value="<?php echo set_value('email_value') ?>"
                    <br/>
                    <label>Password :</label>
                    <input type="password" name="password">
                    <br/>
                    <br/>
                    <div>
                        <input type="submit" value="Submit"/>
                    </div>
                    </form>
                <a href="javascript:void(0)" align="left" onclick="loginclick()">Login</a>
            </div>
        </div>
    </body>
    <script type="text/javascript">
     function loginclick()
     {
         window.location.href="<?php echo base_url() ?>"+"user_authentication";
      }

    </script>
</html>
