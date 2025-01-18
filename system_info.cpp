#include "system_info.hpp"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <memory>
#include <array>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <regex>
#include <cmath>
#include <filesystem>

// ANSI color codes
const std::string SystemInfo::RESET = "\033[0m";
const std::string SystemInfo::BOLD = "\033[1m";
const std::string SystemInfo::BLUE = "\033[34m";
const std::string SystemInfo::GREEN = "\033[32m";
const std::string SystemInfo::YELLOW = "\033[33m";

const std::string SystemInfo::ART = R"(
  ________       __ 
 /  _____/ __ __|__|
/   \  ___|  |  \  |
\    \_\  \  |  /  |
 \______  /____/|__|
        \/          
)";

SystemInfo::SystemInfo() {}

std::string SystemInfo::readFile(const std::string& path) const {
    std::ifstream file(path);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return "N/A";
}

std::string SystemInfo::executeCommand(const std::string& cmd) const {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    
    if (!pipe) {
        return "N/A";
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    return result;
}

std::string SystemInfo::getOSInfo() const {
    std::string content = readFile("/etc/os-release");
    std::regex pattern("PRETTY_NAME=\"([^\"]*)\"");
    std::smatch matches;
    if (std::regex_search(content, matches, pattern)) {
        return matches[1].str();
    }
    return "Arch Linux";
}

std::string SystemInfo::getKernelVersion() const {
    struct utsname uts;
    if (uname(&uts) == 0) {
        return uts.release;
    }
    return "N/A";
}

std::string SystemInfo::getShell() const {
    if (const char* shell = getenv("SHELL")) {
        return shell;
    }
    return "N/A";
}

std::string SystemInfo::getCPUInfo() const {
    std::string cpuinfo = readFile("/proc/cpuinfo");
    std::istringstream stream(cpuinfo);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.find("model name") != std::string::npos) {
            return line.substr(line.find(":") + 2);
        }
    }
    return "N/A";
}

std::string SystemInfo::getMemoryUsage() const {
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        double total = si.totalram * si.mem_unit / (1024.0 * 1024.0);
        double used = (si.totalram - si.freeram) * si.mem_unit / (1024.0 * 1024.0);
        double percentage = (used / total) * 100.0;
        
        std::stringstream ss;
        ss.precision(2);
        ss << std::fixed << used << "MiB / " << total << "MiB (" << percentage << "%)";
        return ss.str();
    }
    return "N/A";
}

std::string SystemInfo::getGPUInfo() const {
    // Try NVIDIA first
    std::string nvidia_path = "/proc/driver/nvidia/gpus/0/information";
    if (std::filesystem::exists(nvidia_path)) {
        std::string info = readFile(nvidia_path);
        std::regex pattern("Model:\\s+(.+)");
        std::smatch matches;
        if (std::regex_search(info, matches, pattern)) {
            return matches[1].str();
        }
    }
    
    // Fall back to lspci
    return executeCommand("lspci | grep -i vga | cut -d ':' -f3");
}

std::string SystemInfo::getPackageCount() const {
    std::string count = executeCommand("pacman -Q | wc -l");
    if (count.empty()) return "N/A";
    return count.substr(0, count.find_last_not_of("\n\r") + 1) + " (pacman)";
}

std::string SystemInfo::getUptime() const {
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        int hours = si.uptime / 3600;
        int minutes = (si.uptime % 3600) / 60;
        std::stringstream ss;
        ss << hours << "h " << minutes << "m";
        return ss.str();
    }
    return "N/A";
}

std::string SystemInfo::getDesktopEnvironment() const {
    if (const char* de = getenv("XDG_CURRENT_DESKTOP")) {
        return de;
    } else if (const char* wm = getenv("WINDOWMANAGER")) {
        return wm;
    }
    std::string out = executeCommand("ps -e | grep -m 1 -o -E 'gnome-session|kde|xfce|mate|i3|sway|bspwm'");
    return out.empty() ? "Unknown" : out;
}

std::string SystemInfo::getUserHostInfo() const {
    char hostname[1024];
    struct passwd *pw = getpwuid(getuid());
    gethostname(hostname, 1024);
    std::stringstream ss;
    ss << pw->pw_name << "@" << hostname;
    return ss.str();
}

const std::string& SystemInfo::getASCIIArt() {
    return ART;
}

