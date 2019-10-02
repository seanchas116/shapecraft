#include "shapecraft/app/state/WindowState.hpp"
#include "shapecraft/app/view/MainWindow.hpp"
#include "shapecraft/document/BottleShapeNode.hpp"
#include "shapecraft/document/BoxShapeNode.hpp"
#include "shapecraft/document/history/History.hpp"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
    using namespace shapecraft;

    auto history = std::make_shared<History>();
    Node::addPrototype(std::make_shared<BoxShapeNode>(history));
    Node::addPrototype(std::make_shared<BottleShapeNode>(history));

    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setDepthBufferSize(24);
    glFormat.setSamples(4);

#ifdef QT_DEBUG
    glFormat.setOption(QSurfaceFormat::DebugContext);
#endif

    // Disable vsync to improve response and do workaround for https://bugreports.qt.io/browse/QTBUG-46634?focusedCommentId=284506&page=com.atlassian.jira.plugin.system.issuetabpanels%3Acomment-tabpanel#comment-284506
    glFormat.setSwapInterval(0);

    QSurfaceFormat::setDefaultFormat(glFormat);

    auto window = new MainWindow(std::make_shared<WindowState>());
    window->show();
    return app.exec();
}
