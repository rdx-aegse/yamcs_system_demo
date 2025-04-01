/*
 * TextEventPacket.hpp
 *
 *  Created on: May 24, 2014
 *      Author: Timothy Canham
 */

#ifndef TEXTEVENTPACKET_HPP_
#define TEXTEVENTPACKET_HPP_

#include <Fw/Com/ComPacket.hpp>
#include <Fw/Log/TextLogString.hpp>
#include <Fw/Time/Time.hpp>

namespace Fw {

    class TextEventPacket : public ComPacket {
        public:

            TextEventPacket();
            virtual ~TextEventPacket();

            SerializeStatus serialize(SerializeBufferBase& buffer) const; //!< serialize contents
            SerializeStatus deserialize(SerializeBufferBase& buffer);

            void setId(FwEventIdType id);
            void setTimeTag(const Fw::Time& timeTag);
            void setText(const TextLogString& buffer);
            void setSeverity(const Fw::LogSeverity& severity);

            FwEventIdType getId();
            Fw::Time& getTimeTag();
            TextLogString& getText();
            Fw::LogSeverity& getSeverity();
            

        protected:
            FwEventIdType m_id; // !< event id
            Fw::Time m_timeTag; // !< time tag
            Fw::LogSeverity m_severity; // !< event severity
            Fw::TextLogString m_text; // !< event text string
    };

} /* namespace Fw */

#endif /* LOGPACKET_HPP_ */
