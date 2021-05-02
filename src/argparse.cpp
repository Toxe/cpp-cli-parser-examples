#include <filesystem>
#include <string>
#include <string_view>
#include <tuple>

#include <argparse/argparse.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

enum class OutputFormat { Text, Raw, Pretty, Data };

int parse_int(const std::string& value)
{
    return std::stoi(value);
}

void show_usage_and_exit(const argparse::ArgumentParser& cli, const std::string& progname, const std::string_view& description, const std::string_view& example, const char* error_message = nullptr)
{
    if (error_message)
        fmt::print("{}\n\n", error_message);

    fmt::print("{}\n\n", description);
    fmt::print("Example: $ {} {}\n\n", progname, example);
    fmt::print("{}", cli);

    std::exit(1);
}

auto eval_args(int argc, char* argv[])
{
    const auto progname{std::filesystem::path{argv[0]}.filename().string()};
    const auto description = "CLI Parser Example (argparse)";
    const auto example = "--text 10 10 -f out.txt";
    argparse::ArgumentParser cli(progname);

    cli.add_argument("--verbose").help("show info log messages (verbose)").default_value(false).implicit_value(true);
    cli.add_argument("-vv", "--debug").help("show debug log messages (very verbose)").default_value(false).implicit_value(true);
    cli.add_argument("-t", "--text").help("output format: text").default_value(false).implicit_value(true);
    cli.add_argument("-p", "--pretty").help("output format: pretty").default_value(false).implicit_value(true);
    cli.add_argument("-d", "--data").help("output format: data").default_value(false).implicit_value(true);
    cli.add_argument("-r", "--raw").help("output format: raw").default_value(false).implicit_value(true);
    cli.add_argument("-i", "--info").help("output additional info").default_value(false).implicit_value(true);
    cli.add_argument("--single").help("run test: single inserts for every row").default_value(false).implicit_value(true);
    cli.add_argument("--multi").help("run test: insert multiple rows in one request").default_value(false).implicit_value(true);
    cli.add_argument("--all").help("run all tests (default)").default_value(false).implicit_value(true);
    cli.add_argument("-s", "--seed").help("random seed (0 or bigger)").default_value(0).action(parse_int);
    cli.add_argument("-z", "--zoom").help("pixel zoom factor for .raw files").default_value(1).action(parse_int);
    cli.add_argument("width").help("maze width").action(parse_int);
    cli.add_argument("height").help("maze height").action(parse_int);
    cli.add_argument("-f", "--filename").help("output filename").default_value(std::string{""});

    try {
        cli.parse_args(argc, argv);
    } catch (const std::exception& err) {
        show_usage_and_exit(cli, progname, description, example, err.what());
    }

    auto width       = cli.get<int>("width");
    auto height      = cli.get<int>("height");
    auto seed        = cli.get<int>("--seed");
    auto zoom        = cli.get<int>("--zoom");
    auto info        = cli.get<bool>("--info");
    auto run_single  = cli.get<bool>("--single");
    auto run_multi   = cli.get<bool>("--multi");
    auto run_all     = cli.get<bool>("--all");
    auto filename    = cli.get<std::string>("--filename");

    OutputFormat output_format = OutputFormat::Text;

    if (cli["--text"] == true)
        output_format = OutputFormat::Text;
    else if (cli["--pretty"] == true)
        output_format = OutputFormat::Pretty;
    else if (cli["--data"] == true)
        output_format = OutputFormat::Data;
    else if (cli["--raw"] == true)
        output_format = OutputFormat::Raw;

    auto log_level = (cli.get<bool>("--verbose")) ? spdlog::level::info : spdlog::level::warn;

    if (cli.get<bool>("--debug"))
        log_level = spdlog::level::debug;

    spdlog::set_level(log_level);
    spdlog::debug("command line option \"width\": {}", width);
    spdlog::debug("command line option \"height\": {}", height);
    spdlog::debug("command line option \"filename\": \"{}\"", filename);
    spdlog::debug("command line option --text: {}", output_format == OutputFormat::Text);
    spdlog::debug("command line option --pretty: {}", output_format == OutputFormat::Pretty);
    spdlog::debug("command line option --data: {}", output_format == OutputFormat::Data);
    spdlog::debug("command line option --raw: {}", output_format == OutputFormat::Raw);
    spdlog::debug("command line option --seed: {}", seed);
    spdlog::debug("command line option --zoom: {}", zoom);
    spdlog::debug("command line option --info: {}", info);
    spdlog::debug("command line option --single: {}", run_single);
    spdlog::debug("command line option --multi: {}", run_multi);
    spdlog::debug("command line option --all: {}", run_all);

    if (!run_all && !run_single && !run_multi)
        run_all = true;

    if (run_all) {
        run_single = true;
        run_multi = true;
    }

    if (!(run_single || run_multi))
        show_usage_and_exit(cli, progname, description, example);

    if (width < 1 || height < 1)
        show_usage_and_exit(cli, progname, description, example, "Error: Maze width and height need to be at least 1");

    if (zoom < 1)
        zoom = 1;

    return std::make_tuple(seed, width, height, filename, output_format, zoom, info, run_single, run_multi);
}

int main(int argc, char* argv[])
{
    auto [seed, width, height, filename, output_format, zoom, info, run_single, run_multi] = eval_args(argc, argv);

    if (run_single)
        spdlog::info("run single test...");

    if (run_multi)
        spdlog::info("run multi test...");
}
