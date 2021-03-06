#include "basewindow.h"
#include <QSettings>
#include <QApplication>

#define WIN_GEO "windowGeometry"
#define WIN_STATE "windowState"

BaseWindow::BaseWindow()  {
    setWindowTitle(QApplication::applicationName());
    setDockNestingEnabled(true);
    mWindowMenu = new QMenu("Window");
    menuBar()->addMenu(mWindowMenu);
}

QDockWidget* BaseWindow::addDock(const QString& name, QWidget* widget, Qt::DockWidgetArea area) {
    auto dock = new QDockWidget(this);
    dock->setObjectName(name);
    dock->setWidget(widget);
    dock->setWindowTitle(name);

    auto action = mWindowMenu->addAction(name);
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, &QAction::triggered, [dock, action]() {
        dock->setVisible(action->isChecked());
    });
    addDockWidget(area, dock);
    return dock;
}

void BaseWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    QSettings s;
    restoreGeometry(s.value(WIN_GEO).toByteArray());
    restoreState(s.value(WIN_STATE).toByteArray());
}

void BaseWindow::closeEvent(QCloseEvent* event) {
    QSettings s;
    s.setValue(WIN_STATE, saveState());
    s.setValue(WIN_GEO, saveGeometry());
    QWidget::closeEvent(event);
}
