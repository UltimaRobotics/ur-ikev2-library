/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "generalconfiguration.h"
#include "peerconfiguration.h"
#include "mutex.h"
#include "autovector.h"


namespace openikev2 {

    /**
        This class keeps all the libopenikev2 configuration. It follows the Singleton design pattern.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Configuration : public Printable {

            /****************************** ATTRIBUTES ******************************/
        protected:
            static Configuration* instance;                         /**< Singleton unique intance */
            AutoVector<PeerConfiguration> peer_configuration;       /**< Peer configurations */
            auto_ptr<GeneralConfiguration> general_configuration;   /**< General configuration */
            auto_ptr<Mutex> mutex_config;                           /**< Mutex to protect configuration accesses */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Creates a default Configuration.
             */
            Configuration();

        public:
            /**
             * Gets the unique Singleton instance.
             * @return The unique Configuration instance.
             */
            static Configuration& getInstance();

            /**
             * Deletes the unique Configuration instance.
             */
            static void deleteConfiguration();

            /**
             * Changes the GeneralConfiguration of the Configuration.
             * @param general_configuration New GeneralConfiguration to be used.
             */
            virtual void setGeneralConfiguration( auto_ptr<GeneralConfiguration> general_configuration );

            /**
             * Adds a new PeerConfiguration to the Configuration.
             * @param peer_configuration New PeerConfiguration to be added.
             */
            virtual void addPeerConfiguration( auto_ptr<PeerConfiguration> peer_configuration );

            /**
             * Deletes a PeerConfiguration. If PeerConfiguration contains more than one network prefix, the indicated network prefix is removed from the
             * list. If the indicated peer network prefix is the only one contained in the PeerConfiguration, the entire PeerConfiguration is deleted.
             * @param peer_network_prefix Peer Network prefix to be removed.
             */
            virtual void deletePeerConfiguration( const NetworkPrefix& peer_network_prefix );

            /**
             * Gets a copy of the PeerConfiguration to be applied with indicated peer IP address and role.
             * @param ip_address Peer IP address
             * @param role Role (cannot be ANY)
             * @return A copy of the PeerConfiguration.
             */
            virtual auto_ptr<PeerConfiguration> getPeerConfiguration( const IpAddress& ip_address, Enums::ROLE_ID role );

            /**
            * Gets a copy of the GeneralConfiguration.
            * @return Copy of the GeneralConfiguration.
            */
            virtual auto_ptr<GeneralConfiguration> getGeneralConfiguration();

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~Configuration();

    };

};
#endif
