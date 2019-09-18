#include "shapecraft/app/view/MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
    using namespace shapecraft;

    QApplication app(argc, argv);

    auto window = new MainWindow();
    window->show();
    return app.exec();
}
