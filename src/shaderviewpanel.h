
#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include "../../baseoneclib/qtlib/src/filebrowserwidget.h"
#include "../../baseoneclib/qtlib/src/filewatcher.h"
#include <QtCore/QFileSystemWatcher>
#include <QtWidgets/QSpinBox>
#include "openglwidget.h"


class ShaderViewPanel : public QWidget {
public:
    ShaderViewPanel(QWidget* parent = nullptr);


private:
    void restoreLastShader();

    void onVertShaderFilenameChanged(const QString& filename);
    void onFragShaderFilenameChanged(const QString& filename);
    void onFileModified(const QString& path);

    void updateWatcherPaths();
    void onToggleAnimating();
    void onFrameUpdated();
    void updateUI();



    QVBoxLayout mLayout;
    QHBoxLayout mSettingsLayout;

    OpenGLWidget mGLWidget;
    FileBrowserWidget mFileBrowserVertShader;
    FileBrowserWidget mFileBrowserFragShader;

    QLabel mTimeLabel;
    QPushButton mResetTime;
    QPushButton mAnimate;
    QSpinBox mFps;

    QTimer mTimeLabelTimer;

    FileWatcher mFileWatcher;
};
