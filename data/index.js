class IotSetting {
  constructor(
    WifiAccessPointName,
    WifiAccessPointPassword,
    WifiAccessPointIp,
    WifiSsid,
    WifiPassword,
    UpperTankEmptyDistance,
    UpperTankMinTriggerPercent,
    UpperTankMaxTriggerPercent,
    LowerTankEmptyDistance,
    LowerTankMinTriggerPercent,
    LowerTankMaxTriggerPercent,
    Motor1RunHour,
    Motor2RunHour,
    Motor1Auto,
    Motor2Auto,
    LightAuto,
    UpsAuto,
    PageRefreshSeconds) {
    this.WifiAccessPointName = WifiAccessPointName;
    this.WifiAccessPointPassword = WifiAccessPointPassword;
    this.WifiAccessPointIp = WifiAccessPointIp;
    this.WifiSsid = WifiSsid;
    this.WifiPassword = WifiPassword;
    this.UpperTankEmptyDistance = UpperTankEmptyDistance;
    this.UpperTankMinTriggerPercent = UpperTankMinTriggerPercent;
    this.UpperTankMaxTriggerPercent = UpperTankMaxTriggerPercent;
    this.LowerTankEmptyDistance = LowerTankEmptyDistance;
    this.LowerTankMinTriggerPercent = LowerTankMinTriggerPercent;
    this.LowerTankMaxTriggerPercent = LowerTankMaxTriggerPercent;
    this.Motor1RunHour = Motor1RunHour;
    this.Motor2RunHour = Motor2RunHour;
    this.Motor1Auto = Motor1Auto;
    this.Motor2Auto = Motor2Auto;
    this.LightAuto = LightAuto;
    this.UpsAuto = UpsAuto;
    this.PageRefreshSeconds = PageRefreshSeconds;
  }
}

class DashboardData {
  constructor(
    ServerTime,
    Motor1RunHour,
    Motor2RunHour,
    Motor1On,
    Motor2On,
    LightOn,
    UpsOn,
    LowerWaterLevelPercent,
    LowerWaterLevelDistance,
    UpperWaterLevelPercent,
    UpperWaterLevelDistance,
    WifiStatus,
    WifiSignalStrength) {
    this.ServerTime = ServerTime;
    this.Motor1RunHour = Motor1RunHour;
    this.Motor2RunHour = Motor2RunHour;
    this.Motor1On = Motor1On;
    this.Motor2On = Motor2On;
    this.LightOn = LightOn;
    this.UpsOn = UpsOn;
    this.LowerWaterLevelPercent = LowerWaterLevelPercent;
    this.LowerWaterLevelDistance = LowerWaterLevelDistance;
    this.UpperWaterLevelPercent = UpperWaterLevelPercent;
    this.UpperWaterLevelDistance = UpperWaterLevelDistance;
    this.WifiStatus = WifiStatus;
    this.WifiSignalStrength = WifiSignalStrength;
  }
}

let _settings = new IotSetting("", "", "", "", "", 0, 0, 0, 0, 0, 0, 0, 0, false, false, false, false, 2);
let _dashboardData = new DashboardData("", 0, 0, false, false, false, false, 0, 0, 0, 0, false, 0);
let _hostUrl = `http://device.local`;
let _enableTimer = true;

async function getSettings() {
  $.ajax({
    url: `${_hostUrl}/api/setting`,
    data: {
      reqType: "Motor"
    },
    type: "GET",
    dataType: "json",
    async: true,
  }).done(function (json) {
    _settings = new IotSetting("", "", "", "", "", 0, 0, 0, 0, 0, 0, 0, 0, false, false, false, false, 2);

    _settings.WifiAccessPointName = json.WifiAccessPointName;
    _settings.WifiAccessPointPassword = json.WifiAccessPointPassword;
    _settings.WifiAccessPointIp = json.WifiAccessPointIp;
    _settings.WifiSsid = json.WifiSsid;
    _settings.WifiPassword = json.WifiPassword;
    _settings.UpperTankEmptyDistance = json.UpperTankEmptyDistance;
    _settings.UpperTankMinTriggerPercent = json.UpperTankMinTriggerPercent;
    _settings.UpperTankMaxTriggerPercent = json.UpperTankMaxTriggerPercent;
    _settings.LowerTankEmptyDistance = json.LowerTankEmptyDistance;
    _settings.LowerTankMinTriggerPercent = json.LowerTankMinTriggerPercent;
    _settings.LowerTankMaxTriggerPercent = json.LowerTankMaxTriggerPercent;
    _settings.Motor1RunHour = json.Motor1RunHour;
    _settings.Motor2RunHour = json.Motor2RunHour;
    _settings.Motor1Auto = json.Motor1Auto;
    _settings.Motor2Auto = json.Motor2Auto;
    _settings.LightAuto = json.LightAuto;
    _settings.UpsAuto = json.UpsAuto;
    _settings.PageRefreshSeconds = json.PageRefreshSeconds;
    delayInSeconds = _settings.PageRefreshSeconds * 1000;
    setSettingUIElements(_settings);
  }).fail(function (xhr) {
    $("#ServerStatus").css("color", `red`);
    $("#ServerStatus").attr("title", xhr.statusText);
    console.error(`Error: ${xhr.statusText}`);
    // console.dir(xhr.responseText);
  })
}

