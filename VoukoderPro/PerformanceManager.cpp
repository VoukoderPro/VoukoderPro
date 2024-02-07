#include "PerformanceManager.h"
#include "Logger.h"

namespace VoukoderPro
{
	PerformanceManager::PerformanceManager(const std::string path)
	{
		// Open performance DB
		//leveldb::Options options;
		//options.create_if_missing = true;

		//leveldb::Status status = leveldb::DB::Open(options, path, &db);
		//if (!status.ok())
		//	BLOG(error) << "Unable to open performance database: " << status.ToString();
	}

	PerformanceManager::~PerformanceManager()
	{
		//if (db)
		//	delete db;
	}

	int PerformanceManager::create(const std::string key, const std::string document)
	{
		//if (db)
		//{
		//	leveldb::Status status = db->Put(leveldb::WriteOptions(), key, document);
		//	if (!status.ok())
		//	{
		//		BLOG(error) << "Error storing document: " << status.ToString();
		//		return ERR_DATABASE;
		//	}
		//}

		return ERR_OK;
	}

	int PerformanceManager::list(std::vector<std::string>& items)
	{
		return ERR_OK;
	}
}