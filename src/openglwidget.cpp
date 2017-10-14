#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <assert.h>
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

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setOption(QSurfaceFormat::DebugContext);
    setFormat(format);

    setFramerate(260);
    mTime.start();
    connect(&mTimer, &QTimer::timeout, this, &OpenGLWidget::onTimer);

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mLastTime = mCurrentTime;
}

bool OpenGLWidget::setShaderFiles(const QString& vertShader, const QString& fragShader)
{
    mVertShaderFile = vertShader;
    mFragShaderFile = fragShader;
    mShaderDirty = true;

    return true;
}

void OpenGLWidget::resetTime()
{
    mCurrentTime = 0;
    mLastTime = QDateTime::currentMSecsSinceEpoch();
}

void OpenGLWidget::setFramerate(int fps)
{
    mTimer.setInterval(qRound(1.0f/(float)fps*1000.0f));
}

void OpenGLWidget::initializeGL()
{
    if (!mProgram.create()) {
        qWarning() << "Failed to create shader";
    }
    mFn = context()->functions();
    mFn->glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    mLogger.initialize();
    mLogger.startLogging();
    qDebug() << "GL Initialized";

    mTimer.start();


}

void OpenGLWidget::resizeGL(int w, int h)
{
    mProj.setToIdentity();
    mProj.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0);
}

void OpenGLWidget::paintGL()
{
    updateTime();







    updateShaders();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mProgram.bind()) {
        GLfloat vertices[] = {
                -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 1.0f,  1.0f, 0.0f,
                -1.0f,  1.0f, 0.0f,
        };
        mProgram.enableAttributeArray(mPosAttr);
        mProgram.setAttributeArray(mPosAttr, vertices, 3);

        GLfloat colors[] = {
                0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
        };
        mProgram.enableAttributeArray(mColAttr);
        mProgram.setAttributeArray(mColAttr, colors, 3);

        mProgram.setUniformValue(mMatrixUniform, mProj);
        mProgram.setUniformValue(mTimeUniform, currentTime());

        mProgram.setUniformValue(mResolutionUniform, QSizeF(size()));

        glDrawArrays(GL_QUADS, 0, 4);

        mProgram.disableAttributeArray(mPosAttr);
        mProgram.disableAttributeArray(mColAttr);

        mProgram.release();
    } else {
        qDebug() << "Failed to bind shader";
    }
    glEnd();

}


void OpenGLWidget::onTimer()
{
    update();
    updated();
}

bool OpenGLWidget::updateShaders()
{
    if (mShaderDirty) {
        mProgram.removeAllShaders();
        qDebug() << "Reloading shaders";
        if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, mVertShaderFile)) {
            qDebug() << "Failed to load: " << mVertShaderFile;
            qDebug() << mProgram.log();
            mShaderDirty = false;
            return false;
        }

        if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, mFragShaderFile)) {
            qDebug() << "Failed to load: " << mFragShaderFile;
            qDebug() << mProgram.log();
            mShaderDirty = false;
            return false;
        }

        if (!mProgram.link()) {
            mShaderDirty = false;
            return false;
        }
        mPosAttr = mProgram.attributeLocation("posAttr");
        mColAttr = mProgram.attributeLocation("colAttr");


        mResolutionUniform = mProgram.uniformLocation("res");
        mMatrixUniform = mProgram.uniformLocation("matrix");
        mTimeUniform = mProgram.uniformLocation("time");
        mShaderDirty = false;
    }


    return true;
}

long OpenGLWidget::updateTime()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (mIsAnimating) {
        qint64 delta = now - mLastTime;
        mCurrentTime = delta;
        return delta;
    }
    mLastTime = now;

    return 0;
}


