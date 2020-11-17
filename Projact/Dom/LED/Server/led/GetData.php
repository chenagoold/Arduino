<?php 
include 'connect.php';

if(!empty($_POST)){
	$id=$_POST["id"];
	$pdo = Database::connect();
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sql = "SELECT * FROM statusled WHERE id = ?";

	$q = $pdo->prepare($sql);
	$q->execute(array($id));
	$data = $q->fetch(PDO::FETCH_ASSOC);
	Database::disconnect();

	echo $data['Status'];


}


?>
