<?php
$conn = new mysqli("localhost", "username", "password", "simple_clinic");
$result = $conn->query("SELECT * FROM patients");
?>
