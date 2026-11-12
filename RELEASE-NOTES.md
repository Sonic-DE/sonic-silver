# Sonic Silver Release Notes

Silver is SonicDE's own visual theme and its default look, covering app styles, window borders, colour schemes, desktop themes, mouse cursors and the startup splash screen.

## Unreleased

- New global themes Silver Light - Apple Pie and Silver Dark - Apple Pie: a see-through top bar with the global menu, system tray and clock, a centered dock at the bottom, titlebar buttons on the left in the Ark style, and see-through menus and titlebars.
- The Silver Light and Silver Dark Bottom Panel and Left Panel themes now show the application's menus in the titlebar, next to the window menu, and draw the titlebar buttons with Fluent-style icons. The On All Desktops button is no longer in the titlebar.

- Switching from Silver to another app style no longer leaves the toolbar and menu bar area of open apps fully see-through until they are restarted.
- Behind the scenes: the Silver window decoration now declares that it supports see-through titlebars, so they are turned back on when you return to Silver.
- The Silver window decoration can show an application's menus right in the titlebar, at the place of the Application menu button, in Silver's own colours. Choose how in System Settings → Window Decorations → Configure Titlebar…. If you already have the Application menu button, a search button now sits next to it; it finds any menu item by name, and you can turn it off there.
- Menu transparency has moved from Silver's settings to the new Transparency page in System Settings → Colors & Themes, shared with the Breeze style. Your setting carries over.
- The plain "Silver" global theme is gone; Silver Light - Bottom Panel is the default. If you used it, you're moved to Silver Light - Bottom Panel, or Silver Dark - Bottom Panel with a dark colour scheme, when you next log in. The Silver splash screen is now listed under Silver Light - Bottom Panel in System Settings → Splash Screen.

## 6.8.2 (2026-11-10)

- The see-through title bar and toolbar settings (opacity, blur, opaque when maximized, matching the app colour) are now shared desktop-wide settings that other themes can use too, and modern apps follow them as well. They moved from Silver's settings to System Settings → Window Decorations → Configure Titlebar…, together with the per-application transparency exceptions; Silver's own exceptions keep their other settings. Your existing Silver settings carry over automatically. Applying a Silver preset sets these shared titlebar settings.
- Fixed see-through toolbars and menu bars in classic apps showing black instead of what's behind the window.
- The log out, restart and shut down screen now darkens the desktop with your theme's background colour at half strength, instead of black.
- Now presents itself with SonicDE names and identity throughout the desktop.
- Behind the scenes: the style's debug log now goes through SonicDE's logging system and is kept to at most 2 MiB.

## 6.8.0.1 (2026-10-23)

- Apps using the Silver style now start safely even when they run without a visible display, such as background or minimal tools.

## 6.8.0 (2026-10-20)

- Behind the scenes: version number update.

## 6.7.5 (2026-09-12)

- Behind the scenes: version number update.

## 6.7.4.9 (2026-08-31)

- A new SonicDE-branded splash screen greets you while your desktop starts.

## 6.7.4.8 (2026-08-31)

- Brand-new mouse cursors for both Silver Dark and Silver Light, with a cleaner busy animation.
- Behind the scenes: cursor build housekeeping.

## 6.7.4.7 (2026-08-28)

- Silver's blur effect now applies properly as soon as you first pick the theme in Settings.
- Silver can now write a troubleshooting log to help track down theme problems.

## 6.7.4.6 (2026-08-27)

- Behind the scenes: removed a title bar colour setting that had no effect on Silver's transparency.

## 6.7.4.5 (2026-08-26)

- Fixed doubled-up blur in apps that already set up their own blurred areas.

## 6.7.4.4 (2026-08-26)

- The Sonic Colorful Dark colour scheme is now installed and ready to choose.
- Behind the scenes: removed a duplicate colour scheme file.

## 6.7.4.3 (2026-08-25)

- The redundant "Silver Light - Bottom Panel" splash screen has been removed.

## 6.7.4.2 (2026-08-24)

- Behind the scenes: theme presets now always carry the current version number.

## 6.7.4.1 (2026-08-12)

- The new Silver wallpaper is now the default.

## 6.7.4 (2026-08-10)

- Behind the scenes: version number update.

## 6.7.3.2 (2026-08-07)

- Silver icons now come from the separate Silver icon theme, which is the base icon set for the whole system.

## 6.7.3.1 (2026-08-04)

- More theme elements have moved from the old Breeze look to Silver.

## 6.7.3 (2026-08-01)

- Silver is now the base and default theme in place of Breeze.
- New Sonic Colorful Dark colour scheme.
- Behind the scenes: code clean-up, improved code formatting and developer tooling.

## 6.6.0 (2026-03-24)

- New Silver Dark and Silver Light desktop themes that match the window borders and app styles, replacing the older Kite themes.
- The Silver Dark and Silver Light colour schemes now include transparency, so Silver's see-through look is applied.
- Style changes now apply to open apps straight away, without needing to close and reopen them.
- Desktop theme icons now use a blue accent.
- Blur behind panels and pop-ups looks cleaner now that the contrast effect no longer washes it out.
- Behind the scenes: removed display-server checks now that SonicDE always uses X11, plus documentation and preview image updates.

## 6.5.3 (2026-03-04)

- Silver branding has been restored throughout the theme, including the Silver Dark and Silver Light colour schemes.
- Behind the scenes: removed old Qt 5 support and added developer tooling support.

## 6.5.1 (2025-12-20)

- The global themes once again use Silver for their window borders and layouts.
- Behind the scenes: removed old Qt 5 support so the full theme builds and installs without it.

## 6.5 (2025-12-19)

- The theme is now Silver, SonicDE's own look.
- Behind the scenes: updated documentation and screenshots.
