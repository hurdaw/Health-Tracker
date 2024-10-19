// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyCo8OPFkNwVBfNjs6uCNac5GqOeuG83_I0",
  authDomain: "tkt1-15e42.firebaseapp.com",
  databaseURL: "https://tkt1-15e42-default-rtdb.firebaseio.com",
  projectId: "tkt1-15e42",
  storageBucket: "tkt1-15e42.appspot.com",
  messagingSenderId: "722983577274",
  appId: "1:722983577274:web:e2af233816a09415d60483",
  measurementId: "G-D7QB571J6M",
  databaseURL: "https://tkt1-15e42-default-rtdb.firebaseio.com/",
};

// Initialize Firebase
// firebase.initializeApp(firebaseConfig);
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
// Kết nối Firebase và đọc dữ liệu từ đường dẫn
const db = getDatabase(app);
// var db = firebase.database();
//--- backup ---
// function goBack() {
//     window.history.back();
// }

// document.addEventListener('DOMContentLoaded', function() {
//     const ctx = document.getElementById('barChart').getContext('2d');

//     // Khởi tạo biểu đồ
//     const barChart = new Chart(ctx, {
//         type: 'bar',
//         data: {
//             labels: [], // Thời gian sẽ được cập nhật
//             datasets: [{
//                 label: 'Nhiệt độ cơ thể (°C)',
//                 data: [], // Dữ liệu sẽ được cập nhật
//                 backgroundColor: 'rgba(75, 192, 192, 0.5)',
//                 borderColor: 'rgba(75, 192, 192, 1)',
//                 borderWidth: 1
//             }]
//         },
//         options: {
//             responsive: true,
//             scales: {
//                 x: {
//                     title: {
//                         display: true,
//                         text: 'Thời gian'
//                     }
//                 },
//                 y: {
//                     title: {
//                         display: true,
//                         text: 'Nhiệt độ (°C)'
//                     },
//                     beginAtZero: false
//                 }
//             }
//         }
//     });

//     // Hàm để cập nhật dữ liệu biểu đồ
//     function updateChart() {
//         const now = new Date();
//         const currentTime = now.toLocaleTimeString();

//         // Thêm dữ liệu mới vào biểu đồ
//         if (barChart.data.labels.length >= 6) {
//             barChart.data.labels.shift(); // Xóa nhãn cũ nhất nếu vượt quá 6 tháng
//             barChart.data.datasets[0].data.shift(); // Xóa dữ liệu cũ nhất
//         }

//         barChart.data.labels.push(currentTime);
//         barChart.data.datasets[0].data.push(36.5 + Math.random() * 1); // Cập nhật nhiệt độ ngẫu nhiên

//         barChart.update();

//         // Cập nhật chỉ số sức khỏe bên cạnh
//         document.getElementById('temperature').innerText = `${(36.5 + Math.random() * 1).toFixed(1)}°C`;
//         document.getElementById('heartRate').innerText = `${Math.floor(Math.random() * 20) + 60} bpm`;
//         document.getElementById('bloodPressure').innerText = `${Math.floor(Math.random() * 40) + 110}/${Math.floor(Math.random() * 20) + 70} mmHg`;
//     }

//     // Cập nhật dữ liệu mỗi giây
//     setInterval(updateChart, 500); // Cập nhật mỗi 500 ms
// });
//-------- backup ----------
// import { getDatabase, ref, onValue } from "firebase/database";

// const db = getDatabase();
// const starCountRef = ref(db, 'posts/' + postId + '/starCount');
// onValue(starCountRef, (snapshot) => {
//   const data = snapshot.val();
//   updateStarCount(postElement, data);
// });
// var chartB = new Highcharts.Chart({
//     chart: { renderTo: "barChart" },
//     title: { text: "Nhiệt độ cơ thể" },
//     series: [
//       {
//         showInLegend: false,
//         data: [],
//       },
//     ],
//     plotOptions: {
//       line: { animation: false, dataLabels: { enabled: true } },
//     },
//     xAxis: {
//       type: "datetime",
//       dateTimeLabelFormats: { second: "%H:%M:%S" },
//     },
//     yAxis: {
//       title: { text: "Temperature (Ambient)" },
//     },
//     credits: { enabled: false },
//   });
// // Add new data points to the chart every second
// setInterval(function () {
//   var x = new Date().getTime(), // Current time
//     y = Math.random() * (40 - 30) + 30; // Generate random temperature between 30 and 40

//   // Round the temperature to one decimal place
//   y = parseFloat(y.toFixed(1));

//   // Add new point, shift the chart if there are more than 10 points
//   if (chartB.series[0].data.length >= 10) {
//     chartB.series[0].addPoint([x, y], true, true, true);
//   } else {
//     chartB.series[0].addPoint([x, y], true, false, true);
//   }

//   // Update the displayed temperature value
//   document.getElementById("temperature").textContent = y + "°C";
// }, 1000);

// Khởi tạo đồ thị với Highcharts
// var chartB = new Highcharts.Chart({
//   chart: { renderTo: "barChart" },
//   title: { text: "Nhiệt độ cơ thể" },
//   series: [
//     {
//       showInLegend: false,
//       data: [],
//     },
//   ],
//   plotOptions: {
//     line: { animation: false, dataLabels: { enabled: true } },
//   },
//   xAxis: {
//     type: "datetime",
//     dateTimeLabelFormats: { second: "%H:%M:%S" },
//   },
//   yAxis: {
//     title: { text: "Temperature (Ambient)" },
//   },
//   credits: { enabled: false },
// });

// onValue(tempRef, (snapshot) => {
//   const data = snapshot.val();

//   // Lấy thời gian hiện tại
//   var x = new Date().getTime();

//   // Chuyển đổi dữ liệu nhiệt độ từ Firebase
//   var y = parseFloat(data.temperature.toFixed(1)); // Giả sử dữ liệu nhiệt độ từ Firebase là 'temperature'

//   // Cập nhật đồ thị
//   if (chartB.series[0].data.length >= 10) {
//     chartB.series[0].addPoint([x, y], true, true, true);
//   } else {
//     chartB.series[0].addPoint([x, y], true, false, true);
//   }

//   // Cập nhật giá trị nhiệt độ hiển thị trên trang
//   document.getElementById("temperature").textContent = y + "°C";
// });
// const tempRef = ref(db, "sensor/objectTemp");objectTemp
import { getDatabase, ref, onValue } from "firebase/database";

const starCountRef = ref(db, "sensor/objectTemp");
onValue(starCountRef, (snapshot) => {
  const data = snapshot.val();
  updateStarCount(postElement, data);
});

db.ref("/sensor/objectTemp").on("value", function (snapshot) {
  var temp = snapshot.val();
  document.getElementById("temperature").innerText = temp;
  console.log(temp);
});
