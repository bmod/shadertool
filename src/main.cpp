#include "../../baseoneclib/qtlib/src/basewindow.h"

#include <qdebug.h>
#include <QtGui/QSurfaceFormat>
#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
#include "shaderviewpanel.h"


class MainWindow : public BaseWindow {
public:
    MainWindow() {
        addDock("Shader", &mShaderPanel);
    }

private:
    ShaderViewPanel mShaderPanel;
};


void initGL() {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
}

int main(int argc, char* argv[]) {
    QCoreApplication::setOrganizationDomain("vectorcycle.com");
    QCoreApplication::setApplicationName("ShaderTool");
    QApplication app(argc, argv);

    initGL();

    MainWindow window;

    window.show();

    return app.exec();
}