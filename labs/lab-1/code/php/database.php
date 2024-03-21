<?php

function create_connection($host, $port, $db, $user, $password)
{
    $conn = pg_connect(
        "host=$host port=$port dbname=$db user=$user password=$password"
    );

    if ($conn === false) {
        echo 'Connection failed';
        exit;
    }

    return $conn;
}

function create_user_table($conn)
{
    return pg_query(
        $conn,
        '
        CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

        CREATE TABLE IF NOT EXISTS user_account (
            id TEXT PRIMARY KEY DEFAULT uuid_generate_v4(),
            login TEXT NOT NULL,
            password TEXT NOT NULL
        );
        '
    );
}

function create_data_table($conn, $table_name)
{
    $table_name = $table_name ?? 'data';
    $table_name = pg_escape_string($conn, $table_name);
    return pg_query(
        $conn,
        "
        CREATE EXTENSION IF NOT EXISTS \"uuid-ossp\";

        CREATE TABLE IF NOT EXISTS $table_name (
            user_id TEXT,
            date DATE,
            time TIME,
            field1 FLOAT NOT NULL,
            field2 FLOAT NOT NULL,
            field3 FLOAT NOT NULL,
            field4 FLOAT NOT NULL,
            field5 INTEGER NOT NULL,
            PRIMARY KEY (user_id, date, time),
            FOREIGN KEY (user_id) REFERENCES user_account (id)
        );
        "
    );
}

function prepare_queries($conn)
{
    pg_prepare(
        $conn,
        'select_user_account_query',
        '
        SELECT id, login, password
        FROM user_account
        WHERE login = $1 AND password = $2;
        '
    );
}

function select_user_account($conn, $login, $password)
{
    $query = pg_execute(
        $conn,
        'select_user_account_query',
        array($login, $password)
    );
    return pg_fetch_row($query);
}

function insert_data($conn, $table_name, $user_id, $data)
{
    $escape_data = function ($str) use (&$conn) {
        return pg_escape_string($conn, $str);
    };
    $data = array_map($escape_data, $data);
    return pg_query(
        $conn,
        "
        INSERT INTO $table_name (
            user_id,
            date,
            time,
            field1,
            field2,
            field3,
            field4,
            field5
        ) VALUES (
            '$user_id',
            '$data[0]',
            '$data[1]',
            $data[2],
            $data[3],
            $data[4],
            $data[5],
            $data[6]
        )
        ON CONFLICT (user_id, date, time) DO UPDATE
        SET
            field1 = EXCLUDED.field1,
            field2 = EXCLUDED.field2,
            field3 = EXCLUDED.field3,
            field4 = EXCLUDED.field4,
            field5 = EXCLUDED.field5;
        "
    );
}
