#include "config.hpp"
#include <memory>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/debug_output_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/trivial.hpp>

#include "state.h"// debug
#include <iostream>// debug

struct logging
{
    logging();
    ~logging() BOOST_NOEXCEPT_OR_NOTHROW;
};

int main()
{
#if BOOST_WORKAROUND(BOOST_GCC, < 40900)
    std::unique_ptr<logging> log{new logging{}};
#else
    auto log = std::make_unique<logging>();
#endif
    animal_shogi::state s;
    std::cout << s.str();
}

logging::logging()
{
    using namespace boost;
    namespace expr = log::expressions;
    namespace sinks = log::sinks;
    namespace keywords = log::keywords;

    // Visual Studioの出力ウインドウに出力する
    {
        auto const date_format =
            expr::format_date_time<posix_time::ptime>("TimeStamp", "%H:%M:%S");
        auto const format = expr::format("%1% [%2%] [%3%] [%4%] %5%\n")
            % expr::attr<unsigned int>("LineID")
            % date_format
            % expr::attr<log::thread_id>("ThreadID")
            % log::trivial::severity
            % expr::message;
        auto backend = make_shared<sinks::debug_output_backend>();
        using sinks_type = sinks::synchronous_sink<sinks::debug_output_backend>;
        auto sink = make_shared<sinks_type>(backend);
        sink->set_formatter(format);
        log::core::get()->add_sink(sink);
    }
    // ファイルへ出力する
    {
        auto const date_format =
            expr::format_date_time<posix_time::ptime>("TimeStamp", "%Y/%m/%d %H:%M:%S");
        auto const format = expr::format("[%1%] [%2%] [%3%] %4%")
            % date_format
            % expr::attr<log::thread_id>("ThreadID")
            % log::trivial::severity
            % expr::message;
        log::add_file_log(
            keywords::file_name = "log%Y%m%d_%2N.txt",
            keywords::max_size = 1024 * 1024,// 10 MiB
            keywords::format = format,
            keywords::auto_flush = true
        );
    }
    // コンソールへ出力する
    {
        auto const format = expr::format("[%1%] [%2%] %3%")
            % expr::attr<log::thread_id>("ThreadID")
            % log::trivial::severity
            % expr::message;
        auto backend = make_shared<sinks::text_ostream_backend>();
        using sink_type = sinks::synchronous_sink<sinks::text_ostream_backend>;
        backend->add_stream(shared_ptr<std::ostream>(&std::clog, log::empty_deleter()));
        backend->auto_flush(true);
        auto sink = make_shared<sink_type>(backend);
        sink->set_formatter(format);
        log::core::get()->add_sink(sink);
    }
    log::add_common_attributes();

#if !defined(ASHOGI_DEBUG)
    log::core::get()->set_filter(log::trivial::severity >= log::trivial::info);
#endif
    BOOST_LOG_TRIVIAL(info) << "起動";
}

logging::~logging() BOOST_NOEXCEPT_OR_NOTHROW
{
    BOOST_LOG_TRIVIAL(info) << "終了";
    boost::log::core::get()->remove_all_sinks();
}