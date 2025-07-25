#include "session_manager.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <random>

namespace OpenIKEv2 {

const char* sessionStateToString(SessionState state) {
    switch (state) {
        case SessionState::IDLE: return "IDLE";
        case SessionState::CONNECTING: return "CONNECTING";
        case SessionState::AUTHENTICATING: return "AUTHENTICATING";
        case SessionState::ESTABLISHING: return "ESTABLISHING";
        case SessionState::ESTABLISHED: return "ESTABLISHED";
        case SessionState::REKEYING: return "REKEYING";
        case SessionState::DELETING: return "DELETING";
        case SessionState::FAILED: return "FAILED";
        case SessionState::DISCONNECTED: return "DISCONNECTED";
        default: return "UNKNOWN";
    }
}

SessionManager::SessionManager(const ConfigManager& config)
    : config_(config), next_session_id_(1) {
}

SessionManager::~SessionManager() {
    cleanup();
}

bool SessionManager::initialize() {
    std::cout << "Initializing session manager..." << std::endl;
    
    // Initialize random number generator for SPIs
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::cout << "Session manager initialized successfully" << std::endl;
    return true;
}

void SessionManager::cleanup() {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    // Stop all active sessions
    for (auto& [session_id, info] : sessions_) {
        if (info.state == SessionState::ESTABLISHED ||
            info.state == SessionState::CONNECTING ||
            info.state == SessionState::AUTHENTICATING ||
            info.state == SessionState::ESTABLISHING) {
            
            std::cout << "Stopping session: " << session_id << std::endl;
            info.state = SessionState::DISCONNECTED;
        }
    }
    
    sessions_.clear();
    std::cout << "Session manager cleanup completed" << std::endl;
}

std::string SessionManager::createSession(const std::string& remote_addr, int remote_port) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    std::string session_id = generateSessionId();
    SessionInfo info;
    initializeSessionInfo(info, session_id, remote_addr, remote_port);
    
    sessions_[session_id] = info;
    
    std::cout << "Created session: " << session_id 
              << " for " << remote_addr << ":" << remote_port << std::endl;
    
    return session_id;
}

bool SessionManager::startSession(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    auto it = sessions_.find(session_id);
    if (it == sessions_.end()) {
        std::cerr << "Session not found: " << session_id << std::endl;
        return false;
    }
    
    SessionInfo& info = it->second;
    
    if (info.state != SessionState::IDLE) {
        std::cerr << "Session " << session_id << " is not in IDLE state" << std::endl;
        return false;
    }
    
    info.state = SessionState::CONNECTING;
    info.last_activity = time(nullptr);
    
    std::cout << "Starting session: " << session_id << std::endl;
    
    // Simulate IKE negotiation phases
    // In a real implementation, this would interface with libopenikev2
    
    // Phase 1: IKE SA establishment
    info.state = SessionState::AUTHENTICATING;
    if (!establishIKESA(session_id)) {
        info.state = SessionState::FAILED;
        info.error_message = "IKE SA establishment failed";
        return false;
    }
    
    // Phase 2: Child SA establishment
    info.state = SessionState::ESTABLISHING;
    if (!establishChildSA(session_id)) {
        info.state = SessionState::FAILED;
        info.error_message = "Child SA establishment failed";
        return false;
    }
    
    info.state = SessionState::ESTABLISHED;
    info.last_activity = time(nullptr);
    
    std::cout << "Session established: " << session_id << std::endl;
    return true;
}

bool SessionManager::stopSession(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    auto it = sessions_.find(session_id);
    if (it == sessions_.end()) {
        std::cerr << "Session not found: " << session_id << std::endl;
        return false;
    }
    
    SessionInfo& info = it->second;
    
    if (info.state == SessionState::DISCONNECTED ||
        info.state == SessionState::FAILED) {
        return true; // Already stopped
    }
    
    info.state = SessionState::DELETING;
    
    // Simulate IKE delete process
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    info.state = SessionState::DISCONNECTED;
    info.last_activity = time(nullptr);
    
    std::cout << "Session stopped: " << session_id << std::endl;
    return true;
}

