// script_js.h
const char script_js[] PROGMEM = R"rawliteral(
// =========================================
// ADIGES DASHBOARD - REAL IOT DATA
// =========================================

const temperatureElement = document.getElementById("temperatureValue");
const phElement = document.getElementById("phValue");
const pressureElement = document.getElementById("pressureValue");
const waterLevelElement = document.getElementById("waterLevel");
const waterLevelValueElement = document.getElementById("waterLevelValue");
const waterLevelStatus = document.getElementById("waterLevelStatus");
const lastUpdateElement = document.getElementById("lastUpdate");
const chartSubtitle = document.getElementById("chartSubtitle");
const alertBanner = document.getElementById("alertBanner");
const alertText = document.getElementById("alertText");
const dosingStatus = document.getElementById("dosingStatus");
const motorAutoReturnNote = document.getElementById("motorAutoReturnNote");
const autoReturnTimer = document.getElementById("autoReturnTimer");
const counterAirEl = document.getElementById("counterAir");

let temperature = 0, ph = 0, pressure = 0, waterLevel = 0;
let activeChart = 'suhu';
let lastManualTime = 0;
let isMotorManualState = false;

// =========================================
// NAVIGASI TAB
// =========================================
function showTab(tabName) {
    document.querySelectorAll('.tab-content').forEach(el => el.style.display = 'none');
    document.getElementById('tab-' + tabName).style.display = 'block';
    document.querySelectorAll('.menu-item').forEach(el => el.classList.remove('active'));
    event.currentTarget.classList.add('active');
}

// =========================================
// UPDATE TIME
// =========================================
function updateTime() {
    const now = new Date();
    const time = now.toLocaleTimeString("id-ID", { hour: "2-digit", minute: "2-digit", second: "2-digit" });
    lastUpdateElement.textContent = time;
}

// =========================================
// UPDATE WATER LEVEL
// =========================================
function updateWaterLevelUI(level) {
    level = Math.max(0, Math.min(100, level));
    waterLevelElement.style.height = `${level}%`;
    waterLevelValueElement.textContent = Math.round(level);
    if (level >= 30 && level <= 85) {
        waterLevelStatus.className = "status-badge normal";
        waterLevelStatus.innerHTML = `<i class="fa-solid fa-circle-check"></i> Normal`;
    } else if ((level >= 20 && level < 30) || (level > 85 && level <= 95)) {
        waterLevelStatus.className = "status-badge warning";
        waterLevelStatus.innerHTML = `<i class="fa-solid fa-triangle-exclamation"></i> Perhatian`;
    } else {
        waterLevelStatus.className = "status-badge danger";
        waterLevelStatus.innerHTML = `<i class="fa-solid fa-circle-exclamation"></i> Bahaya`;
    }
}

