#pragma once

#include <QString>
#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QtGui/QOpenGLDebugLogger>
#include <QTimer>
#include <QTime>

class OpenGLWidget : public QOpenGLWidget {
Q_OBJECT
public:
    OpenGLWidget(QWidget* parent = nullptr);

    bool setShaderFiles(const QString& vertShader, const QString& fragShader);

    bool isAnimating()
    { return mIsAnimating; }

    void setAnimating(bool a)
    { mIsAnimating = a; }

    void resetTime();

    float currentTime()
    { return (float) mCurrentTime / 1000.0f; }

    void setFramerate(int fps);

signals:

    void updated();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    bool updateShaders();

    long updateTime();

private:
    void onTimer();


    bool mIsAnimating = true;

    int mPosAttr;
    int mColAttr;
    int mResolutionUniform;
    int mMatrixUniform;
    int mTimeUniform;

    QString mVertShaderFile;
    QString mFragShaderFile;
    bool mShaderDirty = true;

    QMatrix4x4 mProj;
    QOpenGLShaderProgram mProgram;
    QOpenGLDebugLogger mLogger;
    QOpenGLFunctions* mFn;

    QTimer mTimer;
    QTime mTime;
    qint64 mCurrentTime = 0;
    qint64 mLastTime = 0;
};