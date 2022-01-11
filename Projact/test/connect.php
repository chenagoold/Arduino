<?php

	function Connection(){
		$server="localhost";
                $db="bdarduino";
		$user="admin";
		$pass="toor";
                
		define ('DBHOST', 'localhost');
		define ('DBNAME', 'templog'); // Имя базы данных
		define ('DBUSER', 'admin'); //Пользователь базы
		define ('DBPASS', 'toor');//Пароль БД
		
	   	
		$connection = mysqli_connect($server, $user, $pass, $db) or die ("Соединение с БД отсуствует");
                mysqli_query($connection, 'SET NAMES utf8');

		if (!$connection) {
	    	die('MySQL ERROR: ' . mysqli_error());
		}
		
		mysqli_select_db($connection, $db) or die( 'MySQL ERROR: '. mysqli_error() );

		return $connection;
	}
?>