// =========================================
// FETCH DATA
// =========================================
function fetchData() {
    fetch('/data')
        .then(response => response.json())
        .then(data => {
            temperature = data.suhu;
            ph = data.ph;
            pressure = data.tekanan;
            waterLevel = data.level;

            temperatureElement.textContent = temperature.toFixed(1);
            phElement.textContent = ph.toFixed(1);
            pressureElement.textContent = pressure.toFixed(2);
            updateWaterLevelUI(waterLevel);
            updateTime();

            // Badge Suhu
            const suhuBadge = document.getElementById('suhuBadge');
            if (temperature > 38) {
                suhuBadge.className = "status-badge danger";
                suhuBadge.innerHTML = `<i class="fa-solid fa-circle-exclamation"></i> Panas!`;
            } else if (temperature < 30 || temperature > 34) {
                suhuBadge.className = "status-badge warning";
                suhuBadge.innerHTML = `<i class="fa-solid fa-triangle-exclamation"></i> Warning`;
            } else {
                suhuBadge.className = "status-badge normal";
                suhuBadge.innerHTML = `<i class="fa-solid fa-circle-check"></i> Normal`;
            }

            // Badge Tekanan
            const tekananBadge = document.getElementById('tekananBadge');
            if (pressure >= 1.5) {
                tekananBadge.className = "status-badge danger";
                tekananBadge.innerHTML = `<i class="fa-solid fa-circle-exclamation"></i> KRITIS`;
            } else if (pressure >= 1.2) {
                tekananBadge.className = "status-badge warning";
                tekananBadge.innerHTML = `<i class="fa-solid fa-triangle-exclamation"></i> Ganti Tabung`;
            } else {
                tekananBadge.className = "status-badge normal";
                tekananBadge.innerHTML = `<i class="fa-solid fa-circle-check"></i> Normal`;
            }

            // Alert Banner
            if (data.alarm_kritis) {
                alertBanner.style.display = 'flex';
                alertText.textContent = '🚨 KRITIS! Tekanan melebihi 1.5 MPa. Segera ganti tabung penampung!';
            } else if (data.alarm_tabung) {
                alertBanner.style.display = 'flex';
                alertText.textContent = '⚠️ Tabung penuh (≥1.2 MPa). Ganti tabung penampung!';
            } else if (data.alarm_overheat) {
                alertBanner.style.display = 'flex';
                alertText.textContent = '🔥 Overheat! Suhu melebihi 45°C. Sistem heater dimatikan.';
            } else {
                alertBanner.style.display = 'none';
            }

            // Update Toggle
            document.getElementById('gasValve').checked = data.relay_valve;
            document.getElementById('circulationPump').checked = data.relay_motor;
            document.getElementById('heaterToggle').checked = data.relay_heater;

            // Update Label Status
            document.getElementById('valveStatus').textContent = data.relay_valve ? "Katup terbuka" : "Katup tertutup";
            document.getElementById('pumpStatus').textContent = data.relay_motor ? "Motor aktif" : "Motor mati";
            document.getElementById('heaterStatus').textContent = data.relay_heater ? "Heater menyala" : "Heater mati";

            // Update Segmented Control
            updateSegmented('valveAuto', 'valveManual', data.is_valve_manual);
            updateSegmented('heaterAuto', 'heaterManual', data.is_heater_manual);
            updateSegmented('motorAuto', 'motorManual', data.is_motor_manual);

            // Update counter isi air
            counterAirEl.textContent = data.counter_isi_air;
            document.getElementById('btnIsiAir').disabled = (data.counter_isi_air >= 3 || data.isi_air_running);

            // Update dosing status
            if (data.dosing_running) {
                dosingStatus.style.display = 'flex';
            } else {
                dosingStatus.style.display = 'none';
            }

            // Update motor auto-return note
            if (data.is_motor_manual) {
                motorAutoReturnNote.style.display = 'flex';
                isMotorManualState = true;
            } else {
                motorAutoReturnNote.style.display = 'none';
                isMotorManualState = false;
            }

            updateChart();
        })
        .catch(error => console.error('Error:', error));
}

function updateSegmented(autoId, manualId, isManual) {
    const autoBtn = document.getElementById(autoId);
    const manualBtn = document.getElementById(manualId);
    if (isManual) {
        autoBtn.classList.remove('active-auto');
        manualBtn.classList.add('active-manual');
    } else {
        autoBtn.classList.add('active-auto');
        manualBtn.classList.remove('active-manual');
    }
}

// =========================================
// MODE SWITCH (AUTO / MANUAL)
// =========================================
function setMode(device, mode) {
    if (mode === 'auto') {
        controlDevice('auto_' + device, 1);
    } else {
        // Manual: set ke state saat ini
        const toggleId = device === 'valve' ? 'gasValve' : (device === 'heater' ? 'heaterToggle' : 'circulationPump');
        const state = document.getElementById(toggleId).checked;
        controlDevice(device, state);
    }
}

// =========================================
// KONTROL DEVICE
// =========================================
function controlDevice(device, state) {
    fetch(`/control?device=${device}&state=${state ? 1 : 0}`)
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(error => console.error('Error:', error));
}

// =========================================
// ISI AIR
// =========================================
function isiAir() {
    if (confirm('Yakin ingin mengisi air? Pastikan wadah siap menampung.')) {
        controlDevice('isi_air', 1);
    }
}

