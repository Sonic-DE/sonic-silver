/*
 * SPDX-FileCopyrightText: 2026 Joseph Crowell <joseph.w.crowell@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <KDecoration3/TitleBarMenuFactory>
#include <KDecoration3/TitleBarMenuItem>

namespace Breeze {
class Decoration;

//* One top-level menu in the titlebar, with Silver's text and button colours.
class TitleBarMenuItem : public KDecoration3::TitleBarMenuItem {
    Q_OBJECT
public:
    explicit TitleBarMenuItem(KDecoration3::TitleBarMenu* menu);

    void paint(QPainter* painter, const QRectF& repaintArea) override;
};

//* Creates the titlebar menu's buttons in Silver's style.
class TitleBarMenuFactory : public KDecoration3::TitleBarMenuFactory {
public:
    KDecoration3::TitleBarMenuItem* createItem(KDecoration3::TitleBarMenu* menu) override;
    KDecoration3::DecorationButton* createOverflowButton(KDecoration3::TitleBarMenu* menu) override;
    KDecoration3::DecorationButton* createSearchButton(KDecoration3::TitleBarMenu* menu) override;
};

}
