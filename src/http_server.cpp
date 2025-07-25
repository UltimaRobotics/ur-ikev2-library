#include "http_server.hpp"
#include "integration_layer.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

// Simple HTTP server implementation without external dependencies
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <regex>

namespace OpenIKEv2 {

// Simple HTTP server implementation
class SimpleHttpServer {
public:
    SimpleHttpServer(const std::string& host, int port) 
        : host_(host), port_(port), server_fd_(-1), running_(false) {}
    
    ~SimpleHttpServer() {
        stop();
    }
    
    bool start() {
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd_ < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return false;
        }
        
        int opt = 1;
        setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port_);
        
        if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Error binding socket to port " << port_ << std::endl;
            close(server_fd_);
            return false;
        }
        
        if (listen(server_fd_, 10) < 0) {
            std::cerr << "Error listening on socket" << std::endl;
            close(server_fd_);
            return false;
        }
        
        running_ = true;
        return true;
    }
    
    void stop() {
        running_ = false;
        if (server_fd_ >= 0) {
            close(server_fd_);
            server_fd_ = -1;
        }
    }
    
    void run(std::function<void(int)> handler) {
        while (running_) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            
            int client_fd = accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);
            if (client_fd < 0) {
                if (running_) {
                    std::cerr << "Error accepting connection" << std::endl;
                }
                continue;
            }
            
            // Handle request in a separate thread
            std::thread([handler, client_fd]() {
                handler(client_fd);
                close(client_fd);
            }).detach();
        }
    }
    
private:
    std::string host_;
    int port_;
    int server_fd_;
    std::atomic<bool> running_;
};

HttpServer::HttpServer(const ConfigManager& config, IntegrationLayer& integration_layer)
    : config_(config), integration_layer_(integration_layer), running_(false) {
    server_ = std::make_unique<SimpleHttpServer>(config_.getServerHost(), config_.getServerPort());
}

HttpServer::~HttpServer() {
    stop();
}

bool HttpServer::initialize() {
    std::cout << "Initializing HTTP server..." << std::endl;
    return true;
}

bool HttpServer::start() {
    if (running_.load()) {
        return true;
    }
    
    if (!server_->start()) {
        std::cerr << "Failed to start HTTP server" << std::endl;
        return false;
    }
    
    running_.store(true);
    server_thread_ = std::thread(&HttpServer::serverThreadFunc, this);
    
    std::cout << "HTTP server started on " << config_.getServerHost() 
              << ":" << config_.getServerPort() << std::endl;
    return true;
}

void HttpServer::stop() {
    if (!running_.load()) {
        return;
    }
    
    running_.store(false);
    server_->stop();
    
    if (server_thread_.joinable()) {
        server_thread_.join();
    }
    
    std::cout << "HTTP server stopped" << std::endl;
}

void HttpServer::serverThreadFunc() {
    server_->run([this](int client_fd) {
        // Read HTTP request
        char buffer[4096];
        ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            return;
        }
        
        buffer[bytes_read] = '\0';
        std::string request(buffer);
        
        // Parse HTTP method and path
        std::istringstream iss(request);
        std::string method, path, version;
        iss >> method >> path >> version;
        
        std::string response_body;
        std::string content_type = "application/json";
        int status_code = 200;
        
        // Route handling
        if (method == "GET") {
            if (path == "/" || path == "/dashboard") {
                response_body = generateDashboardHTML();
                content_type = "text/html";
            } else if (path == "/api/status") {
                response_body = integration_layer_.getStateMonitor()->getCurrentStateJson();
            } else if (path == "/api/sessions") {
                response_body = "{\"sessions\": []}"; // Simplified
            } else if (path.find("/api/sessions/") == 0) {
                std::string session_id = path.substr(14);
                response_body = integration_layer_.getStateMonitor()->getSessionStateJson(session_id);
            } else if (path == "/api/states") {
                response_body = integration_layer_.getStateMonitor()->getCurrentStateJson();
            } else if (path == "/api/states/history") {
                auto history = integration_layer_.getStateMonitor()->getStateHistory(10);
                std::ostringstream json;
                json << "{\"history\": [";
                for (size_t i = 0; i < history.size(); ++i) {
                    if (i > 0) json << ",";
                    json << history[i].json_data;
                }
                json << "]}";
                response_body = json.str();
            } else if (path == "/api/stats") {
                response_body = integration_layer_.getStateMonitor()->getSystemStatsJson();
            } else {
                status_code = 404;
                response_body = "{\"error\": \"Not found\"}";
            }
        } else if (method == "POST") {
            if (path == "/api/sessions") {
                // Create new session
                std::string session_id = integration_layer_.getSessionManager()->createSession("192.168.1.1", 500);
                response_body = "{\"session_id\": \"" + session_id + "\", \"status\": \"created\"}";
            } else if (path.find("/api/sessions/") == 0 && path.find("/start") != std::string::npos) {
                std::string session_id = path.substr(14, path.find("/start") - 14);
                bool success = integration_layer_.getSessionManager()->startSession(session_id);
                response_body = "{\"session_id\": \"" + session_id + "\", \"status\": \"" + 
                               (success ? "started" : "failed") + "\"}";
            } else if (path.find("/api/sessions/") == 0 && path.find("/stop") != std::string::npos) {
                std::string session_id = path.substr(14, path.find("/stop") - 14);
                bool success = integration_layer_.getSessionManager()->stopSession(session_id);
                response_body = "{\"session_id\": \"" + session_id + "\", \"status\": \"" + 
                               (success ? "stopped" : "failed") + "\"}";
            } else {
                status_code = 404;
                response_body = "{\"error\": \"Not found\"}";
            }
        } else if (method == "DELETE") {
            if (path.find("/api/sessions/") == 0) {
                std::string session_id = path.substr(14);
                bool success = integration_layer_.getSessionManager()->deleteSession(session_id);
                response_body = "{\"session_id\": \"" + session_id + "\", \"status\": \"" + 
                               (success ? "deleted" : "failed") + "\"}";
            } else {
                status_code = 404;
                response_body = "{\"error\": \"Not found\"}";
            }
        } else {
            status_code = 405;
            response_body = "{\"error\": \"Method not allowed\"}";
        }
        
        // Send HTTP response
        std::ostringstream response;
        response << "HTTP/1.1 " << status_code << " OK\r\n";
        response << "Content-Type: " << content_type << "\r\n";
        response << "Content-Length: " << response_body.length() << "\r\n";
        response << "Access-Control-Allow-Origin: *\r\n";
        response << "Access-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\n";
        response << "Access-Control-Allow-Headers: Content-Type\r\n";
        response << "\r\n";
        response << response_body;
        
        std::string response_str = response.str();
        send(client_fd, response_str.c_str(), response_str.length(), 0);
    });
}

