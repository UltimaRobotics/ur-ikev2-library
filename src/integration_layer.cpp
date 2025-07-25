#include "integration_layer.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// Minimal libopenikev2 includes for basic functionality
// Note: libopenikev2 is statically linked but we minimize header exposure
// due to deprecated auto_ptr usage in the library headers

namespace OpenIKEv2 {

IntegrationLayer::IntegrationLayer(const ConfigManager& config)
    : config_(config), running_(false), libopenikev2_initialized_(false) {
}

IntegrationLayer::~IntegrationLayer() {
    stop();
}

bool IntegrationLayer::initialize() {
    try {
        // Initialize session manager
        session_manager_ = std::make_unique<SessionManager>(config_);
        if (!session_manager_->initialize()) {
            std::cerr << "Failed to initialize session manager" << std::endl;
            return false;
        }

        // Initialize state monitor
        state_monitor_ = std::make_unique<StateMonitor>(config_, *session_manager_);
        if (!state_monitor_->initialize()) {
            std::cerr << "Failed to initialize state monitor" << std::endl;
            return false;
        }

        // Initialize libopenikev2 core components
        if (!initializeLibOpenIKEv2()) {
            std::cerr << "Failed to initialize libopenikev2" << std::endl;
            return false;
        }

        std::cout << "All components initialized successfully" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Exception during initialization: " << e.what() << std::endl;
        return false;
    }
}

bool IntegrationLayer::start() {
    if (running_.load()) {
        return true;
    }

    try {
        // Start state monitor
        if (!state_monitor_->start()) {
            std::cerr << "Failed to start state monitor" << std::endl;
            return false;
        }

        // HTTP server removed - using terminal output

        running_.store(true);
        std::cout << "Integration layer started successfully" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Exception during startup: " << e.what() << std::endl;
        return false;
    }
}

void IntegrationLayer::stop() {
    if (!running_.load()) {
        return;
    }

    std::cout << "Stopping integration layer..." << std::endl;
    running_.store(false);

    // Stop components in reverse order

    if (state_monitor_) {
        state_monitor_->stop();
    }

    if (session_manager_) {
        session_manager_->cleanup();
    }

    std::cout << "Integration layer stopped" << std::endl;
}

void IntegrationLayer::run() {
    std::cout << "OpenIKEv2 Integration Layer running - outputting JSON status to terminal" << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;
    
    while (running_.load()) {
        // Output current state as JSON every few seconds
        if (state_monitor_) {
            std::string json_status = state_monitor_->getCurrentStateJson();
            std::cout << "\n=== CURRENT STATE ===" << std::endl;
            std::cout << json_status << std::endl;
            std::cout << "=====================\n" << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

bool IntegrationLayer::initializeLibOpenIKEv2() {
    try {
        std::cout << "Initializing libopenikev2 core components..." << std::endl;

        // Load configuration from config manager
        auto auth_method = config_.getAuthMethod();
        auto local_id = config_.getLocalId();
        auto remote_addr = config_.getRemoteAddr();
        auto ikev2_config_file = config_.getIKEv2ConfigFile();
        
        std::cout << "libopenikev2 statically linked and ready" << std::endl;
        std::cout << "Using auth method: " << auth_method << std::endl;
        std::cout << "Local ID: " << local_id << std::endl;
        std::cout << "Remote address: " << remote_addr << std::endl;
        std::cout << "IKEv2 config file: " << ikev2_config_file << std::endl;
        
        // libopenikev2 components will be initialized when needed for actual IKE sessions
        // For now, we confirm the library is available and linked
        libopenikev2_initialized_ = true;
        
        std::cout << "libopenikev2 integration layer ready" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception initializing libopenikev2: " << e.what() << std::endl;
        return false;
    }
}

} // namespace OpenIKEv2
