// SPDX-License-Identifier: GPL-2.0-or-later

#include <QApplication>
#include <QDialog>
#include <QMenu>
#include <QPluginLoader>
#include <QRegularExpression>
#include <QStylePlugin>
#include <QTest>

class SilverNonX11Test : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void polishAndRender()
    {
        // A guarded framework call must not mask a style-side regression.
        QTest::failOnWarning(QRegularExpression(QStringLiteral(".*(requires an X11 connection|may only be used on X11).*")));
        QPluginLoader loader(QStringLiteral(SILVER_PLUGIN_FILE));
        auto plugin = qobject_cast<QStylePlugin*>(loader.instance());
        QVERIFY2(plugin, qPrintable(loader.errorString()));
        auto style = plugin->create(QStringLiteral("Silver"));
        QVERIFY(style);
        QApplication::setStyle(style);

        QDialog dialog;
        QMenu menu;
        menu.addAction(QStringLiteral("Crash report"));
        QWidget dragWindow;
        dragWindow.setAttribute(Qt::WA_X11NetWmWindowTypeDND);
        for (QWidget* widget : {static_cast<QWidget*>(&dialog), static_cast<QWidget*>(&menu), &dragWindow}) {
            widget->setAttribute(Qt::WA_TranslucentBackground);
            widget->resize(160, 80);
            widget->ensurePolished();
            widget->show();
            QCoreApplication::processEvents();
            QPixmap image(widget->size());
            image.fill(Qt::transparent);
            widget->render(&image);
            QVERIFY(!image.isNull());
            widget->hide();
        }
    }
};

QTEST_MAIN(SilverNonX11Test)

#include "silver_nonx11test.moc"
