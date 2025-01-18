#ifndef SYSTEM_INFO_HPP
#define SYSTEM_INFO_HPP

#include <string>
#include <optional>
#include <chrono>

class SystemInfo {
public:
    SystemInfo();
        
        private:
            static const std::string ART;
        
        public:
            static const std::string& getASCIIArt();
            std::string getOSInfo() const;
    std::string getKernelVersion() const;
    std::string getShell() const;
    std::string getCPUInfo() const;
    std::string getMemoryUsage() const;
    std::string getUserHostInfo() const;
    std::string getGPUInfo() const;
    std::string getPackageCount() const;
    std::string getUptime() const;
    std::string getDesktopEnvironment() const;

    // ANSI color codes
    static const std::string RESET;
    static const std::string BOLD;
    static const std::string BLUE;
    static const std::string GREEN;
    static const std::string YELLOW;
    std::string readFile(const std::string& path) const;
    std::string executeCommand(const std::string& cmd) const;
};

#endif // SYSTEM_INFO_HPP

