#pragma once

#include <string>

#include "duktape.h"

namespace VoukoderPro
{
	class JavaScript
	{
	public:
		JavaScript();
		~JavaScript();
		void setGlobalString(const std::string name, const std::string value);
		std::string eval(const std::string js);
		void replaceJavaScript(std::string& input);

	private:
		duk_context* ctx = nullptr;
	};
}

