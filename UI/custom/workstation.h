#ifndef WORKSTATION_H
#define WORKSTATION_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QFrame>

class WorkStation : public QWidget
{
    Q_OBJECT
public:
    explicit WorkStation(QWidget *parent = nullptr);

    void setIcon(const QString &iconPath);
    void setStationNumber(const QString &number);
    void loadNodeConfig(const QString &jsonPath);
    void setNodeExecuting(const QString &nodeName, bool executing);
    void setRecipe(const QString &recipe);
    void setPanelId(const QString &panelId);
    void setImage(const QString &image);

private:
    void setupUI();
    void createNodes(const QStringList &nodeNames, int columns);
    void updateNodeStyle(QFrame *nodeFrame, bool executing);

    QLabel *m_iconLabel;
    QLabel *m_stationNumberLabel;
    QGridLayout *m_nodeLayout;
    QLabel *m_recipeValueLabel;
    QLabel *m_panelIdValueLabel;
    QLabel *m_imageValueLabel;
    QMap<QString, QFrame*> m_nodes;
    QString m_iconPath;
};

#endif // WORKSTATION_H