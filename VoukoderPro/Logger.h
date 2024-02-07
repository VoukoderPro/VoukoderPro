#pragma once

#include <boost/filesystem.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

using namespace boost::log::trivial;

#define BLOG(LEVEL) \
  BOOST_LOG_SEV(lg::get(), LEVEL) \
    << boost::log::add_value<int>("Line", __LINE__) \
    << boost::log::add_value("Source", (strrchr(__FILE__, boost::filesystem::path::preferred_separator) ? strrchr(__FILE__, boost::filesystem::path::preferred_separator) + 1 : __FILE__))

namespace VoukoderPro
{
	//Narrow-char thread-safe logger.
	typedef boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> logger_t;

	//declares a global logger with a custom initialization
	BOOST_LOG_GLOBAL_LOGGER(lg, logger_t)
}
