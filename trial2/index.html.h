#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char* index_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>IoT Dashboard</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 20px;
      background-color: #f4f4f9;
      color: #333;
    }

    h1 {
      text-align: center;
    }

    .dashboard {
      display: flex;
      flex-wrap: wrap;
      justify-content: space-around;
      margin-top: 20px;
    }

    .card {
      background: #fff;
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
      border-radius: 8px;
      padding: 20px;
      margin: 10px;
      flex: 1 1 300px;
      text-align: center;
    }

    .progress-bar-container {
      width: 80%;
      height: 20px;
      background-color: #e0e0e0;
      border-radius: 10px;
      overflow: hidden;
      margin: 10px auto;
      position: relative;
    }

    .progress-bar {
      height: 100%;
      width: 0;
      background-color: #4CAF50;
      transition: width 0.3s, background-color 0.3s;
    }

    select {
      width: 80%;
      padding: 5px;
      margin-top: 10px;
      border-radius: 5px;
      border: 1px solid #ccc;
    }
  </style>
</head>
<body>
  <h1>IoT Dashboard</h1>

  <div class="dashboard">
    <!-- Toggle Switch -->
    <div class="card">
      <h3>Feature Toggle</h3>
      <label class="toggle-switch">
        <input type="checkbox" id="toggleSwitch">
        <span class="slider"></span>
      </label>
      <p id="toggleState">Toggle is OFF</p>
    </div>

    <!-- Temperature Gauge -->
    <div class="card">
      <h3>Temperature (°C)</h3>
      <div class="progress-bar-container">
        <div id="temperatureBar" class="progress-bar"></div>
      </div>
      <p id="temperatureValue">25°C</p>
    </div>

    <!-- Gas Gauge -->
    <div class="card">
      <h3>Gas (PPM)</h3>
      <div class="progress-bar-container">
        <div id="gasBar" class="progress-bar"></div>
      </div>
      <p id="gasValue">200 PPM</p>
    </div>

    <!-- Ultrasonic Sensor -->
    <div class="card">
      <h3>Ultrasonic Level</h3>
      <select id="ultrasonicLevel">
        <option value="Empty">Empty</option>
        <option value="Warning">Warning</option>
        <option value="Spilling">Spilling</option>
      </select>
      <p id="ultrasonicValue">Status: Empty</p>
    </div>
  </div>

  <script>
    // Toggle Switch Logic
    const toggleSwitch = document.getElementById('toggleSwitch');
    const toggleState = document.getElementById('toggleState');

    toggleSwitch.addEventListener('change', () => {
      toggleState.textContent = toggleSwitch.checked ? 'Toggle is ON' : 'Toggle is OFF';
    });

    // Update Temperature Bar
    const temperatureBar = document.getElementById('temperatureBar');
    const temperatureValue = document.getElementById('temperatureValue');

    function updateTemperature(value) {
      temperatureValue.textContent = ${value}°C;
      temperatureBar.style.width = ${value}%;

      // Set color based on level
      if (value <= 33) {
        temperatureBar.style.backgroundColor = "#4CAF50"; // Green
      } else if (value <= 66) {
        temperatureBar.style.backgroundColor = "#FFC107"; // Yellow
      } else {
        temperatureBar.style.backgroundColor = "#F44336"; // Red
      }
    }
    updateTemperature(25); // Initialize with default value

    // Update Gas Bar
    const gasBar = document.getElementById('gasBar');
    const gasValue = document.getElementById('gasValue');

    function updateGas(value) {
      gasValue.textContent = ${value} PPM;
      gasBar.style.width = ${(value / 1000) * 100}%;

      // Set color based on level
      if (value <= 300) {
        gasBar.style.backgroundColor = "#4CAF50"; // Green
      } else if (value <= 700) {
        gasBar.style.backgroundColor = "#FFC107"; // Yellow
      } else {
        gasBar.style.backgroundColor = "#F44336"; // Red
      }
    }
    updateGas(200); // Initialize with default value

    // Ultrasonic Level Logic
    const ultrasonicLevel = document.getElementById('ultrasonicLevel');
    const ultrasonicValue = document.getElementById('ultrasonicValue');

    ultrasonicLevel.addEventListener('change', () => {
      ultrasonicValue.textContent = Status: ${ultrasonicLevel.value};
    });

    // Simulate live updates (Optional)
    setInterval(() => {
      updateTemperature(Math.floor(Math.random() * 101)); // Random temperature
      updateGas(Math.floor(Math.random() * 1001)); // Random gas value
    }, 5000); // Update every 5 seconds
  </script>
</body>
</html>
)rawliteral";

#endif
