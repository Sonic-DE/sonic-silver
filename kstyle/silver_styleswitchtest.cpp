// SPDX-License-Identifier: GPL-2.0-or-later

#include <QApplication>
#include <QFile>
#include <QMainWindow>
#include <QPluginLoader>
#include <QStyleFactory>
#include <QStylePlugin>
#include <QTemporaryDir>
#include <QTest>
#include <QToolBar>

// Switching away from Silver at run time must leave main windows as they were before Silver
// made their tools area translucent, or the next style leaves the header unpainted.
class SilverStyleSwitchTest : public QObject {
    Q_OBJECT

public:
    static void initMain()
    {
        static QTemporaryDir config;
        qputenv("XDG_CONFIG_HOME", QFile::encodeName(config.path()));
        QFile kdeglobals(config.filePath(QStringLiteral("kdeglobals")));
        if (kdeglobals.open(QIODevice::WriteOnly)) {
            kdeglobals.write("[Colors:Header]\nBackgroundNormal=222,224,226\nForegroundNormal=35,38,41\n\n"
                             "[ToolsArea]\nEnabled=true\nActiveOpacity=50\nInactiveOpacity=50\nApplyOpacityToHeader=true\n");
        }
    }

private Q_SLOTS:
    void initTestCase()
    {
        QPluginLoader loader(QStringLiteral(SILVER_PLUGIN_FILE));
        auto plugin = qobject_cast<QStylePlugin*>(loader.instance());
        QVERIFY2(plugin, qPrintable(loader.errorString()));
        m_plugin = plugin;
    }

    void init()
    {
        auto style = m_plugin->create(QStringLiteral("Silver"));
        QVERIFY(style);
        QApplication::setStyle(style);
    }

    void switchingStyleRestoresTranslucency_data()
    {
        QTest::addColumn<bool>("appTranslucent");
        QTest::newRow("opaque window") << false;
        QTest::newRow("window the app made translucent") << true;
    }

    void switchingStyleRestoresTranslucency()
    {
        QFETCH(bool, appTranslucent);
        QMainWindow window;
        window.setAttribute(Qt::WA_TranslucentBackground, appTranslucent);
        window.addToolBar(QStringLiteral("Main"))->addAction(QStringLiteral("New"));
        window.show();
        QVERIFY(QTest::qWaitForWindowExposed(&window));
        QVERIFY2(window.testAttribute(Qt::WA_TranslucentBackground), "Silver did not make the tools area translucent");

        QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
        QCOMPARE(window.testAttribute(Qt::WA_TranslucentBackground), appTranslucent);
        // Qt sets this along with WA_TranslucentBackground; left set, the header stays unpainted.
        QCOMPARE(window.testAttribute(Qt::WA_NoSystemBackground), appTranslucent);
    }

private:
    QStylePlugin* m_plugin = nullptr;
};

QTEST_MAIN(SilverStyleSwitchTest)

#include "silver_styleswitchtest.moc"
