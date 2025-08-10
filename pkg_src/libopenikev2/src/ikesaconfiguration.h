/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef IKESACONFIGURATION_H
#define IKESACONFIGURATION_H

#include "proposal.h"
#include "id.h"
#include "payload_auth.h"
#include "payload_cert.h"
#include "enums.h"
#include "printable.h"
#include "attributemap.h"
#include "bytearray.h"
#include "authenticator.h"
#include "idtemplate.h"

using namespace std;


namespace openikev2 {
    /**
        This class represents an IKE SA configuration
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class IkeSaConfiguration : public Printable {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<Proposal> proposal;                            /**< IKE proposal for this IKE SA */
            AutoVector<IdTemplate> allowed_ids;                     /**< Collection of allowed IDs */

        public:
            auto_ptr<ID> my_id;                                     /**< ID to be used with this IKE SA */
            uint32_t max_idle_time;                                 /**< Maximun idle time without any exchange */
            uint32_t retransmition_time;                            /**< Base retransmition time */
            uint32_t retransmition_factor;                          /**< Factor to be added to retransmition time after each retransmition */
            uint32_t rekey_time;                                    /**< IKE SA lifetime */
            uint32_t ike_max_exchange_retransmitions;               /**< Maximun number of retransmitions */
            auto_ptr<Authenticator> authenticator;                  /**< Authenticator */
            auto_ptr<AttributeMap> attributemap;                    /**< Using this map the class attributes can be extended dynamically */
            string aaa_server_addr;
            uint16_t aaa_server_port;
            string aaa_server_secret;

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new IkeSaConfiguration object, settins its default parameters
             * @param proposal IKE proposal
             */
            IkeSaConfiguration( auto_ptr<Proposal> proposal );

            /**
             * Adds an allowed ID
             * @param id_template Allowed ID template
             */
            virtual void addAllowedId(auto_ptr<IdTemplate> id_template);

            /**
             * Creates a clone of this IkeSaConfiguration object
             * @return A new IkeSaConfiguration object
             */
            virtual auto_ptr<IkeSaConfiguration> clone() const;

            /**
             * Gets the proposal
             * @return IKE proposal
             */
            virtual Proposal& getProposal() const;

            /**
             * Sets the proposal
             * @param proposal New IKE proposal
             */
            virtual void setProposal( auto_ptr<Proposal> proposal );

            /**
             * Obtains the Authenticator
             * @return A reference to the authenticator
             */
            virtual Authenticator& getAuthenticator();

            /**
             * Checks if the ID is allowed for this configuration
             * @param id ID to be checked
             * @return TRUE if the ID is allowed. FALSE otherwise
             */
            virtual bool checkId(const ID& id);

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~IkeSaConfiguration();
    };
}
#endif
