#include "Performance.h"

void to_json(nlohmann::ordered_json& j, const std::shared_ptr<TimePoint> tp)
{
	j = nlohmann::ordered_json{
		{ "name", tp->id },
		{ "in", tp->in },
		{ "out", tp->out }
	};

	if (tp->children.size() > 0)
		j["then"] = tp->children;
}

Performance::Performance(const std::string name):
	current(std::make_shared<TimePoint>(name, nullptr))
{}

Performance& Performance::start(const std::string id)
{
	auto child = std::make_shared<TimePoint>(id, current);
	child->in = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - offset).count();

	current->children.push_back(child);

	current = child;

	return *this;
}

Performance& Performance::end()
{
	current->out = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - offset).count();
	current = current->parent;

	return *this;
}

nlohmann::ordered_json Performance::json()
{
	return current;
}
