#include "propertiesdialog.h"

#include <QPushButton>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include "preferences.h"

#include "ui_propertiesdialog.h"
#include "../VoukoderPro/types.h"

/**
 * @brief PropertiesDialog::PropertiesDialog
 * @param nodeInfo
 * @param plugins
 * @param parent
 */
PropertiesDialog::PropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent) : QDialog(parent),
    ui(new Ui::PropertiesDialog), nodeInfo(nodeInfo), formatProperty(nullptr)
{
    ui->setupUi(this);

    // Window flags
    setWindowFlag(Qt::CustomizeWindowHint, true);
    setWindowFlag(Qt::WindowTitleHint, true);
    setWindowFlag(Qt::WindowSystemMenuHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    Preferences& prefs = Preferences::instance();
    showTechNames = prefs.get<bool>(VPRO_PROPERTIES_TECHNAMES);

    // All additional tabs are hidden by default
    ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->tabStereo3D), false);
    ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->tabSpherical), false);
    ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->tabHDR), false);
    ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->tabMetadata), false);

    const QString selected = QString::fromStdString(nodeInfo->data.contains("id") ? nodeInfo->data["id"].get<std::string>() : "");
    populateItems(nodeInfo, plugins, selected);

    // Property browser
    propertyManager = new QtVariantPropertyManager;
    ui->propertyBrowser->setAlternatingRowColors(false);
    ui->propertyBrowser->setFactoryForManager(propertyManager, new QtVariantEditorFactory);
    ui->propertyBrowser->setRootIsDecorated(false);
    ui->propertyBrowser->setPropertiesWithoutValueMarked(true);
    ui->propertyBrowser->setResizeMode(QtTreePropertyBrowser::Interactive);
    ui->propertyBrowser->setSplitterPosition(200);

    // Splitter
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setStretchFactor(0, 1); // 33%
    ui->splitter->setStretchFactor(1, 2); // 66%

    //
    action.setVisible = [&](const std::string paramId, const bool visible)
    {
        QtProperty* property = findPropertyById(paramId);
        if (property)
        {
            const QList<QtBrowserItem*> items = ui->propertyBrowser->items(property);
            for (QtBrowserItem* item : items)
                ui->propertyBrowser->setItemVisible(item, visible);
        }
    };

    action.setProperty = [&](const std::string paramId, const std::string value)
    {
    };

    action.setEdit = [&](const std::string paramId)
    {
        QtProperty* property = findPropertyById(paramId);
        if (property)
        {
            const QList<QtBrowserItem*> items = ui->propertyBrowser->items(property);
            for (QtBrowserItem* item : items)
                ui->propertyBrowser->editItem(item);
        }
    };

    connect(propertyManager, &QtVariantPropertyManager::valueChanged, this, &PropertiesDialog::on_property_valueChanged);

    // Activate the selected item
    const QList<QTreeWidgetItem*> selectedItems = ui->encoders->selectedItems();
    if (selectedItems.size() > 0)
        ui->encoders->setCurrentItem(selectedItems.at(0));
    else if (!selected.isEmpty())
    {
        //QString encoder = QString::fromStdString(nodeInfo->data["params"]["encoder"].get<std::string>());

//        QMessageBox msgBox;
//        msgBox.setText("The plugin is not installed.");
//        //msgBox.setInformativeText("Voukoder does not know an encoder with the name \"" + encoder + "\".");
//        msgBox.setStandardButtons(QMessageBox::Ok);
//        msgBox.setDefaultButton(QMessageBox::Ok);
//        msgBox.exec();
    }

    ui->encoders->sortByColumn(0, Qt::SortOrder::AscendingOrder);
    ui->encoders->setFocus();

    for(int i = 0; i < ui->variablesWidget->columnCount(); i++)
        ui->variablesWidget->resizeColumnToContents(i);
}

/**
 * @brief PropertiesDialog::~PropertiesDialog
 */
PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}

/**
 * @brief PropertiesDialog::populateItems
 * @param nodeInfo
 * @param plugins
 * @param pluginId
 */
