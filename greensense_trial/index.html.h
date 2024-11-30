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
  background-color: #f4f4f4;
  margin: 0;
  padding: 0;
}

.container {
  max-width: 600px;
  margin: 20px auto;
  text-align: center;
}

/* Buzzer Toggle */
.toggle-container {
  margin: 20px 0;
}

.toggle {
  position: relative;
  display: inline-block;
  width: 60px;
  height: 34px;
}

.toggle input {
  opacity: 0;
  width: 0;
  height: 0;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  transition: 0.4s;
  border-radius: 34px;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  transition: 0.4s;
  border-radius: 50%;
}

input:checked + .slider {
  background-color: #4caf50;
}

input:checked + .slider:before {
  transform: translateX(26px);
}

/* Trash Gauge (Vertical) */
.trash-gauge {
  margin: 20px 0;
}

.gauge {
  position: relative;
  width: 50px;
  height: 200px;
  background-color: #ddd;
  border-radius: 10px;
  overflow: hidden;
  margin: 10px auto;
}

.gauge .fill {
  position: absolute;
  bottom: 0;
  width: 100%;
  height: 0;
  background-color: green;
  transition: height 0.5s ease, background-color 0.5s ease;
}

/* Temperature Gauge */
.temperature-gauge {
  margin: 30px auto;
  width: 300px;
  text-align: center;
}

.temperature-gauge .gauge {
  position: relative;
  width: 100%;
  height: 20px;
  background-color: #ddd;
  border-radius: 10px;
  overflow: hidden;
}

.temperature-gauge .gauge .fill {
  height: 100%;
  position: absolute;
  width: 0;
  left: 50%; /* Start in the middle */
  transform-origin: center left;
  background-color: lightblue; /* Initial light blue */
  transition: width 0.5s ease, left 0.5s ease, background-color 0.5s ease;
}

/* Humidity Gauge */
.humidity-gauge {
  margin: 30px auto;
  width: 300px;
  text-align: center;
}

.humidity-gauge .gauge {
  position: relative;
  width: 100%;
  height: 20px;
  background-color: #ddd;
  border-radius: 10px;
  overflow: hidden;
}

.humidity-gauge .gauge .fill {
  height: 100%;
  width: 0;
  background-color: lightblue;
  transition: width 0.5s ease, background-color 0.5s ease;
}

/* Methane Gauge (Horizontal) */
.methane-gauge {
  margin: 30px auto;
  width: 300px;
  text-align: center;
}

.methane-gauge .gauge {
  position: relative;
  width: 100%;
  height: 20px;
  background-color: #ddd;
  border-radius: 10px;
  overflow: hidden;
}

.methane-gauge .gauge .fill {
  height: 100%;
  width: 0;
  background-color: lightblue; /* Initial color */
  transition: width 0.5s ease, background-color 0.5s ease;
}

#methaneText {
  font-size: 16px;
  margin-bottom: 10px;
}
  </style>
</head>
<body>
  <div class="container">
    <h1>IoT Dashboard</h1>

    <!-- Buzzer Toggle -->
    <div class="toggle-container">
      <label class="toggle">
        <input type="checkbox" id="buzzerToggle">
        <span class="slider"></span>
      </label>
      <p>Buzzer Control: <span id="buzzerStatus">OFF</span></p>
    </div>

    <!-- Trash Bin Level (Vertical Gauge) -->
    <div class="trash-gauge">
      <p id="trashLevelText">Trash Level: 0%</p>
      <div class="gauge">
        <div class="fill" id="trashFill"></div>
      </div>
    </div>

    <!-- Temperature Gauge -->
    <div class="temperature-gauge">
      <p id="temperatureText">Temperature: 0°C</p>
      <div class="gauge">
        <div class="fill" id="temperatureFill"></div>
      </div>
    </div>

    <!-- Humidity Gauge -->
    <div class="humidity-gauge">
      <p id="humidityText">Humidity: 0%</p>
      <div class="gauge">
        <div class="fill" id="humidityFill"></div>
      </div>
    </div>

    <!-- Methane Gauge (Horizontal) -->
    <div class="methane-gauge">
      <p id="methaneText">
        Methane Level: <span id="methanePPM">0</span> PPM (<span id="methanePercentage">0.0</span>%)
      </p>
      <div class="gauge">
        <div class="fill" id="methaneFill"></div>
      </div>
    </div>
  </div>
  <script>
  const buzzerToggle = document.getElementById("buzzerToggle");
const buzzerStatus = document.getElementById("buzzerStatus");

buzzerToggle.addEventListener("change", () => {
  buzzerStatus.textContent = buzzerToggle.checked ? "ON" : "OFF";
});

// Trash Gauge Logic
const trashFill = document.getElementById("trashFill");
const trashLevelText = document.getElementById("trashLevelText");

