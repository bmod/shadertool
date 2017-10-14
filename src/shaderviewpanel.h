
#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <generic/filebrowserwidget.h>
#include <generic/filewatcher.h>
#include <QtCore/QFileSystemWatcher>
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


    QVBoxLayout mLayout;
    OpenGLWidget mGLWidget;
    FileBrowserWidget mFileBrowserVertShader;
    FileBrowserWidget mFileBrowserFragShader;

    FileWatcher mFileWatcher;
};