void PropertiesDialog::populateItems(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, const QString pluginId)
{
    // Bold for group names
    QFont groupFont = QFont(font());
    groupFont.setBold(true);

    // Populate encoder list
    for(const auto& plugin : plugins)
    {
        // Only list encoders of the same media type
        if (plugin.mediaType != nodeInfo->mediaType)
            continue;

        const QString groupName = tr(plugin.category.second.c_str());
        const auto items = ui->encoders->findItems(groupName, Qt::MatchExactly | Qt::MatchRecursive);

        // Do we already have this codec group
        QTreeWidgetItem* groupItem = nullptr;
        if (items.size() == 0)
        {
            groupItem = new QTreeWidgetItem();
            groupItem->setText(0, groupName);
            groupItem->setFont(0, groupFont);
            groupItem->setToolTip(0, tr(plugin.category.second.c_str()));

            ui->encoders->addTopLevelItem(groupItem);
        }
        else
        {
            groupItem = items.at(0);
        }

        // Build the encoder item
        QTreeWidgetItem* item = new QTreeWidgetItem(groupItem);
        item->setText(0, tr((showTechNames ? plugin.id : plugin.name).c_str()));
        item->setData(0, Qt::UserRole, QVariant::fromValue(plugin));

        // Is this the selected encoder?
        if (plugin.id == pluginId.toStdString())
            item->setSelected(true);

        groupItem->addChild(item);
        groupItem->sortChildren(0, Qt::SortOrder::AscendingOrder);

        // Create param cache for each encoder
        if (nodeInfo->data["id"] == plugin.id)
        {
            // The data of the saved configuration for the saved encoder
            itemParamsCache[plugin.id] = nodeInfo->data["params"];
        }
        else
            itemParamsCache[plugin.id] = nlohmann::ordered_json::object();
    }

    ui->encoders->expandAll();
}

/**
 * @brief PropertiesDialog::getValues
 * @param data
 */
void PropertiesDialog::getValues(nlohmann::ordered_json& data)
{
    const auto pluginInfo = ui->encoders->currentItem()->data(0, Qt::UserRole).value<VoukoderPro::AssetInfo>();

    if (!itemParamsCache.contains(pluginInfo.id))
        return;

    // Selection
    data["id"] = pluginInfo.id;

    // Add format if present
    if (formatProperty)
    {
        QVariant selectedValue = formatProperty->value();
        if (selectedValue.isValid())
            data["format"] = pluginInfo.formats.at(selectedValue.toInt())->id();
    }

    data["params"] = nlohmann::ordered_json::object();

    // Loop over all groups and all params
    for (const auto& [groupId, group] : pluginInfo.groups)
    {
        for (const auto& baseParam : group->params)
        {
            const std::string paramName = baseParam->name();
            QtProperty* property = findPropertyById(baseParam->id());

            // Store only with modified items
            if (property && property->isModified())
            {
                // Store only visible items
                const QList<QtBrowserItem*> browserItems = ui->propertyBrowser->items(property);
                for (QtBrowserItem* browserItem : browserItems)
                    if (ui->propertyBrowser->isItemVisible(browserItem))
                        data["params"][paramName] = itemParamsCache[pluginInfo.id][paramName];
            }
        }
    }
}

/**
 * @brief PropertiesDialog::createEnumProperty
 * @param param
 * @param selectedValue
 * @return
 */
template<typename T>
QtVariantProperty* PropertiesDialog::createEnumProperty(VoukoderPro::ItemParamBase* param, T selectedValue)
{
    QStringList optionList;

    unsigned selectedIndex = 0;

    // Populate available option list
    const auto options = param->options();
    for (unsigned i = 0; i < options.size(); i++)
    {
        const auto option = static_cast<VoukoderPro::ItemParamOption<T>*>(options.at(i));

        // Decide which text to show
        std::stringstream itemName;
        if (showTechNames)
            itemName << option->value();// << " (" + option->text() + ")";
        else
            itemName << option->text();

        optionList.append(tr(itemName.str().c_str()));

        // Which is the selected index?
        if (option->value() == selectedValue)
            selectedIndex = i;
    }

    const QString indent = QString(" ").repeated(param->indent() * 5);
    const std::string name = showTechNames ? param->name() : param->label();

    // Create property
    QtVariantProperty* property = propertyManager->addProperty(propertyManager->enumTypeId(), indent + tr(name.c_str()));
    property->setAttribute(QLatin1String("enumNames"), optionList);
    property->setValue(QVariant::fromValue(selectedIndex));

    return property;
}

