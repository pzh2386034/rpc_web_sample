<?php

// session_start();
class User_Authentication extends CI_Controller
{
    public function __construct(){
        parent::__construct();

        //load form helper library
        $this->load->helper('form');
        //load form validation library
        $this->load->library('form_validation');
        $this->load->helper('security');


        //load session library
        $this->load->library('session');
        //load database
        $this->load->model('login_database');
        $this->load->helper('url');
    }
    //show login pages
    //index method will be called when the second paragraph is empty.
    public function index(){
        if (isset($this->session->userdata['logged_in']))
        {
            // redirect('view/admin_page', $method = 'auto', $code = NULL);
            $this->load->view('admin_page');
        }
        // $this->load->view('registration_form');
        // $this->load->view('admin_login_form');
        $this->load->view("admin_login_form.php", $vars = array(), $return = FALSE);
    }

    // Validate and store registration data in database
    public function new_user_registration() {

        // Check validation for user input in SignUp form
        $this->form_validation->set_rules('username', 'Username', 'trim|required|xss_clean');
        $this->form_validation->set_rules('email_value', 'Email', 'trim|required|valid_email|xss_clean');
        $this->form_validation->set_rules('password', 'Password', 'trim|required|xss_clean');
        if ($this->form_validation->run() == FALSE) {
            $this->load->view('registration_form');
        } else {
            $data = array(
                'user_name' => $this->input->post('username'),
                'user_email' => $this->input->post('email_value'),
                'user_password' => $this->input->post('password')
            );
            $result = $this->login_database->registration_insert($data);
            if ($result == TRUE) {
                $data['message_display'] = 'Registration Successfully !';
                $this->load->view('login_form', $data);
            } else {
                $data['message_display'] = 'Username already exist!';
                $this->load->view('registration_form', $data);
            }
        }
    }

    //
    public function user_login_process()
    {
        $this->form_validation->set_rules('username', 'Username', 'trim|required|xss_clean');
        $this->form_validation->set_rules('password', 'Password', 'trim|required|xss_clean');

        if ($this->form_validation->run() == FALSE) {
            if(isset($this->session->userdata['logged_in'])){
                //如果已经登入，则跳转至admin_page页面
                $this->load->view('admin_page');
            }else{
                //否则，跳转至登入界面，可以选择登入方式
                $this->load->view('admin_login_form');
            }
        } else {
            $data = array(
                'username' => $this->input->post('username'),
                'password' => $this->input->post('password')
            );
            if ($this->input->post('LoginMethod', $xss_clean = NULL) == "user")
            {
                $result = $this->login_database->login($data);
                if ($result == TRUE) {

                    $username = $this->input->post('username');
                    $result = $this->login_database->read_user_information($username);
                    if ($result != false) {
                        $session_data = array(
                            'username' => $result[0]->user_name,
                            'email' => $result[0]->user_email,
                            'loginmethod' => "user",
                        );
                        // Add user data in session
                        $this->session->set_userdata('logged_in', $session_data);
                        $this->load->view('admin_page');
                    }
                }else{
                    $data = array(
                        'error_message' =>'Invalid Username or Password',
                    );
                    $this->load->view('admin_login_form', $data, $return = FALSE);
                }
            }
            else
            {
                $username = $this->input->post('username');
                $password = $this->input->post('password');
                $retcode=cgi_admin_user_login($username, $password);
                if ($retcode == 0)
                {
                    $session_data = array(
                        'username' => $username,
                        'loginmethod' => "admin",
                    );
                    $this->session->set_userdata('logged_in', $session_data);
                    $this->load->view('admin_page', $vars = array(), $return = FALSE);
                }
                else
                {
                    $data = array(
                        'error_message' =>'Invalid Username or Password',
                    );
                    $this->load->view('admin_login_form', $data, $return = FALSE);
                }
            }
        }
    }
    public function logout()
    {
        //Removing sesssion data
        $sess_array = array('username'=>'');
        $this->session->unset_userdata('logged_in', $sess_array);
        $data['message_display']='Successfully Logout';
        $this->load->view('admin_login_form', $data);
    }
}
?>
