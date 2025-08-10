/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_id.h"

#include <assert.h>

#include "exception.h"

namespace openikev2 {

    Payload_ID::Payload_ID( auto_ptr<ID> id )
            : Payload( PAYLOAD_NONE, false ) {
        this->id = id;
    }

    Payload_ID::Payload_ID( const Payload_ID& other )
            : Payload( other.type, false ) {
        this->id = other.id->clone();
    }

    auto_ptr<Payload_ID> Payload_ID::parse( ByteBuffer& byte_buffer ) {
        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( payload_length < 9 )
            throw ParsingException( "Payload_ID length cannot be < 9 bytes." );

        // reads ID type
        Enums::ID_TYPE id_type = ( Enums::ID_TYPE ) byte_buffer.readInt8();

        // skips reserved bytes
        byte_buffer.skip( 3 );

        // reads id data
        auto_ptr<ByteArray> id_data = byte_buffer.readByteArray( payload_length - 8 );

        auto_ptr<ID> id ( new ID( id_type, id_data ) );

        return auto_ptr<Payload_ID> ( new Payload_ID( id ) );
    }

    Payload_ID::~Payload_ID() { }

    void Payload_ID::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes payload length
        byte_buffer.writeInt16( 8 + this->id->id_data->size() );

        // writes id type
        byte_buffer.writeInt8( this->id->id_type );

        // writes reserved
        byte_buffer.fillBytes( 3, 0 );

        // writes id data
        byte_buffer.writeByteArray( *this->id->id_data );
    }

    string Payload_ID::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        if ( this->type == PAYLOAD_IDi )
            oss << Printable::generateTabs( tabs ) << "<PAYLOAD_IDi> {\n";
        else if ( this->type == PAYLOAD_IDr )
            oss << Printable::generateTabs( tabs ) << "<PAYLOAD_IDr> {\n";
        else
            assert( 0 );

        oss << this->id->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<Payload> Payload_ID::clone( ) const {
        return auto_ptr<Payload> ( new Payload_ID( *this ) );
    }
}

