#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include "nlohmann/json.hpp"

struct TimePoint
{
	TimePoint(const std::string id, std::shared_ptr<TimePoint> parent) :
		id(id), parent(parent)
	{}

	std::string id;
	uint64_t in = 0;
	uint64_t out = 0;
	std::vector<std::shared_ptr<TimePoint>> children;
	std::shared_ptr<TimePoint> parent = nullptr;
};

struct Performance
{
	Performance(const std::string name);
	Performance& start(const std::string id);
	Performance& end();
	nlohmann::ordered_json json();

private:
	std::shared_ptr<TimePoint> current = nullptr;
	std::chrono::steady_clock::time_point offset = std::chrono::steady_clock::now();
};