<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "sensor_data_db";

$conn = mysqli_connect($servername, $username, $password, $dbname);

// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

if(isset($_POST["distance"]) && isset($_POST["bottle_count"])) {
    $dis = $_POST["distance"];
    $count = $_POST["bottle_count"];

    $sql = "INSERT INTO bottle_data (distance,bottle_count) VALUES (".$dis.", ".$count.")";

    if (mysqli_query($conn, $sql)) {
        echo "new data created successfully";
    } else {
        echo "Error: ". $sql ."<br>". mysqli_error($conn);
    }
}
?>