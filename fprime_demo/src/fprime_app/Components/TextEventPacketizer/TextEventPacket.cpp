/*
 * TextEventPacket.cpp
 *
 *  Created on: May 24, 2014
 *      Author: Timothy Canham
 */

#include "Components/TextEventPacketizer/TextEventPacket.hpp"
#include <Fw/Types/Assert.hpp>

namespace Fw {

    TextEventPacket::TextEventPacket() : m_id(0) {
        this->m_type = FW_PACKET_UNKNOWN; //Have to use packet unknown type until Fprime agrees to create a new packet type, otherwise GDS may go ahead and process this packet wrongly
    }

    TextEventPacket::~TextEventPacket() {
    }

    SerializeStatus TextEventPacket::serialize(SerializeBufferBase& buffer) const {
        //Serialise the packet descriptor/type
        SerializeStatus stat = ComPacket::serializeBase(buffer);
        if (stat != FW_SERIALIZE_OK) {
            return stat;
        }

        //Serialise the event ID
        stat = buffer.serialize(this->m_id);
        if (stat != FW_SERIALIZE_OK) {
            return stat;
        }

        //Serialise the time tag
        stat = buffer.serialize(this->m_timeTag);
        if (stat != FW_SERIALIZE_OK) {
            return stat;
        }

        //Serialise the severity (enum)
        stat = buffer.serialize(this->m_severity);
        if (stat != FW_SERIALIZE_OK) {
            return stat;
        }

        //Serialise the text message without its length. 
        return buffer.serialize(
            reinterpret_cast<const U8*>(this->m_text.toChar()), 
            static_cast<NATIVE_UINT_TYPE>(this->m_text.getCapacity()),
            true
        );
    }

    SerializeStatus TextEventPacket::deserialize(SerializeBufferBase& buffer) {
        //Deserialise packet type
        SerializeStatus stat = deserializeBase(buffer);
        if (stat != FW_SERIALIZE_OK) {
            return stat;
        }

        //Deserialise packet ID
        stat = buffer.deserialize(this->m_id);
        if (stat != FW_SERIALIZE_OK) {
            return stat;
        }

        //Deserialise time tag
        stat = buffer.deserialize(this->m_timeTag);
        if (stat != FW_SERIALIZE_OK) {
            return stat;
        }

        //Deserialise severity (enum)
        stat = buffer.deserialize(this->m_severity);
        if (stat != FW_SERIALIZE_OK) {
            return stat;
        }

        //Deserialize textual event (not serialised with its length).
        this->m_text = "";
        NATIVE_UINT_TYPE text_capacity = this->m_text.getCapacity();
        stat = buffer.deserialize(
            reinterpret_cast<U8*>(const_cast<char*>(this->m_text.toChar())), 
            text_capacity, 
            true
        );
        return stat;
    }

    void TextEventPacket::setId(FwEventIdType id) {
        this->m_id = id;
    }

    void TextEventPacket::setText(const TextLogString& buffer) {
        this->m_text = buffer;
    }

    void TextEventPacket::setTimeTag(const Fw::Time& timeTag) {
        this->m_timeTag = timeTag;
    }

    void TextEventPacket::setSeverity(const Fw::LogSeverity& severity) {
        this->m_severity = severity;
    }

    FwEventIdType TextEventPacket::getId() {
        return this->m_id;
    }

    Fw::Time& TextEventPacket::getTimeTag() {
        return this->m_timeTag;
    }

    TextLogString& TextEventPacket::getText() {
        return this->m_text;
    }

    Fw::LogSeverity& TextEventPacket::getSeverity() {
        return this->m_severity;
    }
} /* namespace Fw */
