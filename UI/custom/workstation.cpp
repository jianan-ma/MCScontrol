#include "workstation.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QFrame>

WorkStation::WorkStation(QWidget *parent) :
    QWidget(parent),
    m_iconPath(":/icon/res/workstation.png")
{
    setupUI();
}

void WorkStation::setupUI()
{
    setStyleSheet(
        "WorkStation {"
        "    background-color: #ffffff;"
        "    border: 1px solid #e0e6ed;"
        "    border-radius: 8px;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(12);

    // === 顶部区域：图标 + 节点 ===
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setSpacing(16);

    // 左侧：图标 + 工位编号
    QFrame *iconContainer = new QFrame(this);
    iconContainer->setFixedSize(120, 100);
    iconContainer->setStyleSheet(
        "QFrame {"
        "    background-color: #f0f4f8;"
        "    border-radius: 8px;"
        "    border: 1px solid #dde4ec;"
        "}"
    );
    QHBoxLayout *iconLayout = new QHBoxLayout(iconContainer);
    iconLayout->setContentsMargins(8, 8, 8, 8);

    m_iconLabel = new QLabel(iconContainer);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setStyleSheet("background: transparent;");
    QPixmap iconPixmap(m_iconPath);
    if (!iconPixmap.isNull()) {
        m_iconLabel->setPixmap(iconPixmap.scaled(
            80, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    iconLayout->addWidget(m_iconLabel);

    // 工位编号徽章（叠加在图标右上角）
    m_stationNumberLabel = new QLabel(iconContainer);
    m_stationNumberLabel->setText("A1");
    m_stationNumberLabel->setAlignment(Qt::AlignCenter);
    m_stationNumberLabel->setFixedSize(36, 36);
    m_stationNumberLabel->setStyleSheet(
        "background-color: #e81123;"
        "color: white;"
        "border-radius: 18px;"
        "font-weight: bold;"
        "font-size: 13px;"
    );
    m_stationNumberLabel->move(42, 24);
    m_stationNumberLabel->raise();

    topLayout->addWidget(iconContainer);

    // 右侧：节点网格
    QFrame *nodeContainer = new QFrame(this);
    nodeContainer->setStyleSheet("background: transparent;");
    m_nodeLayout = new QGridLayout(nodeContainer);
    m_nodeLayout->setContentsMargins(0, 0, 0, 0);
    m_nodeLayout->setHorizontalSpacing(6);
    m_nodeLayout->setVerticalSpacing(2);

    // 默认节点（从配置加载，这里先创建默认的）
    QStringList defaultNodes = {
        "入脱", "开电", "AOI", "EDID", "Cxyv",
        "Flicker", "CCR", "Gamma", "TP", "帧检",
        "SN烧录", "关电", "出数"
    };
    createNodes(defaultNodes, 5);

	topLayout->addWidget(nodeContainer, 1);

	mainLayout->addLayout(topLayout);

    // === 底部信息区域 ===
    QFrame *infoContainer = new QFrame(this);
    infoContainer->setStyleSheet
    (
        "QFrame {"
        "    background-color: #f8fafc;"
        "    border-radius: 6px;"
        "    border: 1px solid #e8edf3;"
        "}"
    );
    QVBoxLayout *infoLayout = new QVBoxLayout(infoContainer);
    infoLayout->setContentsMargins(8, 4, 8, 4);
    infoLayout->setSpacing(6);

    // Recipe
    QHBoxLayout *recipeRow = new QHBoxLayout();
    QLabel *recipeLabel = new QLabel("Recipe:", infoContainer);
    recipeLabel->setStyleSheet(
        "color: #6b7a90;"
        "font-size: 13px;"
        "font-weight: bold;"
    );
    recipeLabel->setFixedWidth(90);
    m_recipeValueLabel = new QLabel("--", infoContainer);
    m_recipeValueLabel->setStyleSheet(
        "color: #1a1a2e;"
        "font-size: 13px;"
        "font-weight: bold;"
    );
    recipeRow->addWidget(recipeLabel);
    recipeRow->addWidget(m_recipeValueLabel, 1);
    infoLayout->addLayout(recipeRow);

    // PanelId
    QHBoxLayout *panelIdRow = new QHBoxLayout();
    QLabel *panelIdLabel = new QLabel("PanelId:", infoContainer);
    panelIdLabel->setStyleSheet(
        "color: #6b7a90;"
        "font-size: 13px;"
        "font-weight: bold;"
    );
    panelIdLabel->setFixedWidth(90);
    m_panelIdValueLabel = new QLabel("--", infoContainer);
    m_panelIdValueLabel->setStyleSheet(
        "color: #1a1a2e;"
        "font-size: 13px;"
        "font-weight: bold;"
    );
    panelIdRow->addWidget(panelIdLabel);
    panelIdRow->addWidget(m_panelIdValueLabel, 1);
    infoLayout->addLayout(panelIdRow);

    // Image
    QHBoxLayout *imageRow = new QHBoxLayout();
    QLabel *imageLabel = new QLabel("Image:", infoContainer);
    imageLabel->setStyleSheet(
        "color: #6b7a90;"
        "font-size: 13px;"
        "font-weight: bold;"
    );
    imageLabel->setFixedWidth(90);
    m_imageValueLabel = new QLabel("--", infoContainer);
    m_imageValueLabel->setStyleSheet(
        "color: #1a1a2e;"
        "font-size: 13px;"
        "font-weight: bold;"
    );
    imageRow->addWidget(imageLabel);
    imageRow->addWidget(m_imageValueLabel, 1);
    infoLayout->addLayout(imageRow);

    mainLayout->addWidget(infoContainer);
}

void WorkStation::createNodes(const QStringList &nodeNames, int columns)
{
    // 清除现有节点
    qDeleteAll(m_nodes);
    m_nodes.clear();

    // 清空布局中的残留项
    QLayoutItem *item;
    while ((item = m_nodeLayout->takeAt(0)) != nullptr) {
        delete item;
    }

    for (int i = 0; i < nodeNames.size(); ++i) {
        QFrame *nodeFrame = new QFrame(this);
        nodeFrame->setFixedSize(65, 32);
        nodeFrame->setStyleSheet(
            "QFrame {"
            "    background-color: #dfe4ec;"
//            "    border-radius: 4px;"
//            "    border: 1px solid #cfd5de;"
            "}"
        );

        QHBoxLayout *nodeLayout = new QHBoxLayout(nodeFrame);
        nodeLayout->setContentsMargins(6, 0, 6, 0);

        QLabel *nodeLabel = new QLabel(nodeNames[i], nodeFrame);
        nodeLabel->setAlignment(Qt::AlignCenter);
        nodeLabel->setStyleSheet(
            "color: #4a5568;"
            "font-size: 12px;"
            "background: transparent;"
        );
        nodeLayout->addWidget(nodeLabel);

        int row = i / columns;
        int col = i % columns;
        m_nodeLayout->addWidget(nodeFrame, row, col);

        m_nodes[nodeNames[i]] = nodeFrame;
    }
}

void WorkStation::updateNodeStyle(QFrame *nodeFrame, bool executing)
{
    if (executing) {
        nodeFrame->setStyleSheet(
            "QFrame {"
            "    background-color: #2ecc71;"
//            "    border-radius: 4px;"
//            "    border: 1px solid #27ae60;"
            "}"
        );
        // 更新内部QLabel样式
        QLabel *label = nodeFrame->findChild<QLabel*>();
        if (label) {
            label->setStyleSheet(
                "color: #ffffff;"
                "font-size: 12px;"
                "font-weight: bold;"
                "background: transparent;"
            );
        }
    } else {
        nodeFrame->setStyleSheet(
            "QFrame {"
            "    background-color: #dfe4ec;"
//            "    border-radius: 4px;"
//            "    border: 1px solid #cfd5de;"
            "}"
        );
        QLabel *label = nodeFrame->findChild<QLabel*>();
        if (label) {
            label->setStyleSheet(
                "color: #4a5568;"
                "font-size: 12px;"
                "background: transparent;"
            );
        }
    }
}

void WorkStation::setIcon(const QString &iconPath)
{
    m_iconPath = iconPath;
    QPixmap pixmap(iconPath);
    if (!pixmap.isNull()) {
        m_iconLabel->setPixmap(pixmap.scaled(
            80, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void WorkStation::setStationNumber(const QString &number)
{
    m_stationNumberLabel->setText(number);
}

void WorkStation::loadNodeConfig(const QString &jsonPath)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        int columns = obj.value("columns").toInt(5);

        QStringList nodeNames;
        QJsonArray nodesArray = obj.value("nodes").toArray();
        for (const QJsonValue &val : nodesArray) {
            nodeNames.append(val.toString());
        }

        if (!nodeNames.isEmpty()) {
            createNodes(nodeNames, columns);
        }
    }
}

void WorkStation::setNodeExecuting(const QString &nodeName, bool executing)
{
    if (m_nodes.contains(nodeName)) {
        updateNodeStyle(m_nodes[nodeName], executing);
    }
}

void WorkStation::setRecipe(const QString &recipe)
{
    m_recipeValueLabel->setText(recipe);
}

void WorkStation::setPanelId(const QString &panelId)
{
    m_panelIdValueLabel->setText(panelId);
}

void WorkStation::setImage(const QString &image)
{
    m_imageValueLabel->setText(image);
}
