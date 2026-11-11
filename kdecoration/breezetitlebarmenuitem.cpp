/*
 * SPDX-FileCopyrightText: 2026 Joseph Crowell <joseph.w.crowell@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "breezetitlebarmenuitem.h"
#include "breezebutton.h"
#include "breezedecoration.h"

#include <KDecoration3/DecoratedWindow>
#include <KDecoration3/DecorationSettings>
#include <KDecoration3/TitleBarMenu>

#include <QPainter>

namespace Breeze {

TitleBarMenuItem::TitleBarMenuItem(KDecoration3::TitleBarMenu* menu)
    : KDecoration3::TitleBarMenuItem(menu)
{
}

void TitleBarMenuItem::paint(QPainter* painter, const QRectF& repaintArea)
{
    Q_UNUSED(repaintArea)
    auto d = qobject_cast<Decoration*>(decoration());
    if (!d) {
        KDecoration3::TitleBarMenuItem::paint(painter, repaintArea);
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Hover and pressed backgrounds in the Application menu button's colours.
    const DecorationButtonPalette* palette = d->decorationColors()->buttonPalette(DecorationButtonType::ApplicationMenu);
    // While a popup holds the pointer, hover states go stale; only the open item is highlighted.
    if (palette && ((isHovered() && !isDimmed()) || isPressed() || isChecked())) {
        const DecorationButtonPaletteGroup* group = d->window()->isActive() ? palette->active() : palette->inactive();
        const QColor background = (isPressed() || isChecked()) ? group->backgroundPress : group->backgroundHover;
        if (background.isValid()) {
            const QRectF rect = geometry();
            const qreal radius = qMin(d->scaledCornerRadius(), rect.height() / 4);
            painter->setPen(Qt::NoPen);
            painter->setBrush(background);
            painter->drawRoundedRect(rect, radius, radius);
        }
    }

    QColor color = d->fontColor();
    if (isDimmed()) {
        color.setAlphaF(color.alphaF() * 0.6);
    }
    painter->setFont(d->settings()->font());
    painter->setPen(color);
    painter->drawText(geometry(), Qt::AlignCenter | Qt::TextSingleLine, text());
    painter->restore();
}

KDecoration3::TitleBarMenuItem* TitleBarMenuFactory::createItem(KDecoration3::TitleBarMenu* menu)
{
    return new TitleBarMenuItem(menu);
}

KDecoration3::DecorationButton* TitleBarMenuFactory::createOverflowButton(KDecoration3::TitleBarMenu* menu)
{
    if (auto d = qobject_cast<Decoration*>(menu->decoration())) {
        return Button::createTitleBarMenuButton(Button::TitleBarMenuIcon::Overflow, d, menu);
    }
    return KDecoration3::TitleBarMenuFactory::createOverflowButton(menu);
}

KDecoration3::DecorationButton* TitleBarMenuFactory::createSearchButton(KDecoration3::TitleBarMenu* menu)
{
    if (auto d = qobject_cast<Decoration*>(menu->decoration())) {
        return Button::createTitleBarMenuButton(Button::TitleBarMenuIcon::Search, d, menu);
    }
    return KDecoration3::TitleBarMenuFactory::createSearchButton(menu);
}

}

#include "moc_breezetitlebarmenuitem.cpp"