// =========================================
// MODAL KONFIRMASI TABUNG
// =========================================
function showConfirmTabung() {
    document.getElementById('modalTabung').style.display = 'flex';
}
function closeModalTabung() {
    document.getElementById('modalTabung').style.display = 'none';
}
function confirmTabung() {
    controlDevice('reset_tabung', 1);
    closeModalTabung();
    alert('✅ Tabung baru dikonfirmasi. Katup gas akan kembali normal.');
}

// =========================================
// CHART
// =========================================
const labels = [];
const suhuData = [];
const tekananData = [];
const phData = [];
const levelData = [];

for (let i = 60; i >= 0; i--) {
    labels.push(`${i}m`);
    suhuData.push(0);
    tekananData.push(0);
    phData.push(0);
    levelData.push(0);
}

const ctx = document.getElementById("sensorChart").getContext("2d");
const sensorChart = new Chart(ctx, {
    type: "line",
    data: {
        labels: labels,
        datasets: [{
            label: "Suhu (°C)",
            data: suhuData,
            borderColor: "#ef4444",
            backgroundColor: "rgba(239,68,68,0.06)",
            borderWidth: 2, tension: 0.4, pointRadius: 0, fill: true
        }]
    },
    options: {
        responsive: true, maintainAspectRatio: false,
        interaction: { mode: "index", intersect: false },
        plugins: { legend: { display: false } },
        scales: {
            x: { grid: { display: false }, ticks: { maxTicksLimit: 7, font: { size: 9 } } },
            y: { beginAtZero: true, min: 0, max: 100, ticks: { font: { size: 9 } }, grid: { color: "rgba(0,0,0,0.04)" } }
        }
    }
});

function switchChart(type, btnElement) {
    activeChart = type;
    document.querySelectorAll('.chart-tab-btn').forEach(btn => btn.classList.remove('active'));
    btnElement.classList.add('active');

    const subtitles = {
        'suhu': 'Suhu — 1 jam terakhir',
        'tekanan': 'Tekanan Gas — 1 jam terakhir',
        'ph': 'Tingkat pH — 1 jam terakhir',
        'level': 'Level Air — 1 jam terakhir'
    };
    chartSubtitle.textContent = subtitles[type];

    const configs = {
        'suhu': { label: "Suhu (°C)", data: suhuData, color: "#ef4444", bg: "rgba(239,68,68,0.06)" },
        'tekanan': { label: "Tekanan (MPa)", data: tekananData, color: "#0d9488", bg: "rgba(13,148,136,0.08)" },
        'ph': { label: "pH", data: phData, color: "#3b82f6", bg: "rgba(59,130,246,0.06)" },
        'level': { label: "Level (%)", data: levelData, color: "#10b981", bg: "rgba(16,185,129,0.06)" }
    };

    const cfg = configs[type];
    sensorChart.data.datasets[0].label = cfg.label;
    sensorChart.data.datasets[0].data = cfg.data;
    sensorChart.data.datasets[0].borderColor = cfg.color;
    sensorChart.data.datasets[0].backgroundColor = cfg.bg;
    sensorChart.update();
}

function updateChart() {
    suhuData.push(temperature); suhuData.shift();
    tekananData.push(pressure); tekananData.shift();
    phData.push(ph); phData.shift();
    levelData.push(waterLevel); levelData.shift();

    if (activeChart === 'suhu') sensorChart.data.datasets[0].data = suhuData;
    else if (activeChart === 'tekanan') sensorChart.data.datasets[0].data = tekananData;
    else if (activeChart === 'ph') sensorChart.data.datasets[0].data = phData;
    else if (activeChart === 'level') sensorChart.data.datasets[0].data = levelData;

    sensorChart.update("none");
}

// =========================================
// MOBILE SIDEBAR
// =========================================
const mobileMenuBtn = document.getElementById("mobileMenuBtn");
const sidebar = document.getElementById("sidebar");
const mobileOverlay = document.getElementById("mobileOverlay");

mobileMenuBtn.addEventListener("click", function() {
    sidebar.classList.add("open");
    mobileOverlay.classList.add("show");
});
mobileOverlay.addEventListener("click", function() {
    sidebar.classList.remove("open");
    mobileOverlay.classList.remove("show");
});

// =========================================
// INITIALIZE
// =========================================
updateTime();
setInterval(fetchData, 2000);
fetchData();
)rawliteral";