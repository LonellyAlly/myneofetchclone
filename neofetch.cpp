#include "system_info.hpp"
#include <iostream>
#include <sstream>
#include <chrono>

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    SystemInfo sysInfo;
    
    // Get username@hostname for spacing calculation
    std::string userhost = sysInfo.getUserHostInfo();
    
    // Print ASCII art with system info
    std::stringstream output;
    output << sysInfo.BLUE << sysInfo.getASCIIArt() << sysInfo.RESET << "\n";
    output << sysInfo.BOLD << sysInfo.BLUE << userhost << sysInfo.RESET << "\n";
    output << std::string(userhost.length(), '-') << "\n";
    output << sysInfo.BOLD << "OS: " << sysInfo.RESET << sysInfo.getOSInfo() << "\n";
    output << sysInfo.BOLD << "Kernel: " << sysInfo.RESET << sysInfo.getKernelVersion() << "\n";
    output << sysInfo.BOLD << "Uptime: " << sysInfo.RESET << sysInfo.getUptime() << "\n";
    output << sysInfo.BOLD << "Packages: " << sysInfo.RESET << sysInfo.getPackageCount() << "\n";
    output << sysInfo.BOLD << "Shell: " << sysInfo.RESET << sysInfo.getShell() << "\n";
    output << sysInfo.BOLD << "DE/WM: " << sysInfo.RESET << sysInfo.getDesktopEnvironment() << "\n";
    output << sysInfo.BOLD << "CPU: " << sysInfo.RESET << sysInfo.getCPUInfo() << "\n";
    output << sysInfo.BOLD << "GPU: " << sysInfo.RESET << sysInfo.getGPUInfo() << "\n";
    output << sysInfo.BOLD << "Memory: " << sysInfo.RESET << sysInfo.getMemoryUsage() << "\n";
    
    std::cout << output.str();
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "\nFetch time: " << duration.count() << " ms\n";
    return 0;
}

