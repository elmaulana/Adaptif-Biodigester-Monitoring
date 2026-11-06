// index_html.h
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ADIGES Dashboard - Jimbung</title>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700;800&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.2/css/all.min.css">
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <div class="mobile-overlay" id="mobileOverlay"></div>
    <aside class="sidebar" id="sidebar">
        <div class="sidebar-logo">
            <div class="logo-icon"><i class="fa-solid fa-leaf"></i></div>
            <div><h2>ADIGES</h2><span>Smart Biodigester</span></div>
        </div>
        <nav class="sidebar-menu">
            <a href="#" class="menu-item active" onclick="showTab('dashboard')"><i class="fa-solid fa-chart-line"></i><span>Dashboard</span></a>
            <a href="#" class="menu-item" onclick="showTab('monitoring')"><i class="fa-solid fa-gauge-high"></i><span>Monitoring</span></a>
            <a href="#" class="menu-item" onclick="showTab('kontrol')"><i class="fa-solid fa-sliders"></i><span>Kontrol Perangkat</span></a>
            <a href="#" class="menu-item" onclick="showTab('riwayat')"><i class="fa-solid fa-clock-rotate-left"></i><span>Riwayat Data</span></a>
        </nav>
        <div class="sidebar-bottom">
            <div class="system-status">
                <span class="status-dot"></span>
                <div><strong>System Online</strong><small>ESP32 Connected</small></div>
            </div>
            <div class="location-info"><i class="fa-solid fa-location-dot"></i><span>Jimbung, Klaten</span></div>
        </div>
    </aside>

    <main class="main-content">
        <header class="topbar">
            <div class="topbar-left">
                <button class="mobile-menu-btn" id="mobileMenuBtn"><i class="fa-solid fa-bars"></i></button>
                <div><h1>ADIGES Dashboard</h1><p>Monitoring Biodigester - Jimbung</p></div>
            </div>
            <div class="connection-status">
                <span class="connection-dot"></span>
                <div><strong>Connected</strong><small>to ESP32</small></div>
            </div>
        </header>

        <!-- TAB DASHBOARD -->
        <section id="tab-dashboard" class="dashboard-content tab-content">
            <!-- Alert Banner -->
            <div id="alertBanner" class="alert-banner" style="display:none;">
                <i class="fa-solid fa-triangle-exclamation"></i>
                <span id="alertText">Alarm aktif!</span>
            </div>

            <div class="dashboard-heading">
                <div><h2>Overview Sistem</h2><p>Pantau kondisi biodigester secara real-time.</p></div>
                <div class="last-update"><i class="fa-regular fa-clock"></i> Last update: <span id="lastUpdate">--:--:--</span></div>
            </div>
            <div class="sensor-grid">
                <div class="sensor-card temperature-card">
                    <div class="sensor-card-header"><div class="sensor-icon"><i class="fa-solid fa-temperature-half"></i></div><span class="sensor-label">Suhu Reaktor</span></div>
                    <div class="sensor-value"><span id="temperatureValue">--</span><small>°C</small></div>
                    <div class="sensor-footer"><span class="status-badge normal" id="suhuBadge"><i class="fa-solid fa-circle-check"></i> Normal</span><span class="sensor-description">Target 30-34°C</span></div>
                </div>
                <div class="sensor-card ph-card">
                    <div class="sensor-card-header"><div class="sensor-icon"><i class="fa-solid fa-droplet"></i></div><span class="sensor-label">Tingkat pH</span></div>
                    <div class="sensor-value"><span id="phValue">--</span><small>pH</small></div>
                    <div class="sensor-footer"><span class="status-badge optimal"><i class="fa-solid fa-circle-check"></i> Optimal</span><span class="sensor-description">Target 6.5-7.5</span></div>
                </div>
                <div class="sensor-card pressure-card">
                    <div class="sensor-card-header"><div class="sensor-icon"><i class="fa-solid fa-cloud"></i></div><span class="sensor-label">Tekanan Gas</span></div>
                    <div class="sensor-value"><span id="pressureValue">--</span><small>MPa</small></div>
                    <div class="sensor-footer"><span class="status-badge normal" id="tekananBadge"><i class="fa-solid fa-circle-check"></i> Normal</span><span class="sensor-description">Batas 1.2 MPa</span></div>
                </div>
                <div class="sensor-card water-level-card">
                    <div class="sensor-card-header"><div class="sensor-icon"><i class="fa-solid fa-water"></i></div><span class="sensor-label">Level Air Tangki Gas</span></div>
                    <div class="water-level-content">
                        <div class="tank-container"><div class="tank"><div class="water" id="waterLevel" style="height: 0%;"><div class="water-wave"></div></div><div class="tank-mark mark-100">100%</div><div class="tank-mark mark-75">75%</div><div class="tank-mark mark-50">50%</div><div class="tank-mark mark-25">25%</div></div></div>
                        <div class="water-level-info"><div class="water-level-value"><span id="waterLevelValue">0</span><small>%</small></div><span class="status-badge normal" id="waterLevelStatus"><i class="fa-solid fa-circle-check"></i> Normal</span><p>Ketinggian air dalam tangki gas</p></div>
                    </div>
                </div>
            </div>

            <div class="main-grid">
                <!-- CHART -->
                <div class="panel chart-panel">
                    <div class="panel-header">
                        <div><h3>Riwayat Sensor</h3><p id="chartSubtitle">Suhu — 1 jam terakhir</p></div>
                        <button class="icon-btn"><i class="fa-solid fa-ellipsis"></i></button>
                    </div>
                    <div class="chart-tabs">
                        <button class="chart-tab-btn active" onclick="switchChart('suhu', this)">Suhu</button>
                        <button class="chart-tab-btn" onclick="switchChart('tekanan', this)">Tekanan</button>
                        <button class="chart-tab-btn" onclick="switchChart('ph', this)">pH</button>
                        <button class="chart-tab-btn" onclick="switchChart('level', this)">Level</button>
                    </div>
                    <div class="chart-container"><canvas id="sensorChart"></canvas></div>
                </div>

                <!-- KONTROL -->
                <div class="panel control-panel">
                    <div class="panel-header"><div><h3>Kontrol Perangkat</h3><p>Kendalikan aktuator biodigester</p></div><div class="control-icon"><i class="fa-solid fa-sliders"></i></div></div>

                    <!-- VALVE GAS -->
                    <div class="control-block">
                        <div class="control-block-header">
                            <div class="control-item-icon valve"><i class="fa-solid fa-faucet"></i></div>
                            <div class="control-block-title">
                                <strong>Katup Gas Utama</strong>
                                <span id="valveStatus">Memuat...</span>
                            </div>
                        </div>
                        <div class="segmented-control">
                            <button class="seg-btn" id="valveAuto" onclick="setMode('valve', 'auto')">AUTO</button>
                            <button class="seg-btn" id="valveManual" onclick="setMode('valve', 'manual')">MANUAL</button>
                        </div>
                        <div class="toggle-row">
                            <span>Status Katup</span>
                            <label class="toggle"><input type="checkbox" id="gasValve" onchange="controlDevice('valve', this.checked)"><span class="toggle-slider"></span></label>
                        </div>
                    </div>

                    <!-- HEATING PAD -->
                    <div class="control-block">
                        <div class="control-block-header">
                            <div class="control-item-icon auto"><i class="fa-solid fa-fire"></i></div>
                            <div class="control-block-title">
                                <strong>Heating Pad</strong>
                                <span id="heaterStatus">Memuat...</span>
                            </div>
                        </div>
                        <div class="segmented-control">
                            <button class="seg-btn" id="heaterAuto" onclick="setMode('heater', 'auto')">AUTO</button>
                            <button class="seg-btn" id="heaterManual" onclick="setMode('heater', 'manual')">MANUAL</button>
                        </div>
                        <div class="toggle-row">
                            <span>Status Heater</span>
                            <label class="toggle"><input type="checkbox" id="heaterToggle" onchange="controlDevice('heater', this.checked)"><span class="toggle-slider"></span></label>
                        </div>
                    </div>

                    <!-- MOTOR PENGADUK -->
                    <div class="control-block">
                        <div class="control-block-header">
                            <div class="control-item-icon pump"><i class="fa-solid fa-rotate"></i></div>
                            <div class="control-block-title">
                                <strong>Motor Pengaduk</strong>
                                <span id="pumpStatus">Memuat...</span>
                            </div>
                        </div>
                        <div class="segmented-control">
                            <button class="seg-btn" id="motorAuto" onclick="setMode('motor', 'auto')">AUTO</button>
                            <button class="seg-btn" id="motorManual" onclick="setMode('motor', 'manual')">MANUAL</button>
                        </div>
                        <div class="toggle-row">
                            <span>Status Motor</span>
                            <label class="toggle"><input type="checkbox" id="circulationPump" onchange="controlDevice('motor', this.checked)"><span class="toggle-slider"></span></label>
                        </div>
                        <div class="control-note" id="motorAutoReturnNote" style="display:none;">
                            <i class="fa-solid fa-clock"></i> Auto-return ke otomatis dalam <span id="autoReturnTimer">2j 0m</span>
                        </div>
                    </div>

                    <!-- TOMBOL AKSI KHUSUS -->
                    <div class="action-buttons">
                        <button class="action-btn air-btn" id="btnIsiAir" onclick="isiAir()">
                            <i class="fa-solid fa-droplet"></i> Isi Air (<span id="counterAir">0</span>/3)
                        </button>
                        <button class="action-btn confirm-btn" onclick="showConfirmTabung()">
                            <i class="fa-solid fa-gas-pump"></i> Konfirmasi Tabung Baru
                        </button>
                    </div>

                    <!-- STATUS DOSING -->
                    <div class="dosing-status" id="dosingStatus" style="display:none;">
                        <i class="fa-solid fa-flask"></i> Dosing Aktif: EM4 + Molase
                    </div>

                    <div class="control-info"><i class="fa-solid fa-circle-info"></i><span>Klik AUTO/MANUAL untuk ganti mode. Toggle hanya aktif di mode MANUAL.</span></div>
                </div>
            </div>
        </section>

        <!-- TAB MONITORING -->
        <section id="tab-monitoring" class="dashboard-content tab-content" style="display:none;">
            <h2>Halaman Monitoring</h2>
            <p>Halaman ini sedang dalam pengembangan. Data sensor real-time sudah tersedia di Dashboard.</p>
        </section>

        <!-- TAB KONTROL -->
        <section id="tab-kontrol" class="dashboard-content tab-content" style="display:none;">
            <h2>Halaman Kontrol Perangkat</h2>
            <p>Halaman ini sedang dalam pengembangan. Kontrol aktuator sudah tersedia di Dashboard.</p>
        </section>

        <!-- TAB RIWAYAT -->
        <section id="tab-riwayat" class="dashboard-content tab-content" style="display:none;">
            <h2>Halaman Riwayat Data</h2>
            <p>Halaman ini sedang dalam pengembangan. Grafik riwayat sudah tersedia di Dashboard.</p>
        </section>

        <footer>
            <span>ADIGES - Adaptive Multimodal Sensor Biodigester</span>
            <span>© 2026</span>
        </footer>
    </main>

    <!-- MODAL KONFIRMASI TABUNG -->
    <div class="modal-overlay" id="modalTabung" style="display:none;">
        <div class="modal-box">
            <div class="modal-icon"><i class="fa-solid fa-gas-pump"></i></div>
            <h3>Konfirmasi Tabung Baru</h3>
            <p>Apakah Anda yakin tabung penampung sudah <strong>diganti dan terpasang dengan benar</strong>? Pastikan tidak ada kebocoran sebelum melanjutkan.</p>
            <div class="modal-actions">
                <button class="modal-btn cancel" onclick="closeModalTabung()">Batal</button>
                <button class="modal-btn confirm" onclick="confirmTabung()">Ya, Sudah Diganti</button>
            </div>
        </div>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="/script.js"></script>
</body>
</html>
)rawliteral";