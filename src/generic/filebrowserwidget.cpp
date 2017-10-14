#include "filebrowserwidget.h"

FileBrowserWidget::FileBrowserWidget(const QString& label) : QWidget() {
    setLayout(&mLayout);
    mLayout.addWidget(&mLabel);
    mLayout.addWidget(&mLineEdit);
    mLayout.addWidget(&mBrowseButton);
    mLabel.setText(label);

    connect(&mLineEdit, &QLineEdit::editingFinished,
            this, &FileBrowserWidget::onLineEditChanged);
    connect(&mBrowseButton, &QPushButton::clicked,
            this, &FileBrowserWidget::onBrowse);
}

void FileBrowserWidget::setFilename(const QString& filename) {
    mLineEdit.setText(filename);
    filenameChanged(filename);
}

void FileBrowserWidget::onBrowse() {
    QString dir = QFileInfo(filename()).path();

    QString filename = QFileDialog::getOpenFileName(this, "Select File", "", mFileFilter);
    if (filename.isNull())
        return;

    mLineEdit.setText(filename);
    filenameChanged(mLineEdit.text());
}

void FileBrowserWidget::onLineEditChanged() {
    filenameChanged(mLineEdit.text());
}
