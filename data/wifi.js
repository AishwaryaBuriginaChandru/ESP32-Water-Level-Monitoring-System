class WifiSetting {
  constructor(WifiSsid, WifiPassword) {
    this.WifiSsid = WifiSsid;
    this.WifiPassword = WifiPassword;
  }
}

let _settings = new WifiSetting("", "");
let _hostUrl = `http://cbk.local`;

async function getWifiData() {
  hideMessageBox();
  $.ajax({
    url: `${_hostUrl}/api/wifi-setting`,
    type: "GET",
    dataType: "json",
    async: false,
  }).done(function (json) {
    _settings = new WifiSetting("", "");
    _settings.WifiSsid = json.WifiSsid;
    _settings.WifiPassword = json.WifiPassword;
    setUIElements(_settings);
  }).fail(function (xhr, status, errorThrown) {
    showMessageBox(`Error: ${status}: ${errorThrown}`)
    console.dir(xhr);
  })
}

async function postWifiData() {
  hideMessageBox();
  _settings.WifiSsid = $("#WifiSsid").val();
  _settings.WifiPassword = $("#WifiPassword").val();
  showSpinner("Updating...");
  $.ajax({
    url: `${_hostUrl}/api/wifi-setting`,
    data: JSON.stringify(_settings),
    type: "POST",
    contentType: 'application/json',
    async: true,
  }).done(function (json) {
    delay(5000);
    restartServer();
    hideSpinner();
    window.location.reload();
    return Promise.resolve(json);
  }).fail(function (xhr, status, errorThrown) {
    hideSpinner();
    showMessageBox(`Error: ${status}: ${errorThrown}`)
    console.dir(xhr);
  })
}

function restartServer() {
  hideMessageBox();
  showSpinner("Restarting...");
  $.ajax({
    url: `${_hostUrl}/api/restart`,
    type: "POST",
    contentType: 'application/json',
    async: true,
  }).done(function (json) {
    delay(5000);
    hideSpinner();
    window.location.reload();
  }).fail(function (xhr, status, errorThrown) {
    hideSpinner();
    showMessageBox(`Error: ${status}: ${errorThrown}`)
    console.dir(xhr);
  })
}

function setUIElements(setting) {
  if (!setting) {
    return;
  }
  $("#WifiSsid").val(setting.WifiSsid);
  $("#WifiPassword").val(setting.WifiPassword);
}

$(document).ready(async function () {
  hideMessageBox();
  $("#ConnectButton").click(async function () {
    if (_settings) {
      await postWifiData();
      setUIElements(_settings);
    }
  });
  try {
    await getWifiData();
  }
  catch (err) {
    console.error("Error in getIotData: %o", err);
  }
});