/**
 * @brief PropertiesDialog::on_encoders_currentItemChanged
 * @param current
 * @param previous
 */
void PropertiesDialog::on_encoders_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    Q_UNUSED(previous)

    setCursor(Qt::WaitCursor);

    propertyManager->clear();
    ui->propertyBrowser->clear();
    propertyParamMap.clear();

    // Do we have a new selection?
    if (!current)
        return;

    // Add the encoders config widget here
    const QVariant variant = current->data(0, Qt::UserRole);
    if (variant.isValid())
    {
        // Enable the "Okay" button
        ui->propertyBrowser->setEnabled(true);

        // Get the parameters
        const auto params = nodeInfo->data["params"];
        const VoukoderPro::AssetInfo& pluginInfo = variant.value<VoukoderPro::AssetInfo>();

        // Activate the help button only if a helpUrl is provided
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->buttonBox->button(QDialogButtonBox::Help)->setEnabled(!pluginInfo.helpUrl.empty());

        if (pluginInfo.type == VoukoderPro::NodeInfoType::output)
            ui->propertyWidget->setCurrentIndex(pluginInfo.id == "file" ? 1 : 0);

        auto encoderParams = itemParamsCache.at(pluginInfo.id);

        // Formats
        if (pluginInfo.formats.size())
        {
            auto groupProperty = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("General"));

            const std::string selectedFormat = nodeInfo->data.contains("format") ? nodeInfo->data["format"].get<std::string>() : pluginInfo.formats.front()->id();

            // Populate available option list
            int selectedIndex = -1, i = 0;
            QStringList formatList;
            for (const auto& format : pluginInfo.formats)
            {
                formatList.append((showTechNames ? format->id() : format->name()).c_str());

                if (format->id() == selectedFormat)
                    selectedIndex = i;

                i++;
            }

            // What label should be displayed?
            QString formatName;
            if (showTechNames)
            {
                switch (pluginInfo.mediaType)
                {
                case VoukoderPro::MediaType::video:
                    formatName = "pix_fmt";
                    break;
                case VoukoderPro::MediaType::audio:
                    formatName = "smp_fmt";
                    break;
                default:
                    formatName = "format";
                }
            }
            else
            {
                formatName = tr("Format");
            }

            // Create the property
            formatProperty = propertyManager->addProperty(propertyManager->enumTypeId(), formatName);
            formatProperty->setAttribute(QLatin1String("enumNames"), formatList);
            formatProperty->setValue(QVariant::fromValue(selectedIndex));
            formatProperty->setDescriptionToolTip(tr("The desired pixel- or sample format for the encoder."));
            groupProperty->addSubProperty(formatProperty);

            // Background color
            QtBrowserItem* browserGroup = ui->propertyBrowser->addProperty(groupProperty);
            //ui->propertyBrowser->setBackgroundColor(browserGroup, QColor::fromRgb(255, 255, 255));
            ui->propertyBrowser->setExpanded(browserGroup, true);
        }

        // Populate parameter groups
        for (const auto& [groupId, group] : pluginInfo.groups)
        {
            auto groupProperty = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), QString::fromStdString(group->name()));

            for (const auto baseParam : group->params)
            {
                QtVariantProperty* property = nullptr;

                const QString indent = QString(" ").repeated(baseParam->indent() * 5);
                const std::string name = showTechNames ? baseParam->name() : baseParam->label();

                if (baseParam->type() == VoukoderPro::ItemParamType::Flag)
                {
                    // Get specific param type
                    VoukoderPro::ItemParam<VoukoderPro::flags>* param = static_cast<VoukoderPro::ItemParam<VoukoderPro::flags>*>(baseParam);

                    // Get the selected value and fall back to the default value if not value is selected
                    const auto selectedValue = encoderParams.contains(param->name()) ? encoderParams[param->name()].get<VoukoderPro::flags>() : param->def();

                    if (param->options().size() > 0)
                    {
                        QStringList optionList;

                        // Populate available option list
                        const auto options = param->options();
                        for (unsigned i = 0; i < options.size(); i++)
                        {
                            const auto option = static_cast<VoukoderPro::ItemParamOption<VoukoderPro::flags>*>(options.at(i));
                            optionList.append(tr(option->text().c_str()));
                        }

                        property = propertyManager->addProperty(QtVariantPropertyManager::flagTypeId(), indent + tr(name.c_str()));
                        property->setAttribute(QLatin1String("flagNames"), optionList);
                    }
                    else
                    {
                        property = propertyManager->addProperty(QVariant::Int, indent + tr(name.c_str()));
                    }

                    property->setValue(QVariant::fromValue(selectedValue));
                    property->setToolTip(tr(param->description().c_str()));

                    // Store value in param cache
                    encoderParams[param->name()] = selectedValue;
                }
                else if (baseParam->type() == VoukoderPro::ItemParamType::String)
                {
                    // Get specific param type
                    VoukoderPro::ItemParam<std::string>* param = static_cast<VoukoderPro::ItemParam<std::string>*>(baseParam);

                    // Get the selected value and fall back to the default value if not value is selected
                    const auto selectedValue = encoderParams.contains(param->name()) ? encoderParams[param->name()].get<std::string>() : param->def();

                    if (param->options().size() > 0)
                    {
                        property = createEnumProperty(baseParam, selectedValue);
                    }
                    else
                    {
                        property = propertyManager->addProperty(QVariant::String, indent + tr(name.c_str()));
                        property->setValue(QVariant::fromValue(QString::fromStdString(selectedValue)));
                    }

                    // Build tool tip
                    QString tooltip = tr(param->description().c_str());
                    if (!tooltip.isEmpty())
                        tooltip += "\n\n";

                    tooltip += tr("Default") + ": " + QString::fromStdString(param->def());

                    property->setToolTip(tooltip);

                    // Store value in param cache
                    encoderParams[param->name()] = selectedValue;
                }
                else if (baseParam->type() == VoukoderPro::ItemParamType::Integer)
                {
                    // Get specific param type
                    VoukoderPro::ItemParam<int>* param = static_cast<VoukoderPro::ItemParam<int>*>(baseParam);

                    // Get the selected value and fall back to the default value if not value is selected
                    int selectedValue = param->def() * param->multiplier();
                    if (encoderParams.contains(param->name()))
                    {
                        try {
                            selectedValue = encoderParams[param->name()].get<int>();
                        } catch (...) {
                        }
                    }

                    if (param->options().size() > 0)
                    {
                        property = createEnumProperty(baseParam, selectedValue);
                    }
                    else
                    {
                        property = propertyManager->addProperty(QVariant::Int, indent + tr(name.c_str()));
                        property->setAttribute(QLatin1String("minimum"), QVariant::fromValue(param->minimum()));
                        property->setAttribute(QLatin1String("maximum"), QVariant::fromValue(param->maximum()));
                        property->setValue(QVariant::fromValue(selectedValue / param->multiplier()));
                    }

                    // Build tool tip
                    QString tooltip = tr(param->description().c_str());
                    if (!tooltip.isEmpty())
                        tooltip += "\n\n";

                    tooltip += tr("Min.") += ": " + QString::number(param->minimum());
                    tooltip += "\n" + tr("Max.") + ": " + QString::number(param->maximum());
                    tooltip += "\n" + tr("Default") + ": " + QString::number(param->def());

                    property->setToolTip(tooltip);

                    // Store value in param cache
                    encoderParams[param->name()] = selectedValue;
                }
                else if (baseParam->type() == VoukoderPro::ItemParamType::Double)
                {
                    // Get specific param type
                    VoukoderPro::ItemParam<double>* param = static_cast<VoukoderPro::ItemParam<double>*>(baseParam);

                    // Get the selected value and fall back to the default value if not value is selected
                    double selectedValue = param->def() * param->multiplier();
                    if (encoderParams.contains(param->name()))
                    {
                        try {
                            selectedValue = encoderParams[param->name()].get<double>();
                        } catch (...) {
                        }
                    }

                    if (param->options().size() > 0)
                    {
                        property = createEnumProperty(baseParam, selectedValue);
                    }
                    else
                    {
                        property = propertyManager->addProperty(QVariant::Double, indent + tr(name.c_str()));
                        property->setAttribute(QLatin1String("minimum"), QVariant::fromValue(param->minimum()));
                        property->setAttribute(QLatin1String("maximum"), QVariant::fromValue(param->maximum()));
                        property->setValue(QVariant::fromValue(selectedValue / param->multiplier()));
                    }

                    // Build tool tip
                    QString tooltip = tr(param->description().c_str());
                    if (!tooltip.isEmpty())
                        tooltip += "\n\n";

                    tooltip += tr("Min.") += ": " + QString::number(param->minimum());
                    tooltip += "\n" + tr("Max.") + ": " + QString::number(param->maximum());
                    tooltip += "\n" + tr("Default") + ": " + QString::number(param->def());

                    property->setToolTip(tooltip);

                    // Store value in param cache
                    encoderParams[param->name()] = selectedValue;
                }
                else if (baseParam->type() == VoukoderPro::ItemParamType::Boolean)
                {
                    // Get specific param type
                    VoukoderPro::ItemParam<bool>* param = static_cast<VoukoderPro::ItemParam<bool>*>(baseParam);

                    // Get the selected value and fall back to the default value if not value is selected
                    bool selectedValue = param->def() * param->multiplier();
                    if (encoderParams.contains(param->name()))
                    {
                        try {
                            selectedValue = encoderParams[param->name()].get<bool>();
                        } catch (...) {
                        }
                    }

                    const QString yes = showTechNames ? "1" : tr("Yes");
                    const QString no = showTechNames ? "0" : tr("No");

                    // Boolean values
                    QStringList optionList;
                    optionList.append(no);
                    optionList.append(yes);

                    // Represent boolean params as a dropdown with Yes / No
                    property = propertyManager->addProperty(propertyManager->enumTypeId(), indent + tr(name.c_str()));
                    property->setAttribute(QLatin1String("enumNames"), optionList);
                    property->setValue(QVariant::fromValue(selectedValue ? 1 : 0));

                    // Build tool tip
                    QString tooltip = tr(param->description().c_str());
                    if (!tooltip.isEmpty())
                        tooltip += "\n\n";

                    tooltip += tr("Default") + ": " + (param->def() ? yes : no);

                    property->setToolTip(tooltip);

                    // Store value in param cache
                    encoderParams[param->name()] = selectedValue;
                }

                groupProperty->addSubProperty(property);

                // Store property -> param pair in a map
                propertyParamMap.insert(std::make_pair(property, baseParam));
            }

            if (groupProperty->subProperties().size() > 0)
            {
                QtBrowserItem* browserGroup = ui->propertyBrowser->addProperty(groupProperty);
                ui->propertyBrowser->setExpanded(browserGroup, true);

                // Background color
                switch (group->type())
                {
                case VoukoderPro::ItemParamGroupType::Forced:
                    //ui->propertyBrowser->setBackgroundColor(browserGroup, QColor::fromRgb(255, 255, 205));
                    ui->propertyBrowser->setExpanded(browserGroup, true);
                    break;

                case VoukoderPro::ItemParamGroupType::Standard:
                    //ui->propertyBrowser->setBackgroundColor(browserGroup, QColor::fromRgb(205, 255, 255));
                    ui->propertyBrowser->setExpanded(browserGroup, true);
                    break;
                }
            }
        }

        // Call the triggers initially
        const auto groups = ui->propertyBrowser->properties();
        for (const auto& group : groups)
        {
            const auto properties = group->subProperties();
            for (const auto& property : properties)
            {
                QtVariantProperty* variantProperty = static_cast<QtVariantProperty*>(property);

                on_property_valueChanged(property, variantProperty->value());

                // Expand expandable (i.e. flags) properties if they are modified
                const auto items = ui->propertyBrowser->items(property);
                for (const auto item : items)
                    ui->propertyBrowser->setExpanded(item, property->isModified());
            }
        }
    }
    else
    {
        // Disable the "Okay" button
        ui->propertyWidget->setCurrentIndex(0);
        ui->propertyBrowser->setEnabled(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->button(QDialogButtonBox::Help)->setEnabled(false);
    }

    setCursor(Qt::ArrowCursor);
}

