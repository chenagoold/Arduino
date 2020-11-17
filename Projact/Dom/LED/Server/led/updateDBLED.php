<?php

require 'connect.php';

if(!empty($_POST)){
	$Stat = $_POST['Status'];

	//insert data
	$pdo = Database::connect();
	$pdo -> setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sql = "UPDATE statusled  SET Status = ? WHERE id = 0 ";
	$q = $pdo->prepare($sql);
	$q->execute(array($Stat));
	Database::disconnect();
	header("Location: Main.php");


}

?>
