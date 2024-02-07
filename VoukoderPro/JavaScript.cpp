#include "JavaScript.h"

#include <vector>

namespace VoukoderPro
{
	JavaScript::JavaScript()
	{
		ctx = duk_create_heap_default();
	}

	JavaScript::~JavaScript()
	{
		duk_pop(ctx);
		duk_destroy_heap(ctx);
		ctx = nullptr;
	}

	std::string JavaScript::eval(const std::string js)
	{
		if (duk_peval_string(ctx, js.c_str()) != 0)
		{
			return "#ERROR";
		}

		std::string ret;
		switch (duk_get_type(ctx, -1))
		{
		case DUK_TYPE_STRING:
			ret = duk_get_string(ctx, -1);
			break;

		case DUK_TYPE_NUMBER:
		{
			duk_double_t val = duk_get_number(ctx, -1);
			duk_push_number(ctx, val);
			ret = duk_safe_to_string(ctx, -1);
			break;
		}

		case DUK_TYPE_BOOLEAN:
		{
			duk_bool_t val = duk_get_boolean(ctx, -1);
			duk_push_boolean(ctx, val);
			ret = duk_safe_to_string(ctx, -1);
			break;
		}

		default:
			ret = "#VALUE";
		}

		return ret;
	}

	void JavaScript::replaceJavaScript(std::string& input)
	{
		bool capture = false;
		int count = 0;
		std::string js;
        std::string output;

		for (size_t i = 0; i < input.size(); ++i)
		{
			const char c = input.at(i);

			if (c == '$')
			{
				if (!capture && i < input.size() - 1 && input.at(i + 1) == '{')
				{
					capture = true;
					continue;
				}
			}			

			if (capture)
			{
				if (c == '{')
				{
					++count;
				}
				else if (c == '}')
				{
					--count;
					if (count == 0)
					{
						output += eval(js);
						js = "";
						capture = false;
					}
				}
				else
					js += c;
			}
            else
                output += c;
		}

		input = output;
	}
}
