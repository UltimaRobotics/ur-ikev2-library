#include "state_monitor.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sys/resource.h>
#include <unistd.h>

namespace OpenIKEv2 {

StateMonitor::StateMonitor(const ConfigManager& config, SessionManager& session_manager)
    : config_(config), session_manager_(session_manager), running_(false) {
    
    max_history_size_ = config_.getMaxHistorySize();
    update_interval_ = std::chrono::milliseconds(config_.getUpdateInterval());
}

StateMonitor::~StateMonitor() {
    stop();
}

bool StateMonitor::initialize() {
    std::cout << "Initializing state monitor..." << std::endl;
    std::cout << "Update interval: " << update_interval_.count() << "ms" << std::endl;
    std::cout << "Max history size: " << max_history_size_ << std::endl;
    return true;
}

bool StateMonitor::start() {
    if (running_.load()) {
        return true;
    }
    
    running_.store(true);
    monitor_thread_ = std::thread(&StateMonitor::monitoringLoop, this);
    
    std::cout << "State monitor started" << std::endl;
    return true;
}

void StateMonitor::stop() {
    if (!running_.load()) {
        return;
    }
    
    running_.store(false);
    
    if (monitor_thread_.joinable()) {
        monitor_thread_.join();
    }
    
    std::cout << "State monitor stopped" << std::endl;
}

std::string StateMonitor::getCurrentStateJson() const {
    return generateSystemStateJson();
}

std::vector<StateSnapshot> StateMonitor::getStateHistory(size_t max_entries) const {
    std::lock_guard<std::mutex> lock(state_mutex_);
    
    std::vector<StateSnapshot> history;
    std::queue<StateSnapshot> temp_queue = state_history_;
    
    while (!temp_queue.empty()) {
        history.push_back(temp_queue.front());
        temp_queue.pop();
    }
    
    // Limit entries if requested
    if (max_entries > 0 && history.size() > max_entries) {
        history.erase(history.begin(), history.end() - max_entries);
    }
    
    return history;
}

std::string StateMonitor::getSessionStateJson(const std::string& session_id) const {
    SessionInfo info = session_manager_.getSessionInfo(session_id);
    
    if (info.session_id.empty()) {
        return R"({"error": "Session not found"})";
    }
    
    return formatSessionInfo(info);
}

std::string StateMonitor::getSystemStatsJson() const {
    return formatSystemStats();
}

void StateMonitor::monitoringLoop() {
    while (running_.load()) {
        try {
            captureStateSnapshot();
        } catch (const std::exception& e) {
            std::cerr << "Error in monitoring loop: " << e.what() << std::endl;
        }
        
        std::this_thread::sleep_for(update_interval_);
    }
}

void StateMonitor::captureStateSnapshot() {
    std::string json_data = generateSystemStateJson();
    
    StateSnapshot snapshot;
    snapshot.timestamp = std::chrono::system_clock::now();
    snapshot.json_data = json_data;
    
    std::lock_guard<std::mutex> lock(state_mutex_);
    
    state_history_.push(snapshot);
    
    // Maintain history size limit
    while (state_history_.size() > max_history_size_) {
        state_history_.pop();
    }
}

std::string StateMonitor::generateSystemStateJson() const {
    std::ostringstream json;
    
    // Get all sessions
    auto sessions = session_manager_.getAllSessions();
    auto active_sessions = session_manager_.getActiveSessions();
    
    // Current timestamp
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()).count();
    
    json << "{\n";
    json << "  \"timestamp\": " << timestamp << ",\n";
    json << "  \"timestamp_iso\": \"" << timeToString(timestamp) << "\",\n";
    
    // System information
    json << "  \"system\": {\n";
    json << "    \"pid\": " << getpid() << ",\n";
    json << "    \"uptime\": \"" << durationToString(timestamp) << "\",\n";
    json << "    \"version\": \"1.0.0\"\n";
    json << "  },\n";
    
    // Session statistics
    json << "  \"statistics\": {\n";
    json << "    \"total_sessions\": " << sessions.size() << ",\n";
    json << "    \"active_sessions\": " << active_sessions.size() << ",\n";
    
    // Count sessions by state
    std::map<SessionState, int> state_counts;
    uint64_t total_bytes_sent = 0, total_bytes_received = 0;
    uint32_t total_packets_sent = 0, total_packets_received = 0;
    
    for (const auto& [session_id, info] : sessions) {
        state_counts[info.state]++;
        total_bytes_sent += info.bytes_sent;
        total_bytes_received += info.bytes_received;
        total_packets_sent += info.packets_sent;
        total_packets_received += info.packets_received;
    }
    