/**
 * @brief PropertiesDialog::findPropertyById
 * @param id
 * @return
 */
QtProperty* PropertiesDialog::findPropertyById(const std::string id)
{
    const auto it = std::find_if(propertyParamMap.begin(), propertyParamMap.end(), [=](std::pair<QtProperty*, VoukoderPro::ItemParamBase*> pair)
    {
       return pair.second->id() == id;
    });

    return it != propertyParamMap.end() ? it->first : nullptr;
}

/**
 * @brief PropertiesDialog::getGroupByParamId
 * @param info
 * @param id
 * @return
 */
VoukoderPro::ItemParamGroup* PropertiesDialog::getGroupByParamId(const VoukoderPro::AssetInfo& info, const std::string id)
{
    size_t pos = id.find('.');
    if (pos != std::string::npos)
    {
        std::string extracted = id.substr(0, pos);

        for (const auto& [groupId, group] : info.groups)
            if (groupId == extracted)
                return group;
    }

    return nullptr;
}

/**
 * @brief PropertiesDialog::on_property_valueChanged
 * @param property
 * @param variant
 */
void PropertiesDialog::on_property_valueChanged(QtProperty* property, const QVariant& unused)
{
    // Can we map this property to a param?
    if (propertyParamMap.find(property) == propertyParamMap.end())
        return;

    const auto baseParam = propertyParamMap.at(property);

    const auto pluginInfo = ui->encoders->currentItem()->data(0, Qt::UserRole).value<VoukoderPro::AssetInfo>();
    if (!itemParamsCache.contains(pluginInfo.id))
        return;

    nlohmann::json& paramValue = itemParamsCache[pluginInfo.id][baseParam->name()];

    // Get the group the parameter is in
    auto group = getGroupByParamId(pluginInfo, baseParam->id());
    if (!group)
        return;

    QtVariantProperty* variantProperty = static_cast<QtVariantProperty*>(property);
    const QVariant& variant = variantProperty->value();

    // We require a valid variant
    if (!variant.isValid())
        return;

    if (variant.type() == QVariant::Int)
    {
        const int value = variant.value<int>();

        if (baseParam->type() == VoukoderPro::ItemParamType::Boolean)
        {
            const bool optionValue = value != 0;

            paramValue = optionValue;
            property->setModified(group->type() == VoukoderPro::ItemParamGroupType::Forced || optionValue != reinterpret_cast<VoukoderPro::ItemParam<bool>*>(baseParam)->def());
        }
        else if (baseParam->options().size() > 0) // It's a drop down
        {
            // Store that value in the encoder params
            if (baseParam->type() == VoukoderPro::ItemParamType::Flag)
            {
                paramValue = value;
                property->setModified(group->type() == VoukoderPro::ItemParamGroupType::Forced || value != reinterpret_cast<VoukoderPro::ItemParam<VoukoderPro::flags>*>(baseParam)->def());
            }
            else
            {
                const auto option = baseParam->options().at(value);

                if (baseParam->type() == VoukoderPro::ItemParamType::String)
                {
                    const std::string optionValue = reinterpret_cast<VoukoderPro::ItemParamOption<std::string>*>(option)->value();

                    paramValue = optionValue;
                    property->setModified(group->type() == VoukoderPro::ItemParamGroupType::Forced || optionValue != reinterpret_cast<VoukoderPro::ItemParam<std::string>*>(baseParam)->def());
                }
                else if (baseParam->type() == VoukoderPro::ItemParamType::Integer)
                {
                    const int optionValue = reinterpret_cast<VoukoderPro::ItemParamOption<int>*>(option)->value();

                    paramValue = optionValue;
                    property->setModified(group->type() == VoukoderPro::ItemParamGroupType::Forced || optionValue != reinterpret_cast<VoukoderPro::ItemParam<int>*>(baseParam)->def());
                }
                else if (baseParam->type() == VoukoderPro::ItemParamType::Double)
                {
                    const double optionValue = reinterpret_cast<VoukoderPro::ItemParamOption<double>*>(option)->value();

                    paramValue = optionValue;
                    property->setModified(group->type() == VoukoderPro::ItemParamGroupType::Forced || optionValue != reinterpret_cast<VoukoderPro::ItemParam<double>*>(baseParam)->def());
                }

                option->callSelected(action);
            }
        }
        else
        {
            const auto param = reinterpret_cast<VoukoderPro::ItemParam<int>*>(baseParam);
            paramValue = value * param->multiplier();
            property->setModified(group->type() == VoukoderPro::ItemParamGroupType::Forced || value != param->def());
        }
    }
    else if (variant.type() == QVariant::Double)
    {
        const auto param = reinterpret_cast<VoukoderPro::ItemParam<double>*>(baseParam);
        const double doubleValue = variant.value<double>();

        paramValue = doubleValue * param->multiplier();
        property->setModified(group->type() == VoukoderPro::ItemParamGroupType::Forced || doubleValue != reinterpret_cast<VoukoderPro::ItemParam<double>*>(baseParam)->def());
    }
    else if (variant.type() == QVariant::String)
    {
        const std::string stringValue = variant.value<QString>().toStdString();

        paramValue = stringValue;
        property->setModified(group->type() == VoukoderPro::ItemParamGroupType::Forced || stringValue != reinterpret_cast<VoukoderPro::ItemParam<std::string>*>(baseParam)->def());
    }
}

