<?php
  require 'database.php';
  
  //---------------------------------------- Condition to check that POST value is not empty.
  if (!empty($_POST)) {
    //........................................ keep track POST values
    $id = $_POST['id'];
    $status_read_sensor_dht22 = $_POST['status_read_sensor_dht22'];

    // Waste-related
    $waste_level_percent = $_POST['waste_level_percent'];
    $waste_status = $_POST['waste_status'];

    // Temperature-related
    $temperature_celsius = $_POST['temperature_celsius'];
    $tempt_status = $_POST['tempt_status'];

    // Humidity-related
    $humidity_percent = $_POST['humidity_percent'];
    $humidity_status = $_POST['humidity_status'];

    // Methane
    $methane_level = $_POST['methane_level'];
    $methane_status = $_POST['methane_status'];
    //........................................
    
    //........................................ Get the time and date.
    date_default_timezone_set("Asia/Manila"); // Look here for your timezone : https://www.php.net/manual/en/timezones.php
    $time = date("H:i:s");
    $date = date("Y-m-d");
    //........................................
    
    //........................................ Updating the data in the table.
    $pdo = Database::connect();
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    // replace_with_your_table_name, on this project I use the table name 'esp32_table_dht22_leds_update'.
    // This table is used to store DHT22 sensor data updated by ESP32. 
    // This table is also used to store the state of the LEDs, the state of the LEDs is controlled from the "home.php" page. 
    // This table is operated with the "UPDATE" command, so this table will only contain one row.
    $sql = "UPDATE sensors_update 
            SET status_read_sensor_dht22 = ?, 
            waste_level_percent = ?, 
            waste_status = ?,  
            temperature_celsius = ?,
            tempt_status = ?,
            humidity_percent = ?,
            humidity_status = ?,
            methane_level = ?,
            methane_status = ?,
            time = ?, 
            date = ? 
            WHERE id = ?";
    $q = $pdo->prepare($sql);
    $q->execute(array($status_read_sensor_dht22,
    $waste_level_percent,
    $waste_status,
    $temperature_celsius,
    $tempt_status,
    $humidity_percent,
    $humidity_status,
    $methane_level,
    $methane_status,
    $time,
    $date,
    $id));
    Database::disconnect();
    //........................................ 
    
    //........................................ Entering data into a table.
    $id_key;
    $board = $_POST['id'];
    $found_empty = false;
    
    $pdo = Database::connect();
    
    }
  function generate_string_id($strength = 16) {
    $permitted_chars = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $input_length = strlen($permitted_chars);
    $random_string = '';
    for($i = 0; $i < $strength; $i++) {
      $random_character = $permitted_chars[mt_rand(0, $input_length - 1)];
      $random_string .= $random_character;
    }
    return $random_string;
  }
  //---------------------------------------- 
?>
