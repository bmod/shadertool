#pragma once

#include <QString>
#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>

class OpenGLWidget : public QOpenGLWidget {
public:
    OpenGLWidget(QWidget* parent = nullptr);

    void setFragShader(const QString& filename);

    void setVertShader(const QString& filename);


protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;



private:
    QMatrix4x4 mProj;
    QOpenGLShaderProgram mProgram;
    QOpenGLContext mContext;

};