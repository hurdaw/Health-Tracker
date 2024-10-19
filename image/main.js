// JavaScript
function updateTemperature() {
  // Tạo giá trị nhiệt độ ngẫu nhiên từ -10 đến 40
  const randomTemp = Math.floor(Math.random() * 51) - 10; // Giới hạn từ -10°C đến 40°C
  const temperatureDiv = document.getElementById("temperature");
  temperatureDiv.textContent = `Nhiệt độ hiện tại: ${randomTemp}°C`;
}
