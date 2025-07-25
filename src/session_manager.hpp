#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <atomic>
#include "config_manager.hpp"

namespace OpenIKEv2 {

enum class SessionState {
    IDLE,
    CONNECTING,
    AUTHENTICATING,
    ESTABLISHING,
    ESTABLISHED,
    REKEYING,
    DELETING,
    FAILED,
    DISCONNECTED
};

struct SessionInfo {
    std::string session_id;
    SessionState state;
    std::string local_id;
    std::string remote_id;
    std::string remote_addr;
    int remote_port;
    time_t created_at;
    time_t last_activity;
    std::string error_message;
    
    // IKE SA info
    uint64_t ike_spi_i;
    uint64_t ike_spi_r;
    
    // Child SA info
    uint32_t esp_spi_in;
    uint32_t esp_spi_out;
    
    // Statistics
    uint64_t bytes_sent;
    uint64_t bytes_received;
    uint32_t packets_sent;
    uint32_t packets_received;
};

class SessionManager {
public:
    explicit SessionManager(const ConfigManager& config);
    ~SessionManager();

    bool initialize();
    void cleanup();

    // Session operations
    std::string createSession(const std::string& remote_addr, int remote_port);
    bool startSession(const std::string& session_id);
    bool stopSession(const std::string& session_id);
    bool deleteSession(const std::string& session_id);

    // Session queries
    std::vector<std::string> getActiveSessions() const;
    SessionInfo getSessionInfo(const std::string& session_id) const;
    std::map<std::string, SessionInfo> getAllSessions() const;

    // State management
    void updateSessionState(const std::string& session_id, SessionState state);
    void updateSessionStats(const std::string& session_id, 
                           uint64_t bytes_sent, uint64_t bytes_received,
                           uint32_t packets_sent, uint32_t packets_received);

    // Error handling
    void setSessionError(const std::string& session_id, const std::string& error);

private:
    const ConfigManager& config_;
    mutable std::mutex sessions_mutex_;
    std::map<std::string, SessionInfo> sessions_;
    std::atomic<uint32_t> next_session_id_;

    // Helper methods
    std::string generateSessionId();
    void initializeSessionInfo(SessionInfo& info, const std::string& session_id,
                              const std::string& remote_addr, int remote_port);
    
    // IKEv2 integration methods
    bool establishIKESA(const std::string& session_id);
    bool establishChildSA(const std::string& session_id);
    void handleIKEMessage(const std::string& session_id, const void* message, size_t length);
};

} // namespace OpenIKEv2

#endif // SESSION_MANAGER_HPP
