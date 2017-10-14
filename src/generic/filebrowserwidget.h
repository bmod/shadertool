#pragma once

#include <QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtCore/QSettings>

class FileBrowserWidget : public QWidget {
    Q_OBJECT
public:
    FileBrowserWidget(const QString& label);
    void setFileFilter(const QString& filter) { mFileFilter = filter; }
    const QString filename() { return mLineEdit.text(); }

    void setFilename(const QString& filename);

signals:
    void filenameChanged(const QString& filename);

private:
    void onBrowse();

    void onLineEditChanged();


    QString mFileFilter;
    QHBoxLayout mLayout;
    QLineEdit mLineEdit;
    QLabel mLabel;
    QPushButton mBrowseButton;
};