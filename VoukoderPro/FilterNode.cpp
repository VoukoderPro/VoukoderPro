#include "FilterNode.h"

#include <boost/algorithm/string.hpp>

namespace VoukoderPro
{
	FilterNode::FilterNode(std::shared_ptr<NodeInfo> nodeInfo):
		BaseNode(nodeInfo)
	{}

	/**
	* Builds the filter chain.
	*/
	int FilterNode::preinit(const int nleTrackIndex, std::shared_ptr<NodeData> nleData)
	{
		int ret = ERR_OK;

		nleData->filterConfig += nodeId;

		// Append all filter params
		std::vector<std::string> params;
		for (auto& [name, value] : nodeParams.items())
		{
			std::stringstream param("=");
			param << name << "=";
			if (value.is_string())
				param << value.get<std::string>();
			else if (value.is_number_integer())
				param << value.get<int>();
			else if (value.is_number_float())
				param << value.get<float>();
			else if (value.is_boolean())
				param << value.get<bool>() ? "1" : "0";
			else
				continue;

			params.push_back(param.str());
		}

		// Append the filter parameters
		if (params.size() > 0)
			nleData->filterConfig += "=" + boost::algorithm::join(params, ":");

		// Insert splits if necessary
		if (outputs.size() > 1)
		{
			nleData->filterConfig += "[" + getFilterId() + "];[" + getFilterId() + "]" + getSplitName() + "=" + std::to_string(outputs.size());

			for (auto& node : outputs)
				nleData->filterConfig += "[split_" + node.lock()->getFilterId() + "]";

			nleData->filterConfig += ";";
		}
		else
			nleData->filterConfig += ",";

		// Process the output nodes
		for (auto& node : outputs)
		{
			// Handle splits if present
			if (outputs.size() > 1)
				nleData->filterConfig += "[split_" + node.lock()->getFilterId() + "]";

			// Call subnodes
			if ((ret = node.lock()->preinit(nleTrackIndex, nleData)) < ERR_OK)
				break;
		}

		return ret;
	}
}