async function saveSettings() {
  hideMessageBox();

  _settings.WifiAccessPointName = $("#WifiAccessPointName").val();
  _settings.WifiAccessPointPassword = $("#WifiAccessPointPassword").val();
  _settings.WifiAccessPointIp = $("#WifiAccessPointIp").val();
  _settings.WifiSsid = $("#WifiSsid").val();
  _settings.WifiPassword = $("#WifiPassword").val();

  _settings.UpperTankEmptyDistance = $("#UpperTankEmptyDistance").val();
  _settings.UpperTankMinTriggerPercent = $("#UpperTankMinTriggerPercent").val();
  _settings.UpperTankMaxTriggerPercent = $("#UpperTankMaxTriggerPercent").val();

  _settings.LowerTankEmptyDistance = $("#LowerTankEmptyDistance").val();
  _settings.LowerTankMinTriggerPercent = $("#LowerTankMinTriggerPercent").val();
  _settings.LowerTankMaxTriggerPercent = $("#LowerTankMaxTriggerPercent").val();

  _settings.Motor1RunHour = $('#Motor1RunHour').val();
  _settings.Motor2RunHour = $('#Motor2RunHour').val();

  _settings.Motor1Auto = $("#Motor1AutoSwitch").prop("checked");
  _settings.Motor2Auto = $("#Motor2AutoSwitch").prop("checked");

  _settings.LightAuto = $("#LightAutoSwitch").prop("checked");
  _settings.UpsAuto = $("#UpsAutoSwitch").prop("checked");

  _settings.PageRefreshSeconds = $('#PageRefreshSeconds').val();
  showSpinner("Updating...");
  _enableTimer = false;
  $.ajax({
    url: `${_hostUrl}/api/setting`,
    data: JSON.stringify(_settings),
    type: "POST",
    contentType: 'application/json',
    async: true,
  }).done(function (json) {
    _enableTimer = true;
    showMessageBox("Settings Saved!");
    hideSpinner();
  }).fail(function (xhr) {
    _enableTimer = true;
    showMessageBox(`Error: ${xhr.statusText}`)
    hideSpinner();
  })
}

function setSettingUIElements(setting) {
  if (!setting) {
    return;
  }

  $('#WifiAccessPointName').val(setting.WifiAccessPointName);
  $('#WifiAccessPointPassword').val(setting.WifiAccessPointPassword);
  $('#WifiAccessPointIp').val(setting.WifiAccessPointIp);
  $('#WifiSsid').val(setting.WifiSsid);
  $('#WifiPassword').val(setting.WifiPassword);

  $("#UpperTankEmptyDistance").val(setting.UpperTankEmptyDistance.toFixed(0));
  $("#UpperTankMinTriggerPercent").val(setting.UpperTankMinTriggerPercent.toFixed(0));
  $("#UpperTankMaxTriggerPercent").val(setting.UpperTankMaxTriggerPercent.toFixed(0));

  $("#LowerTankEmptyDistance").val(setting.LowerTankEmptyDistance.toFixed(0));
  $("#LowerTankMinTriggerPercent").val(setting.LowerTankMinTriggerPercent.toFixed(0));
  $("#LowerTankMaxTriggerPercent").val(setting.LowerTankMaxTriggerPercent.toFixed(0));

  $('#Motor1RunHour').val(setting.Motor1RunHour);
  $('#Motor2RunHour').val(setting.Motor2RunHour);

  $('#PageRefreshSeconds').val(setting.PageRefreshSeconds);

  if (setting.Motor1Auto) {
    $('#Motor1AutoSwitch').prop('checked', true);
  }
  if (setting.Motor2Auto) {
    $('#Motor2AutoSwitch').prop('checked', true);
  }
  if (setting.LightAuto) {
    $('#LightAutoSwitch').prop('checked', true);
  }
  if (setting.UpsAuto) {
    $('#UpsAutoSwitch').prop('checked', true);
  }
}