/**
 * @brief PropertiesDialog::on_buttonBox_clicked
 * @param button
 */
void PropertiesDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->tabWidget->currentIndex() == 0)
    {
        const QTreeWidgetItem* current = ui->encoders->currentItem();

        // HELP
        if (button == ui->buttonBox->button(QDialogButtonBox::Help) && current)
        {
            const QVariant variant = current->data(0, Qt::UserRole);
            if (variant.isValid())
            {
                const VoukoderPro::AssetInfo& pluginInfo = variant.value<VoukoderPro::AssetInfo>();
                const QString helpUrl = QString::fromStdString(pluginInfo.helpUrl);

                QDesktopServices::openUrl(helpUrl);
            }
        }
    }
}

/**
 * @brief PropertiesDialog::on_tabWidget_currentChanged
 * @param index
 */
void PropertiesDialog::on_tabWidget_currentChanged(int index)
{
    bool enabled = false;

    if (index == 0)
    {
        const QTreeWidgetItem* currentItem = ui->encoders->currentItem();
        if (currentItem)
        {
            // Add the encoders config widget here
            const QVariant variant = currentItem->data(0, Qt::UserRole);
            if (variant.isValid())
            {
                const VoukoderPro::AssetInfo& pluginInfo = variant.value<VoukoderPro::AssetInfo>();
                enabled = !pluginInfo.helpUrl.empty();
            }
        }
    }

    //
    ui->buttonBox->button(QDialogButtonBox::Help)->setEnabled(enabled);
}

/**
 * @brief PropertiesDialog::on_filenameButton_clicked
 */
void PropertiesDialog::on_filenameButton_clicked()
{
    // Config directory
    QDir dir(qgetenv("USERPROFILE"));
    dir.cd("Desktop");

    // Get filename
    const QString filename = QFileDialog::getSaveFileName(nullptr, QString(tr("Save as ...")), dir.absolutePath(), "All files (*.*)");

    if (!filename.isEmpty())
        ui->filenameInput->setText(QDir::toNativeSeparators(filename));
}

/**
 * @brief PropertiesDialog::on_treeWidget_itemDoubleClicked
 * @param item
 * @param column
 */
void PropertiesDialog::on_variablesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    const QString variable = "${" + item->text(0) + "}";

    const int cursorPosition = ui->filenameInput->cursorPosition();
    ui->filenameInput->insert(variable);
    ui->filenameInput->setCursorPosition(cursorPosition + variable.length());
    ui->filenameInput->setFocus();
}
