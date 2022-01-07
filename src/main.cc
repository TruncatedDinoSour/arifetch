#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <regex>
#include <algorithm>
#include <uniname.h>
#include <stdint.h>
#include <statgrab.h>
#include <vector>
#include <sys/utsname.h>
#include <vector>
#include <functional>
#include <map>

#include "./config.hh"

#define ERROR                                                                  \
    [](std::string msg) {                                                      \
        std::cerr << "ERROR: " << __FILE__ << ":" << __LINE__ << ": " << msg   \
                  << '\n';                                                     \
        exit(1);                                                               \
    }

#ifndef __gnu_linux__
ERROR("This app is only supported on GNU/Linux.")
#endif /* __gnu_linux__ */

inline std::string get_distro(void) {
    std::string file_line;
    std::ifstream file(os_file);

    if (!file)
        ERROR("std::ifstream: read failed for " + os_file);

    while (std::getline(file, file_line)) {
        if (std::regex_match(file_line.begin(), file_line.end(),
                             std::regex("^PRETTY_NAME=.*$"))) {

            std::string d_str = file_line.substr(12, file_line.size());
            d_str.erase(std::remove(d_str.begin(), d_str.end(), '"'),
                        d_str.end());

            file.close();
            return d_str;
        }
    }

    file.close();

    sg_host_info *os = sg_get_host_info_r(NULL);
    return (std::string)os->os_name + " " + os->os_release;
}

inline std::string get_ram(void) {
    sg_mem_stats *mem = sg_get_mem_stats_r(NULL);
    return std::to_string(mem->used / 1024 / 1024) + "/" +
           std::to_string(mem->total / 1024 / 1024) + " MiB";
}

inline std::string get_swap(void) {
    sg_swap_stats *swap = sg_get_swap_stats_r(NULL);
    return std::to_string(swap->used / 1024 / 1024) + "/" +
           std::to_string(swap->total / 1024 / 1024) + " MiB";
}

inline std::string get_load(void) {
    return std::to_string(sg_get_load_stats_r(NULL)->min1) + "/1 min";
}

inline std::string get_uptime(void) {
    uint64_t up = sg_get_host_info_r(NULL)->uptime;
    std::string measure;

    if (up < 60) {
        measure = "second";
    } else if ((up = up / 60) <= 60) {
        measure = "minute";
    } else if ((up = up / 60) <= 24) {
        measure = "hour";
    } else if ((up = up / 24) <= 7) {
        measure = "day";
    } else if ((up = up / 7) <= 30) {
        measure = "week";
    } else if ((up = up / 30) <= 12) {
        measure = "month";
    } else {
        measure = "year";
    }

    if (up > 1)
        measure += "s";

    return std::to_string(up) + " " + measure;
}

inline std::string get_kernel_version(void) {
    return sg_get_host_info_r(NULL)->os_release;
}

inline std::string get_terminal(void) {
    const char *term = getenv("TERM_PROGRAM");
    return term == NULL ? getenv("TERM") : term;
}

inline std::string get_shell(void) {
    std::string shell_path = getenv("SHELL");
    return shell_path.substr(shell_path.find_last_of("/\\") + 1);
}

int main(void) {
    uint64_t art_idx = 0; // TODO: Somehow merge info and art
    char *art_line;

    const std::map<const char *, std::function<std::string(void)>> info = {
        {"Distro", get_distro},     {"Memory", get_ram},
        {"Swap", get_swap},         {"Average load", get_load},
        {"Uptime", get_uptime},     {"Kernel version", get_kernel_version},
        {"Terminal", get_terminal}, {"Shell", get_shell},
    };

    const std::string entry_format =
        "%-" +
        std::to_string(std::max_element(art.begin(), art.end())->length()) +
        "s   %s: %s\n";

    for (auto hash : info) {
        if (!(art_line = (char *)art[art_idx].c_str()))
            art_line = const_cast<char *>("");

        printf(entry_format.c_str(), art_line, hash.first,
               hash.second().c_str());
        art_idx++;
    }

    while (art_idx < art.size()) {
        std::cout << art[art_idx] << '\n';
        art_idx++;
    }

    return 0;
}
