#ifndef __MODCONFIG_H__
#define __MODCONFIG_H__

// Identity for this mod, applied over the retail properties\partner.xml in
// SexyApp::InitPropertiesHook. ProdName picks the save folder
// (C:\ProgramData\PopCap Games\<ProdName>\userdata) and the single-instance mutex,
// so the mod never touches the retail game's settings or saves and can run beside it.
#define MOD_PROD_NAME			"InsaniquariumMod"
#define MOD_REGISTRY_KEY		"PopCap\\InsaniquariumMod"

// Start windowed until the player picks fullscreen in Options (saved to the registry).
#define MOD_DEFAULT_WINDOWED	true

#endif
