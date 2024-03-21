<?php

require 'database.php';

function get_post($key, $default) {
    if (isset($_POST[$key]) && $_POST[$key]) {
        return $POST[$key];
    }
    return $default;
}

$host = get_post('database_server_name', 'localhost');
$port = 5432;
$db = get_post('database_database_name', 'my_db');
$user = get_post('database_user', 'postgres');
$password = get_post('database_password', 'password');
$table_name = get_post('database_table_name', 'data');

$conn = create_connection($host, $port, $db, $user, $password);
create_user_table($conn);
prepare_queries($conn);

$user = select_user_account($conn, $_POST['login'], $_POST['password']);
if (!$user) {
    header("Location: /user-not-found.html");
    exit;
}

$data = array_map('str_getcsv', file($_FILES['file']['tmp_name']));

create_data_table($conn, $table_name);
foreach ($data as $line) {
    insert_data($conn, $table_name, $user[0], $line);
}

header("Location: /form-success.html");
