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

        // Initialize HTTP server
        http_server_ = std::make_unique<HttpServer>(config_, *this);
        if (!http_server_->initialize()) {
            std::cerr << "Failed to initialize HTTP server" << std::endl;
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

        // Start HTTP server
        if (!http_server_->start()) {
            std::cerr << "Failed to start HTTP server" << std::endl;
            return false;
        }

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
    if (http_server_) {
        http_server_->stop();
    }

    if (state_monitor_) {
        state_monitor_->stop();
    }

    if (session_manager_) {
        session_manager_->cleanup();
    }

    std::cout << "Integration layer stopped" << std::endl;
}

void IntegrationLayer::run() {
    while (running_.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // namespace OpenIKEv2
