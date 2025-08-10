# OpenIKEv2 Integration Layer

## Overview

This project is a C++ application that provides a terminal-based integration layer for OpenIKEv2 VPN functionality. It acts as a bridge between the low-level IKEv2 protocol implementation and higher-level applications. The system outputs JSON status strings directly to the terminal for real-time monitoring of VPN sessions and system state, supporting loading of IKEv2 client configuration files through JSON configuration references.

## User Preferences

Preferred communication style: Simple, everyday language.

## System Architecture

### Core Architecture
- **Language**: C++17 with CMake build system
- **Output Interface**: Terminal-based JSON status output
- **IKEv2 Implementation**: libopenikev2 built as subdirectory with direct static linking
- **Library Integration**: Direct source code references to libopenikev2 components
- **Configuration**: JSON-based configuration management with IKEv2 client config file support
- **Monitoring**: Real-time JSON status output to terminal with configurable intervals

### Architectural Pattern
The application follows a modular, service-oriented architecture with clear separation of concerns:
- Integration layer for protocol abstraction
- Session management for connection lifecycle
- State monitoring with real-time JSON output
- Terminal-based interface for system monitoring and control

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

5. **Main Application** (`main.cpp`)
   - Terminal-based JSON output interface
   - Real-time status monitoring
   - System initialization and lifecycle management

### External Library
- **libopenikev2**: Core IKEv2 protocol implementation
  - Provides state machine management
  - Handles message parsing and construction
  - Manages Security Association (SA) lifecycle

## Data Flow

1. **Configuration Loading**: JSON config parsed at startup with IKEv2 client config file support
2. **IKEv2 Initialization**: Integration layer initializes libopenikev2 with config parameters
3. **State Monitoring**: Real-time JSON output starts immediately
4. **Session Management**: 
   - Terminal interface displays current session states
   - Session manager coordinates with integration layer
   - State monitor tracks connection status and outputs JSON
5. **Real-time Updates**: Continuous JSON status output to terminal with timestamps

## External Dependencies

### System Dependencies
- **OpenSSL**: For cryptographic operations and HTTPS support
- **pthreads**: Multi-threading support
- **Standard C++ Libraries**: STL containers and algorithms

### Build Dependencies
- **CMake 3.16+**: Build system
- **pkg-config**: Package configuration
- **automake/autoconf/libtool**: For libopenikev2 compilation

### Terminal Output Format
- **JSON Status**: Real-time structured status output
- **ISO Timestamps**: Standardized time formatting
- **Session Statistics**: Comprehensive state tracking

## Deployment Strategy

### Build Process
1. **Unified Build**: libopenikev2 built as CMake subdirectory
2. **Static Linking**: Direct static linking to libopenikev2 target
3. **Single Step**: CMake handles both library and application build
4. **Installation**: Binary installed to system bin directory

### Configuration Management
- **JSON Configuration**: Single config.json file for all settings
- **Runtime Parameters**: IKEv2 proposals, timeouts, server settings
- **Security Settings**: PSK authentication, encryption algorithms

### Monitoring and Logging
- **JSON Output**: Real-time structured status output to terminal
- **Session Tracking**: Live session state monitoring with statistics
- **System Metrics**: Process ID, uptime, and version information

### Security Considerations
- **Authentication**: Pre-shared key (PSK) based IKEv2 authentication
- **Encryption**: AES-256 encryption with SHA-256 integrity
- **Network Security**: Configurable local/remote endpoints
- **Config File Support**: Direct loading of IKEv2 client configuration files

## Recent Changes

### January 2025
- ✓ Removed HTTP server components per user request
- ✓ Implemented terminal-based JSON status output
- ✓ Added support for loading IKEv2 client config files via JSON configuration
- ✓ Fixed libopenikev2 CMake integration (autotools to CMake conversion)
- ✓ Successfully built and deployed terminal-based integration layer
- ✓ Application now outputs structured JSON status with real-time updates
- ✓ **RESTRUCTURED**: libopenikev2 now built as subdirectory with static linking
- ✓ **DIRECT INTEGRATION**: API directly references libopenikev2 source code and library
- ✓ **VALIDATED**: libopenikev2 is properly statically linked and initialized
- ✓ **CONFIGURATION**: Confirmed IKEv2 config file loading and parameter parsing