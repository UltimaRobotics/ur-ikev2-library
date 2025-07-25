/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2IKESACONTROLLER_H
#define OPENIKEV2IKESACONTROLLER_H

#include "ikesacontrollerimpl.h"

namespace openikev2 {

    /**
     This class allows to perform IkeSa operations using several concrete implementations
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class IkeSaController {

            /****************************** ATTRIBUTES ******************************/
        protected:
            static IkeSaControllerImpl* implementation;       /**< Instance of the implementation of IkeSaController */

            /****************************** METHODS ******************************/
        public:
            /**
             * Establish the IkeSaController concrete implementation
             * @param ike_sa_controller concrete implementation
             */
            static void setImplementation( IkeSaControllerImpl* ike_sa_controller );

            /**
            * Increments the half-opened IKE SAs counter.
            */
            static void incHalfOpenCounter();

            /**
            * Decrements the half-opened IKE SAs counter.
            */
            static void decHalfOpenCounter();

            /**
             * Indicates the the cookie mechanism must be used with the current number of half-opened IKE_SA
             * @return TRUE if cookie mechanism must be used. FALSE otherwise
             */
            static bool useCookies();

            /**
             * Gets the next SPI value
             * @return SPI value
             */
            static uint64_t nextSpi();

            /**
             * Adds an IkeSa to the collection
             * @param ike_sa IkeSa to be added
             */
            static void addIkeSa( auto_ptr<IkeSa> ike_sa );

            /**
            * Request the creation of an CHILD_SA. If exists a suittable IKE_SA between the indicated addresses, then uses it. Otherwise, a new IKE_SA
            * will be created.
            * @param ike_sa_src_addr Source address for the IKE_SA
            * @param ike_sa_dst_addr Destination address for the IKE_SA
            * @param mode IPsec mode
            * @param ipsec_protocol IPsec protocol
            * @param my_traffic_selector My traffic selector
            * @param peer_traffic_selector Peer traffic selector
            */
            static void requestChildSa( IpAddress& ike_sa_src_addr, IpAddress& ike_sa_dst_addr, auto_ptr<ChildSaRequest> child_sa_request);

            static void requestChildSaMobility( IpAddress& ike_sa_src_addr, IpAddress& ike_sa_dst_addr, auto_ptr<ChildSaRequest> child_sa_request, IpAddress& ike_sa_coa_addr, bool is_ha);

            /**
             * Push a Command into an IkeSa by SPI value
             * @param spi IkeSa SPI value
             * @param command The Command to be pushed
             * @param priority The priority. TRUE if it must be inserted in the front and FALSE if it must be inserted in the back
             * @return TRUE if the IKE_SA with the indicated SPI exists. FALSE otherwise
             */
            static bool pushCommandByIkeSaSpi( uint64_t spi, auto_ptr<Command> command, bool priority );

            static IkeSa* getIkeSaByIkeSaSpi( uint64_t spi );


            /**
             * Push a command into an IKE_SA by CHILD_SA SPI value
             * @param spi CHILD_SA SPI value
             * @param command The Command to be pushed
             * @param priority The priority. TRUE if it must be inserted in the front and FALSE if it must be inserted in the back
             * @return TRUE if the IKE_SA with the indicated CHILD_SA SPI exists. FALSE otherwise
             */
            static bool pushCommandByChildSaSpi( uint32_t spi, auto_ptr<Command> command, bool priority );
    };

}

#endif
