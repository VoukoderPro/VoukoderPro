#pragma once

#include <QDialog>
#include <QAbstractButton>
#include <QTreeWidgetItem>
#include "../VoukoderPro/types.h"
#include "qtvariantproperty.h"
#include <json.hpp>

Q_DECLARE_METATYPE(VoukoderPro::AssetInfo)

namespace Ui {
class PropertiesDialog;
}

/**
 * @brief The PropertiesDialog class
 */
class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    PropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent = nullptr);
    ~PropertiesDialog();
    virtual void getValues(nlohmann::ordered_json& data);

private slots:
    void on_encoders_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_property_valueChanged(QtProperty *property, const QVariant &val);
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_tabWidget_currentChanged(int index);
    void on_filenameButton_clicked();
    void on_variablesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    QtProperty* findPropertyById(const std::string id);
    VoukoderPro::ItemParamGroup* getGroupByParamId(const VoukoderPro::AssetInfo& info, const std::string id);
    template<typename T> unsigned getSelectedOptionIndex(const nlohmann::ordered_json& params, VoukoderPro::ItemParam<std::string>* param);
    template<typename T> QtVariantProperty* createEnumProperty(VoukoderPro::ItemParamBase* baseParam, T selectedValue);

protected:
    Ui::PropertiesDialog* ui;
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo;
    virtual void populateItems(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, const QString pluginId);

private:
    QtVariantPropertyManager* propertyManager;
    nlohmann::json itemParamsCache;
    std::map<QtProperty*,VoukoderPro::ItemParamBase*> propertyParamMap;
    VoukoderPro::ItemParamAction action;
    QWidget* propertiesWidget;
    QtVariantProperty* formatProperty;
    bool showTechNames;
};
