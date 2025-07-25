#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <string>
#include <vector>
#include <map>

namespace OpenIKEv2 {

struct IKEProposal {
    std::string encryption;
    std::string integrity;
    int dh_group;
};

struct ESPProposal {
    std::string encryption;
    std::string integrity;
};

class ConfigManager {
public:
    explicit ConfigManager(const std::string& config_file);
    ~ConfigManager() = default;

    bool load();
    bool save() const;

    // Server configuration
    std::string getServerHost() const { return server_host_; }
    int getServerPort() const { return server_port_; }
    int getServerThreads() const { return server_threads_; }

    // IKEv2 configuration
    std::string getLocalId() const { return local_id_; }
    std::string getRemoteId() const { return remote_id_; }
    std::string getRemoteAddr() const { return remote_addr_; }
    int getRemotePort() const { return remote_port_; }
    std::string getAuthMethod() const { return auth_method_; }
    std::string getPresharedKey() const { return preshared_key_; }
    
    const std::vector<IKEProposal>& getIKEProposals() const { return ike_proposals_; }
    const std::vector<ESPProposal>& getESPProposals() const { return esp_proposals_; }
    
    int getRetransmitTimeout() const { return retransmit_timeout_; }
    int getDPDDelay() const { return dpd_delay_; }
    int getDPDTimeout() const { return dpd_timeout_; }

    // Monitoring configuration
    int getUpdateInterval() const { return update_interval_; }
    size_t getMaxHistorySize() const { return max_history_size_; }

    // Logging configuration
    std::string getLogLevel() const { return log_level_; }
    std::string getLogFile() const { return log_file_; }

private:
    std::string config_file_;
    
    // Server settings
    std::string server_host_;
    int server_port_;
    int server_threads_;
    
    // IKEv2 settings
    std::string local_id_;
    std::string remote_id_;
    std::string remote_addr_;
    int remote_port_;
    std::string auth_method_;
    std::string preshared_key_;
    
    std::vector<IKEProposal> ike_proposals_;
    std::vector<ESPProposal> esp_proposals_;
    
    int retransmit_timeout_;
    int dpd_delay_;
    int dpd_timeout_;
    
    // Monitoring settings
    int update_interval_;
    size_t max_history_size_;
    
    // Logging settings
    std::string log_level_;
    std::string log_file_;
    
    // Helper methods
    bool parseJsonFile(const std::string& content);
    void setDefaults();
};

} // namespace OpenIKEv2

#endif // CONFIG_MANAGER_HPP
