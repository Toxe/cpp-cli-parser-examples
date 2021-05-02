#include <filesystem>
#include <string>
#include <string_view>
#include <tuple>

#include <clipp.h>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

enum class OutputFormat { Text, Raw, Pretty, Data };

void show_usage_and_exit(const clipp::group& cli, const std::string& progname, const std::string_view& description, const std::string_view& example, const char* error_message = nullptr)
{
    fmt::print("{}", clipp::make_man_page(cli, progname)
        .prepend_section("DESCRIPTION", fmt::format("    {}", description))
        .append_section("EXAMPLE", fmt::format("    $ {} {}", progname, example)));

    if (error_message)
        fmt::print("\n{}\n", error_message);

    std::exit(1);
}

auto eval_args(int argc, char* argv[])
{
    const auto progname{std::filesystem::path{argv[0]}.filename().string()};
    const auto description = "CLI Parser Example (clipp)";
    const auto example = "--text 10 10 out.txt";
    bool show_help = false;
    auto log_level = spdlog::level::warn;
    int seed = -1;
    int zoom = 1;
    int width, height;
    bool info = false;
    bool run_single = false;
    bool run_multi = false;
    bool run_all = true;
    OutputFormat output_format = OutputFormat::Text;
    std::string filename;

    auto cli = (
        clipp::option("-h", "--help").set(show_help)                          % "show help",
        (clipp::option("-v", "--verbose").set(log_level, spdlog::level::info) % "show info log messages (verbose)" |
         clipp::option("-vv", "--debug").set(log_level, spdlog::level::debug) % "show debug log messages (very verbose)"),
        (clipp::option("-t", "--text").set(output_format, OutputFormat::Text)     |
         clipp::option("-p", "--pretty").set(output_format, OutputFormat::Pretty) |
         clipp::option("-d", "--data").set(output_format, OutputFormat::Data)     |
         clipp::option("-r", "--raw").set(output_format, OutputFormat::Raw))  % "output format (default: text)",
        (clipp::option("-s", "--seed") & clipp::integer("seed", seed))        % "random seed (0 or bigger)",
        (clipp::option("-z", "--zoom") & clipp::integer("zoom", zoom))        % "pixel zoom factor for .raw files (default: 1)",
        clipp::option("-i", "--info").set(info)                               % "output additional info",
        clipp::integer("width", width)                                        % "maze width",
        clipp::integer("height", height)                                      % "maze height",
        clipp::opt_value("filename", filename)                                % "output filename",
        (clipp::option("--single").set(run_single).set(run_all, false)        % "run test: single inserts for every row",
         clipp::option("--multi").set(run_multi).set(run_all, false)          % "run test: insert multiple rows in one request") |
         clipp::option("--all").set(run_all)                                  % "run all tests (default)"
    );

    if (!clipp::parse(argc, argv, cli))
        show_usage_and_exit(cli, progname, description, example);

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

    if (run_all) {
        run_single = true;
        run_multi = true;
    }

    if (show_help || !(run_single || run_multi))
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
