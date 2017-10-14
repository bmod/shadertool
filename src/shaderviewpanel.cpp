#include "shaderviewpanel.h"

#define LAST_VERT_SHADER_FILE "lastVertShaderFile"
#define LAST_FRAG_SHADER_FILE "lastFragShaderFile"

ShaderViewPanel::ShaderViewPanel(QWidget* parent) : QWidget(parent),
                                                    mFileBrowserVertShader("Vert Shader"),
                                                    mFileBrowserFragShader("Frag Shader") {
    setLayout(&mLayout);
    mLayout.setSpacing(0);
    mLayout.setContentsMargins(2, 2, 2, 2);
    mLayout.addWidget(&mGLWidget);
    mLayout.addWidget(&mFileBrowserVertShader);
    mLayout.addWidget(&mFileBrowserFragShader);

    mFileBrowserVertShader.setObjectName("filebrowserVertexShader");
    mFileBrowserFragShader.setObjectName("filebrowserFragmentShader");

    mFileBrowserVertShader.setFileFilter("Vertex Shader (*.vert)");
    mFileBrowserFragShader.setFileFilter("Fragment Shader (*.frag)");

    mGLWidget.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(&mFileBrowserFragShader, &FileBrowserWidget::filenameChanged,
            this, &ShaderViewPanel::onFragShaderFilenameChanged);
    connect(&mFileBrowserVertShader, &FileBrowserWidget::filenameChanged,
            this, &ShaderViewPanel::onVertShaderFilenameChanged);

    connect(&mFileWatcher, &FileWatcher::fileModified,
            this, &ShaderViewPanel::onFileModified);

    restoreLastShader();
}

void ShaderViewPanel::restoreLastShader() {
    QSettings s;
    mFileBrowserVertShader.setFilename(s.value(LAST_VERT_SHADER_FILE).toString());
    mFileBrowserFragShader.setFilename(s.value(LAST_FRAG_SHADER_FILE).toString());
}

void ShaderViewPanel::onFileModified(const QString& path) {
    if (QFileInfo(path) == QFileInfo(mFileBrowserVertShader.filename()))
        mGLWidget.setFragShader(path);

    if (QFileInfo(path) == QFileInfo(mFileBrowserFragShader.filename()))
        mGLWidget.setVertShader(path);
}

void ShaderViewPanel::onVertShaderFilenameChanged(const QString& filename) {
    mFileWatcher.removeFile(filename);

    if (!filename.isEmpty()) {
        QSettings s;
        s.setValue(LAST_VERT_SHADER_FILE, filename);
    }
    updateWatcherPaths();
}

void ShaderViewPanel::onFragShaderFilenameChanged(const QString& filename) {
    updateWatcherPaths();

    if (!filename.isEmpty()) {
        QSettings s;
        s.setValue(LAST_FRAG_SHADER_FILE, filename);
    }
    updateWatcherPaths();
}

void ShaderViewPanel::updateWatcherPaths() {
    mFileWatcher.clear();
    mFileWatcher.addFile(mFileBrowserVertShader.filename());
    mFileWatcher.addFile(mFileBrowserFragShader.filename());
}

