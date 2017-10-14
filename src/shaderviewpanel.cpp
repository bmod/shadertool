#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOption>
#include <QtGui/QFontDatabase>
#include "shaderviewpanel.h"

#define LAST_VERT_SHADER_FILE "lastVertShaderFile"
#define LAST_FRAG_SHADER_FILE "lastFragShaderFile"

ShaderViewPanel::ShaderViewPanel(QWidget* parent) : QWidget(parent),
                                                    mFileBrowserVertShader("Vert Shader"),
                                                    mFileBrowserFragShader("Frag Shader")

{
    setLayout(&mLayout);
    mLayout.setSpacing(2);
    mLayout.setContentsMargins(2, 2, 2, 2);
    mLayout.addWidget(&mGLWidget);

    mSettingsLayout.addWidget(&mTimeLabel);
    mSettingsLayout.addWidget(&mResetTime);
    mSettingsLayout.addWidget(&mAnimate);
    mSettingsLayout.addStretch(1);

    mLayout.addLayout(&mSettingsLayout);
    mLayout.addWidget(&mFileBrowserVertShader);
    mLayout.addWidget(&mFileBrowserFragShader);

    mTimeLabel.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    mTimeLabel.setText("Time");
    mTimeLabelTimer.setInterval(83);
    mTimeLabelTimer.start();

    QStyleOption op;
    mResetTime.setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward, &op));


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

    connect(&mAnimate, &QPushButton::clicked, this, &ShaderViewPanel::onToggleAnimating);
    connect(&mTimeLabelTimer, &QTimer::timeout, this, &ShaderViewPanel::onFrameUpdated);
    connect(&mResetTime, &QPushButton::clicked, [this]() {
        mGLWidget.resetTime();
    });

    restoreLastShader();
    updateUI();
}

void ShaderViewPanel::restoreLastShader()
{
    QSettings s;
    mFileBrowserVertShader.setFilename(s.value(LAST_VERT_SHADER_FILE).toString());
    mFileBrowserFragShader.setFilename(s.value(LAST_FRAG_SHADER_FILE).toString());
    onFileModified(QString());
}

void ShaderViewPanel::onFileModified(const QString& path)
{
    QFileInfo vertShaderFile(mFileBrowserVertShader.filename());
    QFileInfo fragShaderFile(mFileBrowserFragShader.filename());

    if (!vertShaderFile.exists()) {
        qWarning() << "File does not exist: " << vertShaderFile.absoluteFilePath();
        return;
    }
    if (!fragShaderFile.exists()) {
        qWarning() << "File does not exist: " << fragShaderFile.absoluteFilePath();
        return;
    }

    mGLWidget.setShaderFiles(vertShaderFile.absoluteFilePath(), fragShaderFile.absoluteFilePath());
}

void ShaderViewPanel::onVertShaderFilenameChanged(const QString& filename)
{
    mFileWatcher.removeFile(filename);

    if (!filename.isEmpty()) {
        QSettings s;
        s.setValue(LAST_VERT_SHADER_FILE, filename);
    }
    updateWatcherPaths();
}

void ShaderViewPanel::onFragShaderFilenameChanged(const QString& filename)
{
    updateWatcherPaths();

    if (!filename.isEmpty()) {
        QSettings s;
        s.setValue(LAST_FRAG_SHADER_FILE, filename);
    }
    updateWatcherPaths();
}

void ShaderViewPanel::updateWatcherPaths()
{
    mFileWatcher.clear();
    mFileWatcher.addFile(mFileBrowserVertShader.filename());
    mFileWatcher.addFile(mFileBrowserFragShader.filename());
}

void ShaderViewPanel::onToggleAnimating()
{
    mGLWidget.setAnimating(!mGLWidget.isAnimating());
    updateUI();
}

void ShaderViewPanel::onFrameUpdated()
{
    mTimeLabel.setText(QString("%1").arg(mGLWidget.currentTime(), 6, 'f', 2, ' '));
}



void ShaderViewPanel::updateUI()
{
    QStyleOption op;
    mAnimate.setIcon(style()->standardIcon(mGLWidget.isAnimating() ?
                                           QStyle::SP_MediaPause : QStyle::SP_MediaPlay, &op));

}

