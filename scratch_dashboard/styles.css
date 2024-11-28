/* General Styles */
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
