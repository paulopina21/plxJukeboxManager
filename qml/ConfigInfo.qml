import QtQuick 2.2

QtObject {
  // These are used to scale fonts according to screen size
  property real _scaler: 400 + screenGeometry.width * screenGeometry.height * 0.00015 //711.04 @ 1080p
  function scalerSize(size){
    return _scaler * (size * 0.00141)// 1 = 1px @ 1080p
  }

  function loadSettings(){
    for (var i=0; i<application.databaseManager.catalogs().count; i++)
    {
      catalogConfig.push({ "source": application.databaseManager.catalogs().get(i).source, "outputPath": application.databaseManager.catalogs().get(i).outputPath, "autoRebuild": application.databaseManager.catalogs().get(i).autoRebuild});
    }
  }

  function saveSettings(){
    for (var i=0; i<application.databaseManager.catalogs().count; i++)
    {
      application.databaseManager.catalogs().get(i).source      = catalogConfig[i]["source"];
      application.databaseManager.catalogs().get(i).outputPath  = catalogConfig[i]["outputPath"];
      application.databaseManager.catalogs().get(i).autoRebuild = catalogConfig[i]["autoRebuild"];
      application.databaseManager.saveSettings();
    }
  }

  property var pwdConfig: { "plxPwd": "16777235167772351677723716777237167772341677723616777234167772366665",
                            "usrPwd": "16777237167772371677723716777237167772371677723716777237167772371677723716777237"}

  property var catalogConfig: [];
}
