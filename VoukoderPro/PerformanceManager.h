#pragma once

//#include <leveldb/db.h>

#include "voukoderpro_api.h"

//#pragma comment(lib, "leveldb")

namespace VoukoderPro
{
	class PerformanceManager : public IPerformanceManager
	{
	private:
		PerformanceManager(const std::string path);

	public:
		PerformanceManager(const PerformanceManager&) = delete;
		PerformanceManager& operator=(const PerformanceManager&) = delete;
		PerformanceManager(PerformanceManager&&) = delete;
		PerformanceManager& operator=(PerformanceManager&&) = delete;
		~PerformanceManager();

		static auto& instance(const std::string path)
		{
			static PerformanceManager instance(path);
			return instance;
		}
		
		// IPerformanceManager
		int create(const std::string key, const std::string document);
		int list(std::vector<std::string>&);

	private:
		//leveldb::DB* db = nullptr;
	};
}

