#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

enum class OutputFormat { Text, Raw, Pretty, Data };

void show_usage_and_exit(const CLI::App& app, const char* error_message = nullptr, const std::optional<CLI::ParseError>& error = {})
{
    if (error_message)
        fmt::print("\n{}\n", error_message);

    std::exit(error ? app.exit(error.value()) : 0);
}

auto eval_args(int argc, char* argv[])
{
    const auto progname{std::filesystem::path{argv[0]}.filename().string()};
    const auto description = "CLI Parser Example (CLI11)";
    int log_level_flag = 0;
    int seed = -1;
    int zoom = 1;
    int width, height;
    bool info = false;
    bool run_single = false;
    bool run_multi = false;
    bool run_all = false;
    bool format_text = false;
    bool format_pretty = false;
    bool format_data = false;
    bool format_raw = false;
    std::string filename;

    CLI::App app{description};

    app.add_option("width", width, "maze width")->required();
    app.add_option("height", height, "maze height")->required();
    app.add_option("filename", filename, "output filename");
    app.add_option("-s,--seed", seed, "random seed (0 or bigger)");
    app.add_option("-z,--zoom", zoom, "pixel zoom factor for .raw files", true);
    app.add_flag("-i,--info", info, "output additional info");
    app.add_flag("-v", log_level_flag, "log level (-v: verbose, -vv: debug messages)");

    auto grp_output = app.add_option_group("output format (default: text)");
    const auto f1 = grp_output->add_flag("-t,--text", format_text, "text: uses ASCII '#' characters for walls");
    const auto f2 = grp_output->add_flag("-p,--pretty", format_pretty, "pretty: uses UNICODE line drawing characters")->excludes(f1);
    const auto f3 = grp_output->add_flag("-d,--data", format_data, "data: export the internal wall data")->excludes(f1, f2);
    grp_output->add_flag("-r,--raw", format_raw, "raw: generate a 1 byte per pixel grayscale raw image")->excludes(f1, f2, f3);

    auto grp_tests = app.add_option_group("run tests (default: all)");
    const auto t1 = grp_tests->add_flag("--single", run_single, "run test: single inserts for every row");
    const auto t2 = grp_tests->add_flag("--multi", run_multi, "run test: insert multiple rows in one request")->excludes(t1);
    grp_tests->add_flag("--all", run_all, "run all tests")->excludes(t1, t2);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& error) {
        show_usage_and_exit(app, nullptr, error);
    }

    auto log_level = (log_level_flag >= 1) ? spdlog::level::info : spdlog::level::warn;

    if (log_level_flag >= 2)
        log_level = spdlog::level::debug;

    spdlog::set_level(log_level);
    spdlog::debug("command line option \"width\": {}", width);
    spdlog::debug("command line option \"height\": {}", height);
    spdlog::debug("command line option \"filename\": \"{}\"", filename);
    spdlog::debug("command line option --text: {}", format_text);
    spdlog::debug("command line option --pretty: {}", format_pretty);
    spdlog::debug("command line option --data: {}", format_data);
    spdlog::debug("command line option --raw: {}", format_raw);
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

    if (!(run_single || run_multi))
        show_usage_and_exit(app);

    if (width < 1 || height < 1)
        show_usage_and_exit(app, "Error: Maze width and height need to be at least 1");

    if (zoom < 1)
        zoom = 1;

    OutputFormat output_format = OutputFormat::Text;

    if (format_pretty)
        output_format = OutputFormat::Pretty;
    else if (format_data)
        output_format = OutputFormat::Data;
    else if (format_raw)
        output_format = OutputFormat::Raw;

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
