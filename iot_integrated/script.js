// Function to update sensor data and progress bars
function fetchSensorData() {
    fetch('/api/sensors')
      .then(response => response.json())
      .then(data => {
        // Update Temperature
        document.getElementById('temperature-text').textContent = data.temperature;
        document.getElementById('temperature-bar').style.width = `${Math.min(data.temperature, 100)}%`;
  
        // Update Humidity
        document.getElementById('humidity-text').textContent = data.humidity;
        document.getElementById('humidity-bar').style.width = `${Math.min(data.humidity, 100)}%`;
  
        // Update Methane
        document.getElementById('methane-text').textContent = data.methane;
        document.getElementById('methane-bar').style.width = `${Math.min(data.methane / 10, 100)}%`;
  
        // Update Trash Level
        document.getElementById('trash-level-text').textContent = data.trashLevel;
        document.getElementById('trash-level-bar').style.height = `${Math.min(data.trashLevel, 100)}%`;
      })
      .catch(error => console.error('Error fetching sensor data:', error));
  }
  
  // Handle buzzer toggle switch
  document.getElementById('buzzer-switch').addEventListener('change', (event) => {
    const isChecked = event.target.checked;
    document.getElementById('buzzer-status').textContent = isChecked ? 'ON' : 'OFF';
  
    // Send buzzer status to the server (if applicable)
    fetch('/api/buzzer', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ buzzer: isChecked })
    }).catch(error => console.error('Error updating buzzer status:', error));
  });
  
  // Call fetchSensorData periodically
  setInterval(fetchSensorData, 5000);
  fetchSensorData();
  