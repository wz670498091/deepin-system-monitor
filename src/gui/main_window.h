#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <mutex>
#include <thread>

#include <DButtonBox>
#include <DMainWindow>
#include <DSearchEdit>
#include <DShadowLine>
#include <DSpinner>
#include <DStackedWidget>
#include <DWidget>
#include <QTimer>

#include "ui_common.h"
#include "gui/dialog/sys_info_dialog.h"

DWIDGET_USE_NAMESPACE

class Toolbar;
class SystemServicePageWidget;
class ProcessPageWidget;
class Settings;

class MainWindow : public DMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)

public:
    static MainWindow *instance()
    {
        MainWindow *sin = m_instance.load();
        if (!sin) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!sin) {
                sin = new MainWindow();
                m_instance.store(sin);
            }
        }

        return sin;
    }

    inline Toolbar *toolbar() const { return m_toolbar; }
    inline ProcessPageWidget *processPage() const { return m_procPage; }
    inline SystemServicePageWidget *systemServicePage() const { return m_svcPage; }

Q_SIGNALS:
    void killProcessPerformed();
    void displayModeChanged(DisplayMode mode);
    void loadingStatusChanged(bool loading);
    void viewSysInfoMenuItemClicked();

public Q_SLOTS:
    inline void initDisplay()
    {
        initUI();
        initConnections();
    }

    void displayShortcutHelpDialog();

protected:
    void initUI();
    void initConnections();

    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    virtual void showEvent(QShowEvent *event);

private:
    MainWindow(DWidget *parent = nullptr);
    ~MainWindow();

    Settings *m_settings;

    QAction *m_killAction               {};
    QAction *m_viewSysInfoAct           {};
    DMenu   *m_modeMenu                 {};

    Toolbar                 *m_toolbar          {};
    DStackedWidget          *m_pages            {};
    ProcessPageWidget       *m_procPage         {};
    SystemServicePageWidget *m_svcPage          {};
    DShadowLine             *m_tbShadow         {};

    bool m_loading {true};
    char __unused__[7];     // ##padding##
    QTimer *m_timer {nullptr};

    static std::atomic<MainWindow *> m_instance;
    static std::mutex m_mutex;
};

#endif  // MAIN_WINDOW_H
