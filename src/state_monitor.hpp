#ifndef STATE_MONITOR_HPP
#define STATE_MONITOR_HPP

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <chrono>
#include "config_manager.hpp"
#include "session_manager.hpp"

namespace OpenIKEv2 {

struct StateSnapshot {
    std::chrono::system_clock::time_point timestamp;
    std::string json_data;
};

class StateMonitor {
public:
    StateMonitor(const ConfigManager& config, SessionManager& session_manager);
    ~StateMonitor();

    bool initialize();
    bool start();
    void stop();

    // Get current state as JSON
    std::string getCurrentStateJson() const;
    
    // Get state history
    std::vector<StateSnapshot> getStateHistory(size_t max_entries = 0) const;
    
    // Get specific session state
    std::string getSessionStateJson(const std::string& session_id) const;
    
    // Get system statistics
    std::string getSystemStatsJson() const;

private:
    const ConfigManager& config_;
    SessionManager& session_manager_;
    
    std::atomic<bool> running_;
    std::thread monitor_thread_;
    
    mutable std::mutex state_mutex_;
    std::queue<StateSnapshot> state_history_;
    size_t max_history_size_;
    
    std::chrono::milliseconds update_interval_;
    
    // Monitoring methods
    void monitoringLoop();
    void captureStateSnapshot();
    std::string generateSystemStateJson() const;
    std::string formatSessionInfo(const SessionInfo& info) const;
    std::string formatSystemStats() const;
    
    // Utility methods
    std::string timeToString(time_t timestamp) const;
    std::string durationToString(time_t start_time) const;
};

} // namespace OpenIKEv2

#endif // STATE_MONITOR_HPP
