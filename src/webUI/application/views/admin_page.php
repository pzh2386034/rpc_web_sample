<html>
    <?php
    if (isset($this->session->userdata['logged_in'])) {
        $username = ($this->session->userdata['logged_in']['username']);
        $email = ($this->session->userdata['logged_in']['email']);
    } else {
        header("location: login");
    }
    ?>
    <head>
        <title>Admin Page</title>
        <link rel="stylesheet" type="text/css" href="<?php echo base_url(); ?>css/style.css">
        <link href='http://fonts.googleapis.com/css?family=Source+Sans+Pro|Open+Sans+Condensed:300|Raleway' rel='stylesheet' type='text/css'>
    </head>
    <body>
        <div id="profile">
            <h2>Hello <b><i><?php echo $username ?></i></b> !</h2>
            <h5>Welcome to Admin Page</h5>
            <h5>Your Username is <?php echo $username ?> </h5>
            <h5>Your Email is <?php echo $email ?> </h5>
            <b><a href="logout">Logout</a></b>
        </div>
        <br/>
    </body>
</html>
