#!/usr/bin/env php
<?php
/**
 * Reset User 1's password.
 */

$password = file_get_contents(getenv('TUGBOAT_ROOT') . '/.tugboat/password');
$user = user_load(1);
$user->pass = $password;
user_save($user);
