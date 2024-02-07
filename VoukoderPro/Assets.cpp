#include "Assets.h"

namespace VoukoderPro
{
	Assets::Assets():
		pluginPath(boost::filesystem::path(VOUKODERPRO_HOME) / "assets")
	{}

	/**
	 * Initialize and load all plugins.
	 */
	void Assets::init()
	{
		// If the path doesn't exists return 0
		if (!boost::filesystem::is_directory(pluginPath))
			return;

		// Investigate all plugins in the plugin directory
		auto it = boost::filesystem::directory_iterator(pluginPath);
		for (auto& element : it)
		{
			// Accept only files with the default dynamic library extension
			if (element.path().extension() != boost::dll::shared_library::suffix())
				continue;

			// Try to load it
			const std::string filename = element.path().string();

			try
			{
				// Try to instanciate the plugin
				boost::function<voukoderpro_plugin_api_create_t> factory = boost::dll::import_alias<voukoderpro_plugin_api_create_t>(filename.c_str(), "CreateInstance", boost::dll::load_mode::append_decorations);
				std::shared_ptr<plugin_api> plugin = factory();

				factories.push_back(factory);

				// Register plugin
				plugins.push_back(plugin);
			}
			catch (boost::system::system_error e)
			{
				BLOG(warning) << "Unable to load plugin: " << filename;
			}
		}
	}

	/**
     * Returns a list of all loaded plugins.
     */
	void Assets::list(std::vector<AssetInfo>& assets)
	{
		for (auto& plugin : this->plugins)
			for (AssetInfo info : plugin->infos)
				assets.push_back(info);
	}

	/**
	 * Returns a list of all loaded plugins of a specific type.
	 */
	void Assets::list(std::vector<AssetInfo>& assets, NodeInfoType type)
	{
		for (auto& plugin : this->plugins)
		{
			for (AssetInfo info : plugin->infos)
			{
				if (info.type == type)
					assets.push_back(info);
			}
		}
	}

	/**
	 * Get the instance of one specific plugin.
	 */
	std::shared_ptr<Asset> Assets::createAssetInstance(const std::string id, const NodeInfoType type)
	{
		for (auto& plugin : this->plugins)
		{
			for (AssetInfo info : plugin->infos)
			{
				if (info.id == id && info.type == type)
					return plugin->createAsset(info);
			}
		}

		return nullptr;
	}
}
