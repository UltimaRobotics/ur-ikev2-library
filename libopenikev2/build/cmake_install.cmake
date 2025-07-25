# Install script for directory: /home/runner/workspace/libopenikev2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/var/empty/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/nix/store/a0d7m3zn9p2dfa1h7ag9h2wzzr2w25sn-gcc-wrapper-14.2.1.20250322/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/runner/workspace/libopenikev2/build/libopenikev2.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libopenikev2" TYPE FILE FILES
    "/home/runner/workspace/libopenikev2/src/alarm.h"
    "/home/runner/workspace/libopenikev2/src/alarmable.h"
    "/home/runner/workspace/libopenikev2/src/alarmcommand.h"
    "/home/runner/workspace/libopenikev2/src/alarmcontroller.h"
    "/home/runner/workspace/libopenikev2/src/alarmcontrollerimpl.h"
    "/home/runner/workspace/libopenikev2/src/attribute.h"
    "/home/runner/workspace/libopenikev2/src/attributemap.h"
    "/home/runner/workspace/libopenikev2/src/authenticator.h"
    "/home/runner/workspace/libopenikev2/src/autolock.h"
    "/home/runner/workspace/libopenikev2/src/autovector.h"
    "/home/runner/workspace/libopenikev2/src/busevent.h"
    "/home/runner/workspace/libopenikev2/src/buseventchildsa.h"
    "/home/runner/workspace/libopenikev2/src/buseventcore.h"
    "/home/runner/workspace/libopenikev2/src/buseventikesa.h"
    "/home/runner/workspace/libopenikev2/src/busobserver.h"
    "/home/runner/workspace/libopenikev2/src/bytearray.h"
    "/home/runner/workspace/libopenikev2/src/bytebuffer.h"
    "/home/runner/workspace/libopenikev2/src/childsa.h"
    "/home/runner/workspace/libopenikev2/src/childsacollection.h"
    "/home/runner/workspace/libopenikev2/src/childsaconfiguration.h"
    "/home/runner/workspace/libopenikev2/src/childsarequest.h"
    "/home/runner/workspace/libopenikev2/src/cipher.h"
    "/home/runner/workspace/libopenikev2/src/closeikesacommand.h"
    "/home/runner/workspace/libopenikev2/src/command.h"
    "/home/runner/workspace/libopenikev2/src/condition.h"
    "/home/runner/workspace/libopenikev2/src/configuration.h"
    "/home/runner/workspace/libopenikev2/src/configurationattribute.h"
    "/home/runner/workspace/libopenikev2/src/cryptocontroller.h"
    "/home/runner/workspace/libopenikev2/src/cryptocontrollerimpl.h"
    "/home/runner/workspace/libopenikev2/src/diffiehellman.h"
    "/home/runner/workspace/libopenikev2/src/eappacket.h"
    "/home/runner/workspace/libopenikev2/src/enums.h"
    "/home/runner/workspace/libopenikev2/src/eventbus.h"
    "/home/runner/workspace/libopenikev2/src/exception.h"
    "/home/runner/workspace/libopenikev2/src/exitikesacommand.h"
    "/home/runner/workspace/libopenikev2/src/generalconfiguration.h"
    "/home/runner/workspace/libopenikev2/src/id.h"
    "/home/runner/workspace/libopenikev2/src/idtemplate.h"
    "/home/runner/workspace/libopenikev2/src/ikesa.h"
    "/home/runner/workspace/libopenikev2/src/ikesaconfiguration.h"
    "/home/runner/workspace/libopenikev2/src/ikesacontroller.h"
    "/home/runner/workspace/libopenikev2/src/ikesacontrollerimpl.h"
    "/home/runner/workspace/libopenikev2/src/ipaddress.h"
    "/home/runner/workspace/libopenikev2/src/ipseccontroller.h"
    "/home/runner/workspace/libopenikev2/src/ipseccontrollerimpl.h"
    "/home/runner/workspace/libopenikev2/src/keyring.h"
    "/home/runner/workspace/libopenikev2/src/log.h"
    "/home/runner/workspace/libopenikev2/src/logimpl.h"
    "/home/runner/workspace/libopenikev2/src/message.h"
    "/home/runner/workspace/libopenikev2/src/messagereceivedcommand.h"
    "/home/runner/workspace/libopenikev2/src/mutex.h"
    "/home/runner/workspace/libopenikev2/src/networkcontroller.h"
    "/home/runner/workspace/libopenikev2/src/networkcontrollerimpl.h"
    "/home/runner/workspace/libopenikev2/src/networkprefix.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_authentication_failed.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_cookie.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_http_cert_lookup_supported.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_internal_address_failure.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_invalid_ke.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_invalid_syntax.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_no_additional_sas.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_no_proposal_chosen.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_rekey_sa.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_single_pair_required.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_ts_unacceptable.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_unsupported_critical_payload.h"
    "/home/runner/workspace/libopenikev2/src/notifycontroller_use_transport_mode.h"
    "/home/runner/workspace/libopenikev2/src/payload.h"
    "/home/runner/workspace/libopenikev2/src/payload_auth.h"
    "/home/runner/workspace/libopenikev2/src/payload_cert.h"
    "/home/runner/workspace/libopenikev2/src/payload_cert_req.h"
    "/home/runner/workspace/libopenikev2/src/payload_conf.h"
    "/home/runner/workspace/libopenikev2/src/payload_del.h"
    "/home/runner/workspace/libopenikev2/src/payload_eap.h"
    "/home/runner/workspace/libopenikev2/src/payload_id.h"
    "/home/runner/workspace/libopenikev2/src/payload_idi.h"
    "/home/runner/workspace/libopenikev2/src/payload_idr.h"
    "/home/runner/workspace/libopenikev2/src/payload_ke.h"
    "/home/runner/workspace/libopenikev2/src/payload_nonce.h"
    "/home/runner/workspace/libopenikev2/src/payload_notify.h"
    "/home/runner/workspace/libopenikev2/src/payload_sa.h"
    "/home/runner/workspace/libopenikev2/src/payload_sk.h"
    "/home/runner/workspace/libopenikev2/src/payload_ts.h"
    "/home/runner/workspace/libopenikev2/src/payload_tsi.h"
    "/home/runner/workspace/libopenikev2/src/payload_tsr.h"
    "/home/runner/workspace/libopenikev2/src/payload_vendor.h"
    "/home/runner/workspace/libopenikev2/src/payloadfactory.h"
    "/home/runner/workspace/libopenikev2/src/peerconfiguration.h"
    "/home/runner/workspace/libopenikev2/src/printable.h"
    "/home/runner/workspace/libopenikev2/src/proposal.h"
    "/home/runner/workspace/libopenikev2/src/pseudorandomfunction.h"
    "/home/runner/workspace/libopenikev2/src/random.h"
    "/home/runner/workspace/libopenikev2/src/semaphore.h"
    "/home/runner/workspace/libopenikev2/src/senddeletechildsareqcommand.h"
    "/home/runner/workspace/libopenikev2/src/senddeleteikesareqcommand.h"
    "/home/runner/workspace/libopenikev2/src/sendeapcontinuereqcommand.h"
    "/home/runner/workspace/libopenikev2/src/sendeapfinishreqcommand.h"
    "/home/runner/workspace/libopenikev2/src/sendikeauthreqcommand.h"
    "/home/runner/workspace/libopenikev2/src/sendikesainitreqcommand.h"
    "/home/runner/workspace/libopenikev2/src/sendinformationalreqcommand.h"
    "/home/runner/workspace/libopenikev2/src/sendnewchildsareqcommand.h"
    "/home/runner/workspace/libopenikev2/src/sendrekeychildsareqcommand.h"
    "/home/runner/workspace/libopenikev2/src/sendrekeyikesareqcommand.h"
    "/home/runner/workspace/libopenikev2/src/socketaddress.h"
    "/home/runner/workspace/libopenikev2/src/threadcontroller.h"
    "/home/runner/workspace/libopenikev2/src/threadcontrollerimpl.h"
    "/home/runner/workspace/libopenikev2/src/trafficselector.h"
    "/home/runner/workspace/libopenikev2/src/transform.h"
    "/home/runner/workspace/libopenikev2/src/transformattribute.h"
    "/home/runner/workspace/libopenikev2/src/utils.h"
    "/home/runner/workspace/libopenikev2/src/aaasender.h"
    "/home/runner/workspace/libopenikev2/src/boolattribute.h"
    "/home/runner/workspace/libopenikev2/src/stringattribute.h"
    "/home/runner/workspace/libopenikev2/src/int32attribute.h"
    "/home/runner/workspace/libopenikev2/src/radiusattribute.h"
    "/home/runner/workspace/libopenikev2/src/aaacontroller.h"
    "/home/runner/workspace/libopenikev2/src/aaacontrollerimpl.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libopenikev2" TYPE FILE FILES "/home/runner/workspace/libopenikev2/build/config.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libopenikev2/libopenikev2Targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libopenikev2/libopenikev2Targets.cmake"
         "/home/runner/workspace/libopenikev2/build/CMakeFiles/Export/746865a7c1449b060c180c427656fd87/libopenikev2Targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libopenikev2/libopenikev2Targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libopenikev2/libopenikev2Targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libopenikev2" TYPE FILE FILES "/home/runner/workspace/libopenikev2/build/CMakeFiles/Export/746865a7c1449b060c180c427656fd87/libopenikev2Targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libopenikev2" TYPE FILE FILES "/home/runner/workspace/libopenikev2/build/CMakeFiles/Export/746865a7c1449b060c180c427656fd87/libopenikev2Targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libopenikev2" TYPE FILE FILES "/home/runner/workspace/libopenikev2/build/libopenikev2Config.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/runner/workspace/libopenikev2/build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/runner/workspace/libopenikev2/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
