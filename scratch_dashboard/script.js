// Buzzer Toggle Logic
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


