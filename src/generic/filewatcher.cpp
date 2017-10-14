#include "filewatcher.h"
#include <QtCore/QThread>

FileWatcher::FileWatcher() : QObject() {
    connect(&mWatcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::onFileChanged);

}

void FileWatcher::addFile(const QString& filename) {
    if (filename.isEmpty())
        return;

    QString file = absPath(filename);
    if (mFilenames.contains(file))
        return;
    mFilenames.append(file);
    updatePaths();
}

void FileWatcher::removeFile(const QString& filename) {
    if (filename.isEmpty())
        return;

    auto file = absPath(filename);
    if (!mFilenames.contains(file))
        return;
    mFilenames.removeOne(file);
    updatePaths();
}

void FileWatcher::clear() {
    mFilenames.clear();
    updatePaths();
}

void FileWatcher::onFileChanged(const QString& filename) {
    mTime.restart();
    while (mTime.elapsed() < 1000) {
        if (QFileInfo::exists(filename)) {
            fileModified(filename);
            break;
        }
        QThread::currentThread()->msleep(50);
    }
    updatePaths();
}

void FileWatcher::updatePaths() {
    for (const auto& file : mWatcher.files()) {
        if (!mFilenames.contains(file))
            mWatcher.removePath(file);
    }

    for (const auto& file : mFilenames) {
        if (!mWatcher.files().contains(file))
            mWatcher.addPath(file);
    }
}

QString FileWatcher::absPath(const QString& filename) {
    return QFileInfo(filename).canonicalFilePath();
}

