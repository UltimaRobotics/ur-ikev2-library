#ifndef INTEGRATION_LAYER_HPP
#define INTEGRATION_LAYER_HPP

#include <memory>
#include <atomic>
#include "config_manager.hpp"
#include "session_manager.hpp"
#include "state_monitor.hpp"
#include "http_server.hpp"

namespace OpenIKEv2 {

class IntegrationLayer {
public:
    explicit IntegrationLayer(const ConfigManager& config);
    ~IntegrationLayer();

    bool initialize();
    bool start();
    void stop();
    void run();

    // Get components for API access
    SessionManager* getSessionManager() const { return session_manager_.get(); }
    StateMonitor* getStateMonitor() const { return state_monitor_.get(); }

private:
    const ConfigManager& config_;
    std::unique_ptr<SessionManager> session_manager_;
    std::unique_ptr<StateMonitor> state_monitor_;
    std::unique_ptr<HttpServer> http_server_;
    std::atomic<bool> running_;

    void setupSignalHandlers();
};

} // namespace OpenIKEv2

#endif // INTEGRATION_LAYER_HPP