async function getDashboardData() {
  $("#ServerStatus").css("color", `lightgrey`);
  $("#ServerStatus").attr("title", `Not Connected`);

  $.ajax({
    url: `${_hostUrl}/api/dashboard-data`,
    data: {
      reqType: "Motor"
    },
    type: "GET",
    dataType: "json",
    async: false,
  }).done(function (json) {
    $("#ServerStatus").css("color", `green`);
    $("#ServerStatus").attr("title", `Connected`);
    _dashboardData = new DashboardData("", 0, 0, false, false, false, false, 0, 0, 0, 0, false, 0);
    _dashboardData.ServerTime = json.ServerTime;
    _dashboardData.Motor1RunHour = json.Motor1RunHour;
    _dashboardData.Motor2RunHour = json.Motor2RunHour;
    _dashboardData.Motor1On = json.Motor1On;
    _dashboardData.Motor2On = json.Motor2On;
    _dashboardData.LightOn = json.LightOn;
    _dashboardData.UpsOn = json.UpsOn;
    _dashboardData.LowerWaterLevelPercent = json.LowerWaterLevelPercent;
    _dashboardData.LowerWaterLevelDistance = json.LowerWaterLevelDistance;
    _dashboardData.UpperWaterLevelPercent = json.UpperWaterLevelPercent;
    _dashboardData.UpperWaterLevelDistance = json.UpperWaterLevelDistance;
    _dashboardData.PageRefreshSeconds = json.PageRefreshSeconds;
    _dashboardData.WifiStatus = json.WifiStatus;
    _dashboardData.WifiSignalStrength = dbmToQuality(json.WifiSignalStrength);
    delayInSeconds = _settings.PageRefreshSeconds * 1000;
    setDashboardUIElements(_dashboardData);
  }).fail(function (xhr) {
    $("#ServerStatus").css("color", `red`);
    $("#ServerStatus").attr("title", xhr.statusText);
    console.error(`Error: ${xhr.statusText}`);
  })
}

function setDashboardUIElements(dashboardData) {
  if (!dashboardData) {
    return;
  }
  $("#ServerTime").html(dashboardData.ServerTime);
  $("#UpperWaterLevelPercent").html(`${dashboardData.UpperWaterLevelPercent}%`);
  $("#UpperWaterLevelPercentProgress").css("width", `${dashboardData.UpperWaterLevelPercent}%`);
  $("#UpperWaterLevelPercentProgress").attr("aria-valuenow", dashboardData.UpperWaterLevelPercent);
  $("#UpperWaterLevelDistance").html(`Distance: ${dashboardData.UpperWaterLevelDistance.toFixed(0)} CM`);

  $("#LowerWaterLevelPercent").html(`${dashboardData.LowerWaterLevelPercent}%`);
  $("#LowerWaterLevelPercentProgress").css("width", `${dashboardData.LowerWaterLevelPercent}%`);
  $("#LowerWaterLevelPercentProgress").attr("aria-valuenow", dashboardData.LowerWaterLevelPercent);
  $("#LowerWaterLevelDistance").html(`Distance: ${dashboardData.LowerWaterLevelDistance.toFixed(0)} CM`);

  $("#Motor1Status").css("color", `lightgrey`);
  $("#Motor2Status").css("color", `lightgrey`);
  $("#LightStatus").css("color", `lightgrey`);
  $("#UpsStatus").css("color", `lightgrey`);
  $("#WifiStatus").css("color", `lightgrey`);
  if (dashboardData.Motor1On) {
    $("#Motor1Status").css("color", `green`);
  }
  if (dashboardData.Motor2On) {
    $("#Motor2Status").css("color", `green`);
  }
  if (dashboardData.LightOn) {
    $("#LightStatus").css("color", `green`);
  }
  if (dashboardData.WifiStatus) {
    $("#WifiStatus").css("color", `green`);
  }
  $('#WifiSignalStrength').html(`WIFI (${dashboardData.WifiSignalStrength}%)`);
  $('#Motor1RunHourLabel').html(`MOTOR 1: ${dashboardData.Motor1RunHour}`);
  $('#Motor2RunHourLabel').html(`MOTOR 2: ${dashboardData.Motor2RunHour}`);
}

$(document).ready(async function () {
  let delayInSeconds = 2 * 1000;
  hideMessageBox();
  $("#Motor1AutoSwitch").click(async function () {
    await saveSettings();
  });
  $("#Motor2AutoSwitch").click(async function () {
    await saveSettings();
  });
  $("#LightAutoSwitch").click(async function () {
    await saveSettings();
  });
  $("#UpsAutoSwitch").click(async function () {
    await saveSettings();
  });
  $("#Motor1PowerSwitch").click(async function () {
    await saveSettings();
  });
  $("#Motor2PowerSwitch").click(async function () {
    await saveSettings();
  });
  $("#SaveMotorSettingButton").click(async function () {
    await saveSettings();
  });

  await getSettings();
  try {
    while (true) {
      if (_enableTimer) {
        await getDashboardData();
      }
      await delay(delayInSeconds);

    }
  }
  catch (err) {
    console.error("Error in getDashboardData: %o", err);
  }
  finally {

  }
});