function updateTrashLevel(level) {
  trashLevelText.textContent = `Trash Level: ${level}%`;
  trashFill.style.height = `${level}%`;

  if (level <= 25) {
    trashFill.style.backgroundColor = "green";
  } else if (level <= 50) {
    trashFill.style.backgroundColor = "yellow";
  } else {
    trashFill.style.backgroundColor = "red";
  }
}

// Temperature Gauge Logic
const temperatureFill = document.getElementById("temperatureFill");
const temperatureText = document.getElementById("temperatureText");

function updateTemperature(temp) {
  temperatureText.textContent = `Temperature: ${temp}°C`;

  if (temp >= 0) {
    const fillPercent = (temp / 100) * 50; // Positive range (0 to 100 maps to 50% width to 100%)
    temperatureFill.style.left = "50%";
    temperatureFill.style.width = `${fillPercent}%`;

    // Gradient: light blue → orange → yellow → red
    let red, green, blue;
    if (temp <= 33) {
      // Light blue → orange
      red = Math.min(255, temp * 7.5); // Increase red
      green = 128 + temp * 3.5; // Increase green
      blue = 255 - temp * 7.5; // Decrease blue
    } else if (temp <= 66) {
      // Orange → yellow
      red = 255;
      green = 255 - (66 - temp) * 3.85; // Decrease green
      blue = 0;
    } else {
      // Yellow → red
      red = 255;
      green = 255 - (temp - 66) * 3.85; // Decrease green further
      blue = 0;
    }
    temperatureFill.style.backgroundColor = `rgb(${red}, ${green}, ${blue})`;
  } else {
    const fillPercent = (Math.abs(temp) / 100) * 50; // Negative range (0 to -100 maps to 50% width to 0%)
    temperatureFill.style.left = `${50 - fillPercent}%`; // Move leftward
    temperatureFill.style.width = `${fillPercent}%`;

    // Gradient: light blue → dark blue
    const red = 128 - Math.abs(temp) * 1.28; // Decrease red
    const green = 255 - Math.abs(temp) * 2.55; // Decrease green
    const blue = 255; // Full blue
    temperatureFill.style.backgroundColor = `rgb(${red}, ${green}, ${blue})`;
  }
}

// Simulate updates
setInterval(() => {
  updateTemperature(Math.floor(Math.random() * 201) - 100); // Random temperature from -100 to 100
}, 2000);

// Humidity Gauge Logic
const humidityFill = document.getElementById("humidityFill");
const humidityText = document.getElementById("humidityText");

function updateHumidity(humidity) {
  humidityText.textContent = `Humidity: ${humidity}%`;
  humidityFill.style.width = `${humidity}%`;
  humidityFill.style.backgroundColor = `rgb(${255 - humidity * 2.55}, ${255 - humidity * 1.8}, 255)`;
}

// Simulate updates
setInterval(() => {
  updateTrashLevel(Math.floor(Math.random() * 101));
  updateTemperature(Math.floor(Math.random() * 201) - 100);
  updateHumidity(Math.floor(Math.random() * 101));
}, 2000);

// Initial methane levels in PPM and percentage
let methanePPM = 0;
let methanePercentage = 0;

// Update methane level function
function updateMethaneLevel() {
  // Simulate random methane levels between 100 and 10000 PPM
  methanePPM = Math.random() * (10000 - 100) + 100;  // Random methane level between 100 and 10000 PPM
  
  // Convert PPM to percentage (0.1% to 1%)
  methanePercentage = ((methanePPM - 100) / 9900) * (1 - 0.1) + 0.1;

  // Update the displayed PPM and percentage (formatted as a number, not multiple %)
  document.getElementById('methanePPM').textContent = methanePPM.toFixed(2); // Show methane PPM
  document.getElementById('methanePercentage').textContent = (methanePercentage).toFixed(2); // Show methane percentage

  // Update the gauge fill width based on percentage
  const fill = document.getElementById('methaneFill');
  fill.style.width = `${methanePercentage * 100}%`; // Adjust the width of the fill

  // Change color based on methane level
  if (methanePPM < 1000) {
    fill.style.backgroundColor = 'green'; // Safe levels (below 1000 ppm)
  } else if (methanePPM >= 1000 && methanePPM <= 5000) {
    fill.style.backgroundColor = 'orange'; // Cautionary levels (1000-5000 ppm)
  } else if (methanePPM > 5000 && methanePPM <= 10000) {
    fill.style.backgroundColor = 'red'; // Hazardous levels (5000-10000 ppm)
  }
}

// Call the update function every second
setInterval(updateMethaneLevel, 1000);
  </script>
</body>
</html>
  
)rawliteral";

#endif
