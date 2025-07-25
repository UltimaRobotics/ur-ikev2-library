# OpenIKEv2 Integration Layer

## Overview

This project is a C++ application that provides an HTTP-based integration layer for OpenIKEv2 VPN functionality. It acts as a bridge between the low-level IKEv2 protocol implementation and higher-level applications or management interfaces. The system includes a web dashboard for monitoring VPN sessions and provides RESTful APIs for programmatic control.

## User Preferences

Preferred communication style: Simple, everyday language.

## System Architecture

### Core Architecture
- **Language**: C++17 with CMake build system
- **Web Framework**: Built-in HTTP server using cpphttplib with OpenSSL support
- **IKEv2 Implementation**: Custom libopenikev2 static library
- **Configuration**: JSON-based configuration management
- **Frontend**: Bootstrap-based web dashboard with real-time updates

### Architectural Pattern
The application follows a modular, service-oriented architecture with clear separation of concerns:
- Integration layer for protocol abstraction
- Session management for connection lifecycle
- State monitoring with real-time updates
- HTTP server for web interface and API endpoints

## Key Components

### Backend Components
1. **Integration Layer** (`integration_layer.cpp`)
   - Abstracts libopenikev2 functionality
   - Handles protocol initialization and configuration
   - Manages communication between HTTP layer and IKEv2 core

2. **Session Manager** (`session_manager.cpp`)
   - Tracks active VPN sessions
   - Manages session lifecycle (connect, disconnect, monitor)
   - Maintains session state and metadata

3. **State Monitor** (`state_monitor.cpp`)
   - Real-time monitoring of VPN connection status
   - Collects metrics and connection statistics
   - Provides data for dashboard updates

4. **Configuration Manager** (`config_manager.cpp`)
   - JSON configuration parsing and validation
   - Runtime configuration updates
   - Manages IKEv2 proposals and authentication settings

5. **HTTP Server** (`http_server.cpp`)
   - RESTful API endpoints
   - Serves web dashboard
   - Handles real-time updates via polling/websockets

### Frontend Components
1. **Web Dashboard** (`index.html`)
   - Bootstrap-based responsive interface
   - Real-time session monitoring
   - Visual status indicators with color coding
   - FontAwesome icons for enhanced UX

### External Library
- **libopenikev2**: Core IKEv2 protocol implementation
  - Provides state machine management
  - Handles message parsing and construction
  - Manages Security Association (SA) lifecycle

## Data Flow

1. **Configuration Loading**: JSON config parsed at startup
2. **IKEv2 Initialization**: Integration layer initializes libopenikev2 with config parameters
3. **HTTP Server Startup**: Web server starts serving dashboard and API endpoints
4. **Session Management**: 
   - API calls trigger session creation/termination
   - Session manager coordinates with integration layer
   - State monitor tracks connection status
5. **Real-time Updates**: Dashboard polls server for status updates

## External Dependencies

### System Dependencies
- **OpenSSL**: For cryptographic operations and HTTPS support
- **pthreads**: Multi-threading support
- **Standard C++ Libraries**: STL containers and algorithms

### Build Dependencies
- **CMake 3.16+**: Build system
- **pkg-config**: Package configuration
- **automake/autoconf/libtool**: For libopenikev2 compilation

### Frontend Dependencies (CDN)
- **Bootstrap 5.1.3**: UI framework
- **FontAwesome 6.0.0**: Icon library

## Deployment Strategy

### Build Process
1. **libopenikev2 Compilation**: Static library built separately
2. **Main Application**: CMake builds with static linking
3. **Installation**: Binary installed to system bin directory

### Configuration Management
- **JSON Configuration**: Single config.json file for all settings
- **Runtime Parameters**: IKEv2 proposals, timeouts, server settings
- **Security Settings**: PSK authentication, encryption algorithms

### Monitoring and Logging
- **Configurable Logging**: File-based logging with level control
- **Real-time Monitoring**: Web dashboard with live status updates
- **Session History**: Configurable history retention for diagnostics

### Security Considerations
- **Authentication**: Pre-shared key (PSK) based IKEv2 authentication
- **Encryption**: AES-256 encryption with SHA-256 integrity
- **Network Security**: Configurable local/remote endpoints
- **HTTPS Support**: OpenSSL integration for secure web interface