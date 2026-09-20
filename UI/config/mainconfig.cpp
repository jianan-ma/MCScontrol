#include "mainconfig.h"
#include "plcconfig.h"
#include "serialconfig.h"
#include "socketconfig.h"
#include "systemconfig.h"
#include <QVBoxLayout>
MainConfig::MainConfig(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    tabwidget = new QTabWidget;
    mainlayout->addWidget(tabwidget);
    setStyleSheet("QTabWidget::pane {"
                  "    border: 1px solid #d0d7de;"
                  "    border-radius: 6px;"
                  "    background: white;"
                  "    top: -1px;"
                  "}"
                  "QTabBar::tab {"
                  "    background: #e8edf3;"
                  "    color: #4a5568;"
                  "    padding: 8px 28px;"
                  "    min-width: 80px;"
                  "    border-top-left-radius: 6px;"
                  "    border-top-right-radius: 6px;"
                  "    margin-right: 2px;"
                  "}"
                  "QTabBar::tab:selected {"
                  "    background: white;"
                  "    color: #1a1a2e;"
                  "    font-weight: bold;"
                  "    padding-left: 32px;"
                  "    padding-right: 32px;"
                  "}"
                  "QTabBar::tab:hover:!selected {"
                  "    background: #dde4ec;"
                  "}"
                  "QScrollArea {"
                  "    border: none;"
                  "    background: transparent;"
                  "}"
                  "QScrollBar:vertical {"
                  "    background-color: #f0f4f8;"
                  "    width: 8px;"
                  "}"
                  "QScrollBar::handle:vertical {"
                  "    background-color: #cfd8e3;"
                  "    border-radius: 4px;"
                  "    min-height: 30px;"
                  "}"
                  "QScrollBar::handle:vertical:hover {"
                  "    background-color: #b0bac5;"
                  "}"
                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                  "    height: 0px;"
                  "}");
    PLCConfig *plcconfig = new PLCConfig;
    SerialConfig *serialconfig = new SerialConfig;
    SocketConfig *socketconfig = new SocketConfig;
    SystemConfig *systemconfig = new SystemConfig;
    tabwidget->addTab(plcconfig,"PLC配置");
    tabwidget->addTab(serialconfig,"串口配置");
    tabwidget->addTab(socketconfig,"Socket配置");
    tabwidget->addTab(systemconfig,"系统配置");
}
