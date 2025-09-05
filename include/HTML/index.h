#ifndef INDEXPAGE_H
#define INDEXPAGE_H

const char indexPage[] PROGMEM = R"(
<!DOCTYPE html>
<html>
  <head>
    <title>ESP32 Fan & Temp Monitor</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <meta charset="UTF-8" />
    <style>
      body {
        font-family: Arial, sans-serif;
        text-align: center;
        margin: 0;
        padding: 20px;
        background-color: #f4f7f9;
        color: #333;
      }
      .container {
        max-width: 800px;
        margin: auto;
        padding: 20px;
        background-color: #fff;
        border-radius: 12px;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
      }
      h1,
      h2 {
        color: #2c3e50;
      }
      .data-card {
        margin: 15px 0;
        padding: 15px;
        border-radius: 8px;
        background-color: #ecf0f1;
      }
      .data-label {
        font-size: 1.2em;
        font-weight: bold;
        color: #34495e;
        width: 250px;
      }
      .data-value {
        font-size: 1.8em;
        font-weight: normal;
        color: #1abc9c;
        width: 138px;
        display: inline-flex;
        align-items: center;
      }
      .fan-group {
        margin-top: 20px;
        text-align: left;
      }
      .fan-group p {
        margin: 5px 0;
        display: flex;
        justify-content: space-between;
        align-items: center;
      }
      .fan-table {
        width: 100%;
        border-collapse: collapse;
        margin-top: 20px;
      }
      .fan-table th,
      .fan-table td {
        border: 1px solid #ccc;
        padding: 10px;
        text-align: center;
      }
      .fan-table th {
        background-color: #34495e;
        color: #fff;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>ESP32 Fan & Temp Monitor</h1>
      <div class="data-card">
        <span class="data-label">Temperature: </span>
        <span id="temperature-value" class="data-value">--.- °C</span>
      </div>
      <div class="data-card fan-group">
        <h2>Fan Speed & Target RPM</h2>
        <p>
          <span class="data-label">MB In Fan Speed: </span>
          <span id="in-fan-speed-percent" class="data-value">--%</span>
        </p>
        <p>
          <span class="data-label">MB Out Fan Speed: </span>
          <span id="out-fan-speed-percent" class="data-value">--%</span>
        </p>
        <p>
          <span class="data-label">Enclosure Fan Speed: </span>
          <span id="enclosure-fan-speed-percent" class="data-value">--%</span>
        </p>
      </div>
      <div class="data-card">
        <h2>Individual Fan RPM</h2>
        <table class="fan-table">
          <thead>
            <tr>
              <th>Fan</th>
              <th>Actual RPM</th>
              <th>Expected RPM</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>Motherboard In</td>
              <td id="fan-in-rpm">----</td>
              <td id="expected-rpm-in">----</td>
            </tr>
            <tr>
              <td>Motherboard Out</td>
              <td id="fan-out-rpm">----</td>
              <td id="expected-rpm-out">----</td>
            </tr>
            <tr>
              <td>Enclosure</td>
              <td id="fan-enclosure-rpm">----</td>
              <td id="expected-rpm-enclosure">----</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
    <script>
      function fetchData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            document.getElementById("temperature-value").innerHTML = data.motherboardTemperature.toFixed(2) + " °C";

            document.getElementById("in-fan-speed-percent").innerHTML = data.motherboardInFanSpeedPercent + "%";
            document.getElementById("out-fan-speed-percent").innerHTML = data.motherboardOutFanSpeedPercent + "%";
            document.getElementById("enclosure-fan-speed-percent").innerHTML = data.EnclosureFanSpeedPercent + "%";

            document.getElementById("fan-in-rpm").innerHTML = data.motherboardInFanRPM.toFixed(0);
            document.getElementById("fan-out-rpm").innerHTML = data.MotherboardOutFanRPM.toFixed(0);
            document.getElementById("fan-enclosure-rpm").innerHTML = data.EnclosureFanRPM.toFixed(0);

            document.getElementById("expected-rpm-in").innerHTML = data.smallFanExpectedRPM.toFixed(0);
            document.getElementById("expected-rpm-out").innerHTML = data.smallFanExpectedRPM.toFixed(0);

            document.getElementById("expected-rpm-enclosure").innerHTML = data.largeFanExpectedRPM.toFixed(0);
          }
        };
        xhttp.open("GET", "/data", true);
        xhttp.send();
      }
      setInterval(fetchData, 1000); // Fetch new data every second
    </script>
  </body>
</html>
)";

#endif
