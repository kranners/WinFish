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

// HD tank. The game was built for 640x480; the mod runs at MOD_SCREEN_WIDTH x MOD_SCREEN_HEIGHT.
// The tank grows right and down while every sprite keeps its native pixel size, so
// right/bottom-anchored constants add MOD_EXTRA_WIDTH/HEIGHT and centered ones add half.
#define MOD_SCREEN_WIDTH		1920
#define MOD_SCREEN_HEIGHT		1080
#define MOD_EXTRA_WIDTH			(MOD_SCREEN_WIDTH - 640)
#define MOD_EXTRA_HEIGHT		(MOD_SCREEN_HEIGHT - 480)

// The 640x75 HUD menubar is centered at the top of the screen.
#define MOD_HUD_X				(MOD_EXTRA_WIDTH / 2)

// 640x480 menu screens and dialogs are centered on the screen.
#define MOD_MENU_X				(MOD_EXTRA_WIDTH / 2)
#define MOD_MENU_Y				(MOD_EXTRA_HEIGHT / 2)

// Tank backgrounds are scaled up MOD_BG_SCALE times at load and cropped from the top.
// MOD_BG_X/Y map a point on the original 640x480 background art to the screen;
// MOD_BG_SPRITE_X/Y place a w x h sprite so its bottom-center stays on the same art.
#define MOD_BG_SCALE			3
#define MOD_BG_CROP_Y			(480 * MOD_BG_SCALE - MOD_SCREEN_HEIGHT)
#define MOD_BG_X(x)				((x) * MOD_BG_SCALE)
#define MOD_BG_Y(y)				((y) * MOD_BG_SCALE - MOD_BG_CROP_Y)
#define MOD_BG_SPRITE_X(x, w)	(MOD_BG_X((x) + (w) / 2) - (w) / 2)
#define MOD_BG_SPRITE_Y(y, h)	(MOD_BG_Y((y) + (h)) - (h))

static_assert(640 * MOD_BG_SCALE >= MOD_SCREEN_WIDTH && 480 * MOD_BG_SCALE >= MOD_SCREEN_HEIGHT,
	"MOD_BG_SCALE must cover the screen");

// Saves made before the HD tank hold 640x480 movement bounds (every right edge was <= 560).
// Widen them when loading. Nimbus is the only object with a bottom-anchored top bound (320).
template <typename T>
inline void ModMigrateBounds(T& theXMax, T& theYMin, T& theYMax)
{
	if (theXMax >= 640)
		return;
	theXMax += MOD_EXTRA_WIDTH;
	theYMax += MOD_EXTRA_HEIGHT;
	if (theYMin > 300)
		theYMin += MOD_EXTRA_HEIGHT;
}

#endif
