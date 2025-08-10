cmake_minimum_required(VERSION 3.16)
project(libopenikev2 VERSION 0.7.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define version
add_definitions(-DLIBOPENIKE_VERSION="0.7")

# Find required packages
find_package(Threads REQUIRED)

# Source files from Makefile.am
set(LIBOPENIKEV2_SOURCES
    src/alarm.cpp
    src/alarmable.cpp
    src/alarmcommand.cpp
    src/alarmcontroller.cpp
    src/alarmcontrollerimpl.cpp
    src/attribute.cpp
    src/attributemap.cpp
    src/authenticator.cpp
    src/autolock.cpp
    src/autovector.cpp
    src/busevent.cpp
    src/buseventchildsa.cpp
    src/buseventcore.cpp
    src/buseventikesa.cpp
    src/busobserver.cpp
    src/bytearray.cpp
    src/bytebuffer.cpp
    src/childsa.cpp
    src/childsacollection.cpp
    src/childsaconfiguration.cpp
    src/childsarequest.cpp
    src/cipher.cpp
    src/closeikesacommand.cpp
    src/command.cpp
    src/condition.cpp
    src/configuration.cpp
    src/configurationattribute.cpp
    src/cryptocontroller.cpp
    src/cryptocontrollerimpl.cpp
    src/diffiehellman.cpp
    src/eappacket.cpp
    src/enums.cpp
    src/eventbus.cpp
    src/exitikesacommand.cpp
    src/generalconfiguration.cpp
    src/id.cpp
    src/idtemplate.cpp
    src/ikesa.cpp
    src/ikesaconfiguration.cpp
    src/ikesacontroller.cpp
    src/ikesacontrollerimpl.cpp
    src/ipaddress.cpp
    src/ipseccontroller.cpp
    src/ipseccontrollerimpl.cpp
    src/keyring.cpp
    src/log.cpp
    src/logimpl.cpp
    src/message.cpp
    src/messagereceivedcommand.cpp
    src/mutex.cpp
    src/networkcontroller.cpp
    src/networkcontrollerimpl.cpp
    src/networkprefix.cpp
    src/notifycontroller.cpp
    src/notifycontroller_authentication_failed.cpp
    src/notifycontroller_cookie.cpp
    src/notifycontroller_http_cert_lookup_supported.cpp
    src/notifycontroller_internal_address_failure.cpp
    src/notifycontroller_invalid_ke.cpp
    src/notifycontroller_invalid_syntax.cpp
    src/notifycontroller_no_additional_sas.cpp
    src/notifycontroller_no_proposal_chosen.cpp
    src/notifycontroller_rekey_sa.cpp
    src/notifycontroller_single_pair_required.cpp
    src/notifycontroller_ts_unacceptable.cpp
    src/notifycontroller_unsupported_critical_payload.cpp
    src/notifycontroller_use_transport_mode.cpp
    src/payload.cpp
    src/payload_auth.cpp
    src/payload_cert.cpp
    src/payload_cert_req.cpp
    src/payload_conf.cpp
    src/payload_del.cpp
    src/payload_eap.cpp
    src/payload_id.cpp
    src/payload_idi.cpp
    src/payload_idr.cpp
    src/payload_ke.cpp
    src/payload_nonce.cpp
    src/payload_notify.cpp
    src/payload_sa.cpp
    src/payload_sk.cpp
    src/payload_ts.cpp
    src/payload_tsi.cpp
    src/payload_tsr.cpp
    src/payload_vendor.cpp
    src/payloadfactory.cpp
    src/peerconfiguration.cpp
    src/printable.cpp
    src/proposal.cpp
    src/pseudorandomfunction.cpp
    src/random.cpp
    src/semaphore.cpp
    src/senddeletechildsareqcommand.cpp
    src/senddeleteikesareqcommand.cpp
    src/sendeapcontinuereqcommand.cpp
    src/sendeapfinishreqcommand.cpp
    src/sendikeauthreqcommand.cpp
    src/sendikesainitreqcommand.cpp
    src/sendinformationalreqcommand.cpp
    src/sendnewchildsareqcommand.cpp
    src/sendrekeychildsareqcommand.cpp
    src/sendrekeyikesareqcommand.cpp
    src/socketaddress.cpp
    src/threadcontroller.cpp
    src/threadcontrollerimpl.cpp
    src/trafficselector.cpp
    src/transform.cpp
    src/transformattribute.cpp
    src/utils.cpp
    src/aaasender.cpp
    src/aaacontroller.cpp
    src/aaacontrollerimpl.cpp
    src/boolattribute.cpp
    src/stringattribute.cpp
    src/int32attribute.cpp
    src/radiusattribute.cpp
)

# Header files from Makefile.am
set(LIBOPENIKEV2_HEADERS
    src/alarm.h
    src/alarmable.h
    src/alarmcommand.h
    src/alarmcontroller.h
    src/alarmcontrollerimpl.h
    src/attribute.h
    src/attributemap.h
    src/authenticator.h
    src/autolock.h
    src/autovector.h
    src/busevent.h
    src/buseventchildsa.h
    src/buseventcore.h
    src/buseventikesa.h
    src/busobserver.h
    src/bytearray.h
    src/bytebuffer.h
    src/childsa.h
    src/childsacollection.h
    src/childsaconfiguration.h
    src/childsarequest.h
    src/cipher.h
    src/closeikesacommand.h
    src/command.h
    src/condition.h
    src/configuration.h
    src/configurationattribute.h
    src/cryptocontroller.h
    src/cryptocontrollerimpl.h
    src/diffiehellman.h
    src/eappacket.h
    src/enums.h
    src/eventbus.h
    src/exception.h
    src/exitikesacommand.h
    src/generalconfiguration.h
    src/id.h
    src/idtemplate.h
    src/ikesa.h
    src/ikesaconfiguration.h
    src/ikesacontroller.h
    src/ikesacontrollerimpl.h
    src/ipaddress.h
    src/ipseccontroller.h
    src/ipseccontrollerimpl.h
    src/keyring.h
    src/log.h
    src/logimpl.h
    src/message.h
    src/messagereceivedcommand.h
    src/mutex.h
    src/networkcontroller.h
    src/networkcontrollerimpl.h
    src/networkprefix.h
    src/notifycontroller.h
    src/notifycontroller_authentication_failed.h
    src/notifycontroller_cookie.h
    src/notifycontroller_http_cert_lookup_supported.h
    src/notifycontroller_internal_address_failure.h
    src/notifycontroller_invalid_ke.h
    src/notifycontroller_invalid_syntax.h
    src/notifycontroller_no_additional_sas.h
    src/notifycontroller_no_proposal_chosen.h
    src/notifycontroller_rekey_sa.h
    src/notifycontroller_single_pair_required.h
    src/notifycontroller_ts_unacceptable.h
    src/notifycontroller_unsupported_critical_payload.h
    src/notifycontroller_use_transport_mode.h
    src/payload.h
    src/payload_auth.h
    src/payload_cert.h
    src/payload_cert_req.h
    src/payload_conf.h
    src/payload_del.h
    src/payload_eap.h
    src/payload_id.h
    src/payload_idi.h
    src/payload_idr.h
    src/payload_ke.h
    src/payload_nonce.h
    src/payload_notify.h
    src/payload_sa.h
    src/payload_sk.h
    src/payload_ts.h
    src/payload_tsi.h
    src/payload_tsr.h
    src/payload_vendor.h
    src/payloadfactory.h
    src/peerconfiguration.h
    src/printable.h
    src/proposal.h
    src/pseudorandomfunction.h
    src/random.h
    src/semaphore.h
    src/senddeletechildsareqcommand.h
    src/senddeleteikesareqcommand.h
    src/sendeapcontinuereqcommand.h
    src/sendeapfinishreqcommand.h
    src/sendikeauthreqcommand.h
    src/sendikesainitreqcommand.h
    src/sendinformationalreqcommand.h
    src/sendnewchildsareqcommand.h
    src/sendrekeychildsareqcommand.h
    src/sendrekeyikesareqcommand.h
    src/socketaddress.h
    src/threadcontroller.h
    src/threadcontrollerimpl.h
    src/trafficselector.h
    src/transform.h
    src/transformattribute.h
    src/utils.h
    src/aaasender.h
    src/boolattribute.h
    src/stringattribute.h
    src/int32attribute.h
    src/radiusattribute.h
    src/aaacontroller.h
    src/aaacontrollerimpl.h
)

# Create config.h
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
    @ONLY
)

