#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include "openglwidget.h"


static const char* defaultVertexShader =
        "attribute highp vec4 posAttr;\n"
                "attribute lowp vec4 colAttr;\n"
                "varying lowp vec4 col;\n"
                "uniform highp mat4 matrix;\n"
                "void main() {\n"
                "   col = colAttr;\n"
                "   gl_Position = matrix * posAttr;\n"
                "}\n";

static const char* defaultFragmentShader =
        "varying lowp vec4 col;\n"
                "void main() {\n"
                "   gl_FragColor = col;\n"
                "}\n";

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);
}

void OpenGLWidget::setFragShader(const QString& filename) {
    qDebug() << "reloading " << filename;
}

void OpenGLWidget::setVertShader(const QString& filename) {
    qDebug() << "reloading" << filename;
}

void OpenGLWidget::initializeGL() {
    auto fn = QOpenGLContext::currentContext()->functions();
    fn->glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h) {
    mProj.setToIdentity();
    mProj.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
    {
        glColor3f(0.0, 0.0, 0.0);
        glVertex3f(-0.5, -0.5, 0.0);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-0.5, 0.5, 0.0);
    }
    glEnd();
}
