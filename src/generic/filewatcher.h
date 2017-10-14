#pragma once

#include <QFileSystemWatcher>
#include <QFileInfo>
#include <qdebug.h>
#include <QtCore/QTime>

class FileWatcher : public QObject {
Q_OBJECT
public:
    FileWatcher();

    void addFile(const QString& filename);

    void removeFile(const QString& filename);

    void clear();

signals:
    void fileModified(const QString& filename);

private:
    void onFileChanged(const QString& filename);

    void updatePaths();


    QString absPath(const QString& filename);


    QStringList mFilenames;
    QFileSystemWatcher mWatcher;
    QTime mTime;
};