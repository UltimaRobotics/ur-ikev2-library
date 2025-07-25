#include <iostream>
#include <signal.h>
#include <memory>
#include "integration_layer.hpp"
#include "config_manager.hpp"

std::unique_ptr<OpenIKEv2::IntegrationLayer> integration_layer;

void signal_handler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down gracefully..." << std::endl;
    if (integration_layer) {
        integration_layer->stop();
    }
    exit(0);
}

int main(int argc, char* argv[]) {
    // Setup signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    std::cout << "OpenIKEv2 Integration Layer v1.0.0" << std::endl;
    std::cout << "====================================" << std::endl;

    try {
        // Load configuration
        std::string config_file = "config.json";
        if (argc > 1) {
            config_file = argv[1];
        }

        OpenIKEv2::ConfigManager config(config_file);
        if (!config.load()) {
            std::cerr << "Error: Failed to load configuration from " << config_file << std::endl;
            return 1;
        }

        std::cout << "Configuration loaded from: " << config_file << std::endl;

        // Initialize integration layer
        integration_layer = std::make_unique<OpenIKEv2::IntegrationLayer>(config);
        
        if (!integration_layer->initialize()) {
            std::cerr << "Error: Failed to initialize integration layer" << std::endl;
            return 1;
        }

        std::cout << "Integration layer initialized successfully" << std::endl;

        // Start the integration layer
        if (!integration_layer->start()) {
            std::cerr << "Error: Failed to start integration layer" << std::endl;
            return 1;
        }

        std::cout << "JSON status monitoring active" << std::endl;
        std::cout << "Press Ctrl+C to stop..." << std::endl;

        // Keep running until interrupted
        integration_layer->run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
