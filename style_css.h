// style_css.h
const char style_css[] PROGMEM = R"rawliteral(
* { margin: 0; padding: 0; box-sizing: border-box; }
:root {
    --green: #10b981; --green-dark: #059669; --green-light: #d1fae5;
    --teal: #0d9488; --teal-light: #ccfbf1;
    --orange: #f59e0b; --orange-light: #fef3c7;
    --red: #ef4444; --red-light: #fee2e2;
    --blue: #3b82f6; --blue-light: #dbeafe;
    --bg: #f5f7f9; --white: #ffffff;
    --text: #17211b; --text-secondary: #66736b; --text-muted: #9aa59e;
    --border: #e6ebe8;
    --shadow: 0 4px 20px rgba(15, 23, 42, 0.05);
    --shadow-hover: 0 12px 30px rgba(15, 23, 42, 0.09);
    --sidebar-width: 245px;
}
body { font-family: "Inter", sans-serif; background: var(--bg); color: var(--text); min-height: 100vh; }

.sidebar { width: var(--sidebar-width); height: 100vh; position: fixed; left: 0; top: 0; background: var(--white); border-right: 1px solid var(--border); padding: 25px 16px; display: flex; flex-direction: column; z-index: 1000; }
.sidebar-logo { display: flex; align-items: center; gap: 12px; padding: 0 10px 28px; border-bottom: 1px solid var(--border); }
.logo-icon { width: 43px; height: 43px; background: linear-gradient(135deg, var(--green), var(--teal)); color: white; border-radius: 12px; display: flex; align-items: center; justify-content: center; font-size: 20px; }
.sidebar-logo h2 { font-size: 18px; font-weight: 800; letter-spacing: 0.5px; }
.sidebar-logo span { display: block; font-size: 10px; color: var(--text-muted); margin-top: 3px; }
.sidebar-menu { margin-top: 28px; display: flex; flex-direction: column; gap: 7px; }
.menu-item { display: flex; align-items: center; gap: 13px; padding: 12px 14px; border-radius: 10px; text-decoration: none; color: var(--text-secondary); font-size: 13px; font-weight: 500; transition: 0.25s ease; }
.menu-item i { width: 20px; text-align: center; font-size: 15px; }
.menu-item:hover { background: #f0fdf8; color: var(--green-dark); transform: translateX(2px); }
.menu-item.active { background: var(--green-light); color: var(--green-dark); font-weight: 600; }
.sidebar-bottom { margin-top: auto; padding: 15px 10px 5px; }
.system-status { display: flex; gap: 10px; padding: 13px; background: #f6faf8; border-radius: 11px; margin-bottom: 14px; }
.status-dot { width: 9px; height: 9px; margin-top: 5px; border-radius: 50%; background: var(--green); box-shadow: 0 0 0 4px var(--green-light); animation: pulse 2s infinite; }
.system-status strong { display: block; font-size: 11px; }
.system-status small { display: block; font-size: 9px; color: var(--text-muted); margin-top: 3px; }
.location-info { color: var(--text-muted); font-size: 10px; display: flex; gap: 7px; padding: 0 5px; }

.main-content { margin-left: var(--sidebar-width); min-height: 100vh; }
.topbar { height: 82px; background: rgba(255,255,255,0.94); border-bottom: 1px solid var(--border); display: flex; align-items: center; justify-content: space-between; padding: 0 34px; position: sticky; top: 0; z-index: 500; backdrop-filter: blur(10px); }
.topbar h1 { font-size: 18px; font-weight: 700; }
.topbar p { font-size: 11px; color: var(--text-muted); margin-top: 4px; }
.topbar-left { display: flex; align-items: center; gap: 14px; }
.mobile-menu-btn { display: none; border: none; background: transparent; font-size: 20px; cursor: pointer; color: var(--text); }
.connection-status { display: flex; align-items: center; gap: 9px; padding: 8px 13px; background: #f0fdf8; border: 1px solid #d8f4e7; border-radius: 30px; }
.connection-dot { width: 8px; height: 8px; border-radius: 50%; background: var(--green); animation: pulse 1.8s infinite; }
.connection-status strong { display: block; font-size: 10px; color: var(--green-dark); }
.connection-status small { display: block; font-size: 9px; color: var(--text-muted); }

.dashboard-content { padding: 30px 34px 25px; max-width: 1600px; margin: auto; }
.dashboard-heading { display: flex; align-items: center; justify-content: space-between; margin-bottom: 22px; }
.dashboard-heading h2 { font-size: 21px; font-weight: 700; }
.dashboard-heading p { font-size: 12px; color: var(--text-secondary); margin-top: 5px; }
.last-update { font-size: 10px; color: var(--text-muted); display: flex; align-items: center; gap: 5px; }

/* ALERT BANNER */
.alert-banner { background: linear-gradient(135deg, #fef2f2, #fee2e2); border: 1.5px solid #ef4444; color: #b91c1c; padding: 14px 18px; border-radius: 12px; margin-bottom: 20px; display: flex; align-items: center; gap: 12px; font-weight: 600; font-size: 13px; animation: pulseAlert 1.5s infinite; }
@keyframes pulseAlert { 0%, 100% { box-shadow: 0 0 0 0 rgba(239,68,68,0.4); } 50% { box-shadow: 0 0 0 10px rgba(239,68,68,0); } }

/* SENSOR CARDS */
.sensor-grid { display: grid; grid-template-columns: repeat(4, minmax(0, 1fr)); gap: 17px; margin-bottom: 20px; }
.sensor-card { background: var(--white); border: 1px solid var(--border); border-radius: 15px; padding: 19px; min-height: 185px; box-shadow: var(--shadow); transition: transform 0.25s ease, box-shadow 0.25s ease; }
.sensor-card:hover { transform: translateY(-4px); box-shadow: var(--shadow-hover); }
.sensor-card-header { display: flex; align-items: center; justify-content: space-between; }
.sensor-icon { width: 39px; height: 39px; border-radius: 11px; display: flex; align-items: center; justify-content: center; font-size: 17px; }
.temperature-card .sensor-icon { color: #ef4444; background: #fee2e2; }
.ph-card .sensor-icon { color: var(--teal); background: var(--teal-light); }
.pressure-card .sensor-icon { color: var(--blue); background: var(--blue-light); }
.water-level-card .sensor-icon { color: var(--teal); background: var(--teal-light); }
.sensor-label { margin-left: auto; font-size: 11px; color: var(--text-secondary); font-weight: 500; }
.sensor-value { margin-top: 17px; font-size: 29px; font-weight: 700; letter-spacing: -1px; }
.sensor-value small { font-size: 12px; color: var(--text-secondary); font-weight: 500; margin-left: 3px; }
.sensor-footer { display: flex; align-items: center; justify-content: space-between; margin-top: 17px; }
.status-badge { display: inline-flex; align-items: center; gap: 5px; border-radius: 20px; padding: 5px 8px; font-size: 9px; font-weight: 600; }
.status-badge i { font-size: 7px; }
.status-badge.normal { background: var(--green-light); color: var(--green-dark); }
.status-badge.optimal { background: var(--teal-light); color: var(--teal); }
.status-badge.warning { background: var(--orange-light); color: #b45309; }
.status-badge.danger { background: var(--red-light); color: #dc2626; }
.sensor-description { color: var(--text-muted); font-size: 9px; }

/* WATER TANK */
.water-level-content { display: flex; align-items: center; gap: 18px; margin-top: 10px; }
.tank-container { height: 115px; width: 62px; display: flex; justify-content: center; position: relative; }
.tank { width: 49px; height: 108px; border: 3px solid #94a3b8; border-radius: 8px 8px 12px 12px; position: relative; overflow: hidden; background: #f8fafc; }
.water { position: absolute; bottom: 0; left: 0; width: 100%; background: linear-gradient(180deg, #2dd4bf, #0d9488); transition: height 1s ease; overflow: hidden; }
.water-wave { position: absolute; top: -7px; left: -15%; width: 130%; height: 14px; background: rgba(255,255,255,0.25); border-radius: 50%; animation: wave 3s linear infinite; }
.tank-mark { position: absolute; right: -38px; font-size: 7px; color: var(--text-muted); }
.mark-100 { top: -3px; } .mark-75 { top: 24%; } .mark-50 { top: 48%; } .mark-25 { top: 73%; }
.water-level-value { font-size: 25px; font-weight: 700; letter-spacing: -1px; }
.water-level-value small { font-size: 11px; color: var(--text-secondary); }
.water-level-info p { margin-top: 7px; font-size: 8px; line-height: 1.5; color: var(--text-muted); }

/* PANEL */
.main-grid { display: grid; grid-template-columns: minmax(0, 1.7fr) minmax(320px, 1fr); gap: 20px; margin-bottom: 20px; }
.panel { background: var(--white); border: 1px solid var(--border); border-radius: 15px; padding: 21px; box-shadow: var(--shadow); }
.panel-header { display: flex; justify-content: space-between; align-items: flex-start; }
.panel-header h3 { font-size: 14px; font-weight: 700; }
.panel-header p { color: var(--text-muted); font-size: 10px; margin-top: 5px; }
.icon-btn { width: 30px; height: 30px; border: none; border-radius: 8px; background: #f5f7f6; color: var(--text-muted); cursor: pointer; transition: 0.2s; }
.icon-btn:hover { background: var(--green-light); color: var(--green-dark); }
.chart-container { position: relative; height: 280px; margin-top: 20px; }

/* CHART TABS */
.chart-tabs { display: flex; gap: 8px; margin-top: 15px; margin-bottom: 10px; flex-wrap: wrap; }
.chart-tab-btn { padding: 8px 16px; border: 1.5px solid var(--border); background: #f8fafc; border-radius: 8px; font-size: 11px; font-weight: 600; color: var(--text-secondary); cursor: pointer; transition: all 0.25s ease; }
.chart-tab-btn:hover { background: var(--green-light); color: var(--green-dark); border-color: var(--green); }
.chart-tab-btn.active { background: var(--green); color: white; border-color: var(--green); box-shadow: 0 2px 8px rgba(16, 185, 129, 0.3); }

/* CONTROL BLOCKS */
.control-icon { width: 32px; height: 32px; border-radius: 9px; background: var(--green-light); color: var(--green-dark); display: flex; align-items: center; justify-content: center; font-size: 13px; }
.control-block { padding: 16px 0; border-bottom: 1px solid var(--border); }
.control-block:last-of-type { border-bottom: none; }
.control-block-header { display: flex; align-items: center; gap: 11px; margin-bottom: 12px; }
.control-item-icon { width: 34px; height: 34px; border-radius: 9px; display: flex; align-items: center; justify-content: center; font-size: 13px; flex-shrink: 0; }
.control-item-icon.valve { color: #f59e0b; background: #fef3c7; }
.control-item-icon.pump { color: #3b82f6; background: #dbeafe; }
.control-item-icon.auto { color: var(--green-dark); background: var(--green-light); }
.control-block-title strong { display: block; font-size: 12px; }
.control-block-title span { display: block; margin-top: 3px; font-size: 10px; color: var(--text-muted); }

/* SEGMENTED CONTROL */
.segmented-control { display: flex; background: #f1f5f9; border-radius: 10px; padding: 4px; gap: 4px; margin-bottom: 10px; }
.seg-btn { flex: 1; padding: 8px; border: none; background: transparent; border-radius: 7px; font-size: 10px; font-weight: 700; color: var(--text-muted); cursor: pointer; transition: all 0.25s ease; letter-spacing: 0.5px; }
.seg-btn:hover { color: var(--text-secondary); }
.seg-btn.active-auto { background: #3b82f6; color: white; box-shadow: 0 2px 6px rgba(59,130,246,0.3); }
.seg-btn.active-manual { background: #f59e0b; color: white; box-shadow: 0 2px 6px rgba(245,158,11,0.3); }

/* TOGGLE ROW */
.toggle-row { display: flex; align-items: center; justify-content: space-between; padding: 6px 0; }
.toggle-row > span { font-size: 11px; color: var(--text-secondary); font-weight: 500; }
.toggle { position: relative; width: 44px; height: 24px; display: inline-block; }
.toggle input { display: none; }
.toggle-slider { position: absolute; inset: 0; cursor: pointer; background: #d1d5db; border-radius: 30px; transition: 0.3s; }
.toggle-slider::before { content: ""; position: absolute; width: 18px; height: 18px; left: 3px; top: 3px; border-radius: 50%; background: white; box-shadow: 0 2px 5px rgba(0,0,0,0.2); transition: 0.3s; }
.toggle input:checked + .toggle-slider { background: var(--green); }
.toggle input:checked + .toggle-slider::before { transform: translateX(20px); }
.toggle input:disabled + .toggle-slider { opacity: 0.4; cursor: not-allowed; }

/* CONTROL NOTE */
.control-note { margin-top: 8px; padding: 8px 10px; background: #eff6ff; border-radius: 8px; font-size: 10px; color: #1e40af; display: flex; align-items: center; gap: 6px; }

/* ACTION BUTTONS */
.action-buttons { display: flex; flex-direction: column; gap: 8px; margin-top: 15px; }
.action-btn { padding: 11px; border-radius: 10px; border: none; font-size: 11px; font-weight: 600; cursor: pointer; transition: all 0.25s ease; display: flex; align-items: center; justify-content: center; gap: 8px; }
.air-btn { background: #dbeafe; color: #1e40af; border: 1.5px solid #93c5fd; }
.air-btn:hover { background: #3b82f6; color: white; }
.air-btn:disabled { opacity: 0.5; cursor: not-allowed; }
.confirm-btn { background: white; color: var(--green-dark); border: 1.5px solid var(--green); }
.confirm-btn:hover { background: var(--green); color: white; }

/* DOSING STATUS */
.dosing-status { margin-top: 12px; padding: 10px 12px; background: linear-gradient(135deg, #fef3c7, #fde68a); border-radius: 9px; font-size: 11px; font-weight: 600; color: #92400e; display: flex; align-items: center; gap: 8px; animation: pulseDosing 2s infinite; }
@keyframes pulseDosing { 0%, 100% { opacity: 1; } 50% { opacity: 0.7; } }

.control-info { margin-top: 15px; padding: 11px; border-radius: 9px; background: #f7faf8; display: flex; gap: 8px; color: var(--text-muted); font-size: 9px; line-height: 1.5; }
.control-info i { color: var(--green); margin-top: 1px; }

/* MODAL */
.modal-overlay { position: fixed; inset: 0; background: rgba(0,0,0,0.5); backdrop-filter: blur(4px); z-index: 2000; display: flex; align-items: center; justify-content: center; padding: 20px; }
.modal-box { background: white; border-radius: 16px; padding: 28px; max-width: 420px; width: 100%; box-shadow: 0 20px 60px rgba(0,0,0,0.3); text-align: center; }
.modal-icon { width: 60px; height: 60px; border-radius: 50%; background: var(--green-light); color: var(--green-dark); display: flex; align-items: center; justify-content: center; font-size: 26px; margin: 0 auto 15px; }
.modal-box h3 { font-size: 18px; font-weight: 700; margin-bottom: 10px; }
.modal-box p { font-size: 13px; color: var(--text-secondary); line-height: 1.6; margin-bottom: 20px; }
.modal-actions { display: flex; gap: 10px; }
.modal-btn { flex: 1; padding: 12px; border-radius: 10px; border: none; font-size: 12px; font-weight: 700; cursor: pointer; transition: 0.2s; }
.modal-btn.cancel { background: #f1f5f9; color: var(--text-secondary); }
.modal-btn.cancel:hover { background: #e2e8f0; }
.modal-btn.confirm { background: var(--green); color: white; }
.modal-btn.confirm:hover { background: var(--green-dark); }

footer { display: flex; justify-content: space-between; padding: 20px 34px; border-top: 1px solid var(--border); color: var(--text-muted); font-size: 9px; }
.mobile-overlay { display: none; position: fixed; inset: 0; background: rgba(0,0,0,0.3); z-index: 900; }
@keyframes pulse { 0% { box-shadow: 0 0 0 0 rgba(16,185,129,0.5); } 70% { box-shadow: 0 0 0 7px rgba(16,185,129,0); } 100% { box-shadow: 0 0 0 0 rgba(16,185,129,0); } }
@keyframes wave { from { transform: translateX(0); } to { transform: translateX(20px); } }

@media (max-width: 1100px) {
    .sensor-grid { grid-template-columns: repeat(2, 1fr); }
    .main-grid { grid-template-columns: 1fr; }
}
@media (max-width: 768px) {
    :root { --sidebar-width: 0px; }
    .sidebar { width: 245px; transform: translateX(-100%); transition: transform 0.3s ease; }
    .sidebar.open { transform: translateX(0); }
    .mobile-overlay.show { display: block; }
    .main-content { margin-left: 0; }
    .mobile-menu-btn { display: block; }
    .topbar { padding: 0 18px; height: 70px; }
    .topbar h1 { font-size: 15px; }
    .dashboard-content { padding: 22px 17px; }
    .dashboard-heading { display: block; }
    .last-update { margin-top: 10px; }
    .sensor-grid { grid-template-columns: 1fr; gap: 13px; }
    .sensor-card { min-height: 170px; }
    .chart-container { height: 230px; }
    footer { padding: 18px; flex-direction: column; gap: 6px; }
    .modal-box { padding: 22px; }
}
@media (max-width: 450px) {
    .connection-status div { display: none; }
    .connection-status { width: 31px; height: 31px; justify-content: center; padding: 0; }
}
)rawliteral";