std::string HttpServer::generateDashboardHTML() const {
    // Read the dashboard HTML file
    std::ifstream file("index.html");
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    // Fallback HTML if file not found
    return R"(
<!DOCTYPE html>
<html>
<head>
    <title>OpenIKEv2 Integration Layer</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .container { max-width: 1200px; margin: 0 auto; }
        .status { background: #f0f0f0; padding: 10px; border-radius: 5px; margin: 10px 0; }
        .session { border: 1px solid #ccc; padding: 10px; margin: 10px 0; border-radius: 5px; }
        .established { border-color: #4CAF50; background-color: #f8fff8; }
        .failed { border-color: #f44336; background-color: #fff8f8; }
        button { background: #2196F3; color: white; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; }
        button:hover { background: #1976D2; }
    </style>
</head>
<body>
    <div class="container">
        <h1>OpenIKEv2 Integration Layer Dashboard</h1>
        <div id="status" class="status">Loading...</div>
        <h2>Sessions</h2>
        <button onclick="createSession()">Create New Session</button>
        <div id="sessions"></div>
    </div>
    
    <script>
        function updateStatus() {
            fetch('/api/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('status').innerHTML = 
                        '<h3>System Status</h3>' +
                        '<p>Active Sessions: ' + data.statistics.active_sessions + '</p>' +
                        '<p>Total Sessions: ' + data.statistics.total_sessions + '</p>' +
                        '<p>Uptime: ' + data.system.uptime + '</p>';
                    updateSessions(data.sessions);
                });
        }
        
        function updateSessions(sessions) {
            const container = document.getElementById('sessions');
            container.innerHTML = '';
            sessions.forEach(session => {
                const div = document.createElement('div');
                div.className = 'session ' + session.state.toLowerCase();
                div.innerHTML = 
                    '<h4>Session: ' + session.session_id + '</h4>' +
                    '<p>State: ' + session.state + '</p>' +
                    '<p>Remote: ' + session.remote_addr + ':' + session.remote_port + '</p>' +
                    '<p>Uptime: ' + session.uptime + '</p>' +
                    '<button onclick="startSession(\'' + session.session_id + '\')">Start</button> ' +
                    '<button onclick="stopSession(\'' + session.session_id + '\')">Stop</button> ' +
                    '<button onclick="deleteSession(\'' + session.session_id + '\')">Delete</button>';
                container.appendChild(div);
            });
        }
        
        function createSession() {
            fetch('/api/sessions', { method: 'POST' })
                .then(response => response.json())
                .then(data => {
                    console.log('Session created:', data);
                    updateStatus();
                });
        }
        
        function startSession(sessionId) {
            fetch('/api/sessions/' + sessionId + '/start', { method: 'POST' })
                .then(response => response.json())
                .then(data => {
                    console.log('Session started:', data);
                    updateStatus();
                });
        }
        
        function stopSession(sessionId) {
            fetch('/api/sessions/' + sessionId + '/stop', { method: 'POST' })
                .then(response => response.json())
                .then(data => {
                    console.log('Session stopped:', data);
                    updateStatus();
                });
        }
        
        function deleteSession(sessionId) {
            fetch('/api/sessions/' + sessionId, { method: 'DELETE' })
                .then(response => response.json())
                .then(data => {
                    console.log('Session deleted:', data);
                    updateStatus();
                });
        }
        
        // Update status every 2 seconds
        setInterval(updateStatus, 2000);
        updateStatus();
    </script>
</body>
</html>
)";
}

} // namespace OpenIKEv2
