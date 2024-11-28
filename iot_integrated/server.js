const express = require('express');
const app = express();
const port = 3000;

// Simulated sensor data
let sensorData = {
  temperature: 25.5,
  humidity: 60,
  methane: 150,
  trashLevel: 45
};

// API route to get sensor data
app.get('/api/sensors', (req, res) => {
  // In a real app, you would get this data from actual sensors
  res.json(sensorData);
});

// Serve the front-end files
app.use(express.static('public'));

// Start the server
app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});