    json << "    \"states\": {\n";
    json << "      \"idle\": " << state_counts[SessionState::IDLE] << ",\n";
    json << "      \"connecting\": " << state_counts[SessionState::CONNECTING] << ",\n";
    json << "      \"authenticating\": " << state_counts[SessionState::AUTHENTICATING] << ",\n";
    json << "      \"establishing\": " << state_counts[SessionState::ESTABLISHING] << ",\n";
    json << "      \"established\": " << state_counts[SessionState::ESTABLISHED] << ",\n";
    json << "      \"rekeying\": " << state_counts[SessionState::REKEYING] << ",\n";
    json << "      \"deleting\": " << state_counts[SessionState::DELETING] << ",\n";
    json << "      \"failed\": " << state_counts[SessionState::FAILED] << ",\n";
    json << "      \"disconnected\": " << state_counts[SessionState::DISCONNECTED] << "\n";
    json << "    },\n";
    
    json << "    \"traffic\": {\n";
    json << "      \"total_bytes_sent\": " << total_bytes_sent << ",\n";
    json << "      \"total_bytes_received\": " << total_bytes_received << ",\n";
    json << "      \"total_packets_sent\": " << total_packets_sent << ",\n";
    json << "      \"total_packets_received\": " << total_packets_received << "\n";
    json << "    }\n";
    json << "  },\n";
    
    // Active sessions details
    json << "  \"sessions\": [\n";
    bool first_session = true;
    for (const auto& [session_id, info] : sessions) {
        if (!first_session) {
            json << ",\n";
        }
        first_session = false;
        
        json << "    " << formatSessionInfo(info);
    }
    json << "\n  ]\n";
    
    json << "}";
    
    return json.str();
}

std::string StateMonitor::formatSessionInfo(const SessionInfo& info) const {
    std::ostringstream json;
    
    json << "{\n";
    json << "      \"session_id\": \"" << info.session_id << "\",\n";
    json << "      \"state\": \"" << sessionStateToString(info.state) << "\",\n";
    json << "      \"local_id\": \"" << info.local_id << "\",\n";
    json << "      \"remote_id\": \"" << info.remote_id << "\",\n";
    json << "      \"remote_addr\": \"" << info.remote_addr << "\",\n";
    json << "      \"remote_port\": " << info.remote_port << ",\n";
    json << "      \"created_at\": " << info.created_at << ",\n";
    json << "      \"last_activity\": " << info.last_activity << ",\n";
    json << "      \"uptime\": \"" << durationToString(info.created_at) << "\",\n";
    
    if (!info.error_message.empty()) {
        json << "      \"error_message\": \"" << info.error_message << "\",\n";
    }
    
    json << "      \"ike_sa\": {\n";
    json << "        \"spi_i\": \"0x" << std::hex << info.ike_spi_i << std::dec << "\",\n";
    json << "        \"spi_r\": \"0x" << std::hex << info.ike_spi_r << std::dec << "\"\n";
    json << "      },\n";
    
    json << "      \"child_sa\": {\n";
    json << "        \"esp_spi_in\": \"0x" << std::hex << info.esp_spi_in << std::dec << "\",\n";
    json << "        \"esp_spi_out\": \"0x" << std::hex << info.esp_spi_out << std::dec << "\"\n";
    json << "      },\n";
    
    json << "      \"statistics\": {\n";
    json << "        \"bytes_sent\": " << info.bytes_sent << ",\n";
    json << "        \"bytes_received\": " << info.bytes_received << ",\n";
    json << "        \"packets_sent\": " << info.packets_sent << ",\n";
    json << "        \"packets_received\": " << info.packets_received << "\n";
    json << "      }\n";
    json << "    }";
    
    return json.str();
}

std::string StateMonitor::formatSystemStats() const {
    std::ostringstream json;
    
    // Get system resource usage
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    
    json << "{\n";
    json << "  \"process\": {\n";
    json << "    \"pid\": " << getpid() << ",\n";
    json << "    \"memory\": {\n";
    json << "      \"rss_kb\": " << usage.ru_maxrss << ",\n";
    json << "      \"page_faults\": " << usage.ru_majflt << "\n";
    json << "    },\n";
    json << "    \"cpu\": {\n";
    json << "      \"user_time_ms\": " << (usage.ru_utime.tv_sec * 1000 + usage.ru_utime.tv_usec / 1000) << ",\n";
    json << "      \"system_time_ms\": " << (usage.ru_stime.tv_sec * 1000 + usage.ru_stime.tv_usec / 1000) << "\n";
    json << "    }\n";
    json << "  }\n";
    json << "}";
    
    return json.str();
}

std::string StateMonitor::timeToString(time_t timestamp) const {
    std::ostringstream ss;
    ss << std::put_time(std::gmtime(&timestamp), "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

std::string StateMonitor::durationToString(time_t start_time) const {
    time_t now = time(nullptr);
    time_t duration = now - start_time;
    
    int hours = duration / 3600;
    int minutes = (duration % 3600) / 60;
    int seconds = duration % 60;
    
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    
    return ss.str();
}

} // namespace OpenIKEv2
