#include "integration_layer.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace OpenIKEv2 {

IntegrationLayer::IntegrationLayer(const ConfigManager& config)
    : config_(config), running_(false) {
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

        // HTTP server removed - using terminal output

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

} // namespace OpenIKEv2
