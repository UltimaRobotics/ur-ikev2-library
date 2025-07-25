#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <memory>
#include <thread>
#include <atomic>
#include "config_manager.hpp"

// Forward declaration to avoid including the full httplib header
namespace httplib {
    class Server;
}

namespace OpenIKEv2 {

class IntegrationLayer; // Forward declaration

class HttpServer {
public:
    HttpServer(const ConfigManager& config, IntegrationLayer& integration_layer);
    ~HttpServer();

    bool initialize();
    bool start();
    void stop();

private:
    const ConfigManager& config_;
    IntegrationLayer& integration_layer_;
    
    std::unique_ptr<httplib::Server> server_;
    std::thread server_thread_;
    std::atomic<bool> running_;
    
    // Route handlers
    void setupRoutes();
    void handleGetStatus(const httplib::Request& req, httplib::Response& res);
    void handleGetSessions(const httplib::Request& req, httplib::Response& res);
    void handleGetSession(const httplib::Request& req, httplib::Response& res);
    void handleCreateSession(const httplib::Request& req, httplib::Response& res);
    void handleStartSession(const httplib::Request& req, httplib::Response& res);
    void handleStopSession(const httplib::Request& req, httplib::Response& res);
    void handleDeleteSession(const httplib::Request& req, httplib::Response& res);
    void handleGetStates(const httplib::Request& req, httplib::Response& res);
    void handleGetStateHistory(const httplib::Request& req, httplib::Response& res);
    void handleGetSystemStats(const httplib::Request& req, httplib::Response& res);
    void handleGetDashboard(const httplib::Request& req, httplib::Response& res);
    
    // Utility methods
    void setCORSHeaders(httplib::Response& res);
    void setJSONHeaders(httplib::Response& res);
    void setHTMLHeaders(httplib::Response& res);
    std::string generateDashboardHTML() const;
    
    // Server thread function
    void serverThreadFunc();
};

} // namespace OpenIKEv2

#endif // HTTP_SERVER_HPP
