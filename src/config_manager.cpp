#include "config_manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace OpenIKEv2 {

ConfigManager::ConfigManager(const std::string& config_file)
    : config_file_(config_file) {
    setDefaults();
}

bool ConfigManager::load() {
    std::ifstream file(config_file_);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open config file: " << config_file_ << std::endl;
        std::cerr << "Using default configuration values" << std::endl;
        return true; // Use defaults
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    return parseJsonFile(content);
}

bool ConfigManager::save() const {
    // Implement JSON serialization and file writing
    // For now, just indicate success
    return true;
}

bool ConfigManager::parseJsonFile(const std::string& content) {
    // Simple JSON parsing - in a real implementation, use nlohmann/json
    // For this example, we'll do basic string parsing
    
    try {
        // Parse server configuration
        size_t pos = content.find("\"host\":");
        if (pos != std::string::npos) {
            size_t start = content.find("\"", pos + 7) + 1;
            size_t end = content.find("\"", start);
            if (start != std::string::npos && end != std::string::npos) {
                server_host_ = content.substr(start, end - start);
            }
        }

        pos = content.find("\"port\":");
        if (pos != std::string::npos) {
            size_t start = content.find_first_of("0123456789", pos + 7);
            size_t end = content.find_first_not_of("0123456789", start);
            if (start != std::string::npos) {
                std::string port_str = content.substr(start, end - start);
                server_port_ = std::stoi(port_str);
            }
        }

        pos = content.find("\"threads\":");
        if (pos != std::string::npos) {
            size_t start = content.find_first_of("0123456789", pos + 10);
            size_t end = content.find_first_not_of("0123456789", start);
            if (start != std::string::npos) {
                std::string threads_str = content.substr(start, end - start);
                server_threads_ = std::stoi(threads_str);
            }
        }

        // Parse IKEv2 configuration
        pos = content.find("\"local_id\":");
        if (pos != std::string::npos) {
            size_t start = content.find("\"", pos + 11) + 1;
            size_t end = content.find("\"", start);
            if (start != std::string::npos && end != std::string::npos) {
                local_id_ = content.substr(start, end - start);
            }
        }

        pos = content.find("\"remote_id\":");
        if (pos != std::string::npos) {
            size_t start = content.find("\"", pos + 12) + 1;
            size_t end = content.find("\"", start);
            if (start != std::string::npos && end != std::string::npos) {
                remote_id_ = content.substr(start, end - start);
            }
        }

        pos = content.find("\"remote_addr\":");
        if (pos != std::string::npos) {
            size_t start = content.find("\"", pos + 14) + 1;
            size_t end = content.find("\"", start);
            if (start != std::string::npos && end != std::string::npos) {
                remote_addr_ = content.substr(start, end - start);
            }
        }

        pos = content.find("\"remote_port\":");
        if (pos != std::string::npos) {
            size_t start = content.find_first_of("0123456789", pos + 14);
            size_t end = content.find_first_not_of("0123456789", start);
            if (start != std::string::npos) {
                std::string port_str = content.substr(start, end - start);
                remote_port_ = std::stoi(port_str);
            }
        }

        pos = content.find("\"preshared_key\":");
        if (pos != std::string::npos) {
            size_t start = content.find("\"", pos + 16) + 1;
            size_t end = content.find("\"", start);
            if (start != std::string::npos && end != std::string::npos) {
                preshared_key_ = content.substr(start, end - start);
            }
        }

        // Parse monitoring configuration
        pos = content.find("\"update_interval\":");
        if (pos != std::string::npos) {
            size_t start = content.find_first_of("0123456789", pos + 18);
            size_t end = content.find_first_not_of("0123456789", start);
            if (start != std::string::npos) {
                std::string interval_str = content.substr(start, end - start);
                update_interval_ = std::stoi(interval_str);
            }
        }

        pos = content.find("\"max_history\":");
        if (pos != std::string::npos) {
            size_t start = content.find_first_of("0123456789", pos + 14);
            size_t end = content.find_first_not_of("0123456789", start);
            if (start != std::string::npos) {
                std::string history_str = content.substr(start, end - start);
                max_history_size_ = std::stoul(history_str);
            }
        }

        std::cout << "Configuration loaded successfully from: " << config_file_ << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error parsing config file: " << e.what() << std::endl;
        std::cerr << "Using default configuration values" << std::endl;
        setDefaults();
        return true; // Continue with defaults
    }
}

void ConfigManager::setDefaults() {
    // Server defaults
    server_host_ = "0.0.0.0";
    server_port_ = 5000;
    server_threads_ = 4;
    
    // IKEv2 defaults
    local_id_ = "client@example.com";
    remote_id_ = "vpn.example.com";
    remote_addr_ = "192.168.1.1";
    remote_port_ = 500;
    auth_method_ = "psk";
    preshared_key_ = "default_psk";
    
    // Default IKE proposals
    ike_proposals_.clear();
    IKEProposal ike_prop;
    ike_prop.encryption = "aes256";
    ike_prop.integrity = "sha256";
    ike_prop.dh_group = 14;
    ike_proposals_.push_back(ike_prop);
    
    // Default ESP proposals
    esp_proposals_.clear();
    ESPProposal esp_prop;
    esp_prop.encryption = "aes256";
    esp_prop.integrity = "sha256";
    esp_proposals_.push_back(esp_prop);
    
    // Timeout defaults
    retransmit_timeout_ = 4000;
    dpd_delay_ = 30;
    dpd_timeout_ = 120;
    
    // Monitoring defaults
    update_interval_ = 1000;
    max_history_size_ = 100;
    
    // Logging defaults
    log_level_ = "info";
    log_file_ = "ikev2_integration.log";
}

} // namespace OpenIKEv2