# Create static library
add_library(libopenikev2 STATIC ${LIBOPENIKEV2_SOURCES})

# Set library properties
set_target_properties(libopenikev2 PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    OUTPUT_NAME "libopenikev2"
    PREFIX ""
)

# Include directories
target_include_directories(libopenikev2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include/libopenikev2>
)

# Link libraries
target_link_libraries(libopenikev2 PUBLIC Threads::Threads)

# Compiler flags
target_compile_options(libopenikev2 PRIVATE
    -Wall
    -Wextra
    -O2
)

# Add definitions
target_compile_definitions(libopenikev2 PRIVATE HAVE_CONFIG_H)

# Install targets
install(TARGETS libopenikev2
    EXPORT libopenikev2Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES ${LIBOPENIKEV2_HEADERS}
    DESTINATION include/libopenikev2
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/config.h"
    DESTINATION include/libopenikev2
)

# Export targets
install(EXPORT libopenikev2Targets
    FILE libopenikev2Targets.cmake
    NAMESPACE libopenikev2::
    DESTINATION lib/cmake/libopenikev2
)

# Create config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/libopenikev2Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libopenikev2Config.cmake"
    INSTALL_DESTINATION lib/cmake/libopenikev2
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libopenikev2Config.cmake"
    DESTINATION lib/cmake/libopenikev2
)