const topBar = new Panel;
topBar.location = "top";
topBar.height = Math.round(gridUnit * 1.5);
topBar.hiding = "normal";
topBar.floating = false;
topBar.floatingApplets = true;
topBar.opacity = "translucent";

topBar.addWidget("org.sonicde.sonicmenu");
topBar.addWidget("org.kde.plasma.appmenu");
topBar.addWidget("org.kde.plasma.panelspacer");
topBar.addWidget("org.kde.plasma.systemtray");
const digitalclock = topBar.addWidget("org.kde.plasma.digitalclock");
digitalclock.currentConfigGroup = ["Appearance"];
digitalclock.writeConfig("customDateFormat", "ddd-d");
digitalclock.writeConfig("dateFormat", "custom");
topBar.addWidget("org.kde.plasma.showdesktop");

const dock = new Panel;
dock.location = "bottom";
dock.height = Math.round(gridUnit * 20 / 9);
dock.hiding = "normal";
dock.offset = 0;
dock.floating = false;
dock.lengthMode = "fit";
dock.alignment = "center";
dock.opacity = "translucent";

dock.addWidget("org.kde.plasma.kickoff");
dock.addWidget("org.kde.plasma.pager");
dock.addWidget("org.kde.plasma.icontasks");
dock.addWidget("org.kde.plasma.trash");

const kwinConfig = ConfigFile('kwinrc');

kwinConfig.group = 'Effect-overview';
kwinConfig.writeEntry('BorderActivate', '9'); //disable overview

kwinConfig.group = 'Effect-windowview';
kwinConfig.writeEntry('BorderActivateAll', '3'); //overview all desktops bottom-right