bool SessionManager::deleteSession(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    auto it = sessions_.find(session_id);
    if (it == sessions_.end()) {
        return false;
    }
    
    // Stop session if it's still active
    if (it->second.state != SessionState::DISCONNECTED &&
        it->second.state != SessionState::FAILED) {
        it->second.state = SessionState::DELETING;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    sessions_.erase(it);
    std::cout << "Session deleted: " << session_id << std::endl;
    return true;
}

std::vector<std::string> SessionManager::getActiveSessions() const {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    std::vector<std::string> active_sessions;
    for (const auto& [session_id, info] : sessions_) {
        if (info.state != SessionState::DISCONNECTED &&
            info.state != SessionState::FAILED) {
            active_sessions.push_back(session_id);
        }
    }
    
    return active_sessions;
}

SessionInfo SessionManager::getSessionInfo(const std::string& session_id) const {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    auto it = sessions_.find(session_id);
    if (it != sessions_.end()) {
        return it->second;
    }
    
    // Return empty session info if not found
    SessionInfo empty_info;
    empty_info.session_id = "";
    empty_info.state = SessionState::DISCONNECTED;
    return empty_info;
}

std::map<std::string, SessionInfo> SessionManager::getAllSessions() const {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    return sessions_;
}

void SessionManager::updateSessionState(const std::string& session_id, SessionState state) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    auto it = sessions_.find(session_id);
    if (it != sessions_.end()) {
        it->second.state = state;
        it->second.last_activity = time(nullptr);
    }
}

void SessionManager::updateSessionStats(const std::string& session_id,
                                       uint64_t bytes_sent, uint64_t bytes_received,
                                       uint32_t packets_sent, uint32_t packets_received) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    auto it = sessions_.find(session_id);
    if (it != sessions_.end()) {
        it->second.bytes_sent += bytes_sent;
        it->second.bytes_received += bytes_received;
        it->second.packets_sent += packets_sent;
        it->second.packets_received += packets_received;
        it->second.last_activity = time(nullptr);
    }
}

void SessionManager::setSessionError(const std::string& session_id, const std::string& error) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    
    auto it = sessions_.find(session_id);
    if (it != sessions_.end()) {
        it->second.state = SessionState::FAILED;
        it->second.error_message = error;
        it->second.last_activity = time(nullptr);
    }
}

std::string SessionManager::generateSessionId() {
    std::stringstream ss;
    ss << "sess_" << std::setfill('0') << std::setw(8) << next_session_id_.fetch_add(1);
    return ss.str();
}

void SessionManager::initializeSessionInfo(SessionInfo& info, const std::string& session_id,
                                         const std::string& remote_addr, int remote_port) {
    info.session_id = session_id;
    info.state = SessionState::IDLE;
    info.local_id = config_.getLocalId();
    info.remote_id = config_.getRemoteId();
    info.remote_addr = remote_addr;
    info.remote_port = remote_port;
    info.created_at = time(nullptr);
    info.last_activity = time(nullptr);
    info.error_message = "";
    
    // Generate random SPIs
    std::random_device rd;
    std::mt19937_64 gen(rd());
    info.ike_spi_i = gen();
    info.ike_spi_r = gen();
    info.esp_spi_in = static_cast<uint32_t>(gen());
    info.esp_spi_out = static_cast<uint32_t>(gen());
    
    // Initialize statistics
    info.bytes_sent = 0;
    info.bytes_received = 0;
    info.packets_sent = 0;
    info.packets_received = 0;
}

bool SessionManager::establishIKESA(const std::string& session_id) {
    // Simulate IKE SA establishment
    // In a real implementation, this would use libopenikev2 APIs
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // Simulate some traffic
    updateSessionStats(session_id, 148, 148, 1, 1); // IKE_SA_INIT exchange
    updateSessionStats(session_id, 284, 132, 1, 1); // IKE_AUTH exchange
    
    return true;
}

bool SessionManager::establishChildSA(const std::string& session_id) {
    // Simulate Child SA establishment
    // In a real implementation, this would use libopenikev2 APIs
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Simulate some traffic
    updateSessionStats(session_id, 92, 92, 1, 1); // CREATE_CHILD_SA exchange
    
    return true;
}

void SessionManager::handleIKEMessage(const std::string& session_id, 
                                     const void* message, size_t length) {
    // Handle incoming IKE messages
    // In a real implementation, this would process IKE protocol messages
    updateSessionStats(session_id, 0, length, 0, 1);
}

} // namespace OpenIKEv2
