// ======================================================================
// @file   TextEventSourceString.hpp
// @author F Prime
// @brief  A string sized for an event log entry
// ======================================================================

#ifndef FW_TEXT_EVENT_SOURCE_STRING_HPP
#define FW_TEXT_EVENT_SOURCE_STRING_HPP

#include <FpConfig.hpp>

#include "Fw/Cfg/SerIds.hpp"
#include "Fw/Types/StringBase.hpp"

namespace Fw {

class TextEventSourceString final : public StringBase {
  public:
    enum {
        SERIALIZED_TYPE_ID = FW_TYPEID_EIGHTY_CHAR_STRING,
        STRING_SIZE = 80,
        SERIALIZED_SIZE = STATIC_SERIALIZED_SIZE(STRING_SIZE)
    };

    TextEventSourceString() : StringBase() { *this = ""; }

    TextEventSourceString(const TextEventSourceString& src) : StringBase() { *this = src; }

    TextEventSourceString(const StringBase& src) : StringBase() { *this = src; }

    TextEventSourceString(const char* src) : StringBase() { *this = src; }

    ~TextEventSourceString() {}

    TextEventSourceString& operator=(const TextEventSourceString& src) {
        (void)StringBase::operator=(src);
        return *this;
    }

    TextEventSourceString& operator=(const StringBase& src) {
        (void)StringBase::operator=(src);
        return *this;
    }

    TextEventSourceString& operator=(const char* src) {
        (void)StringBase::operator=(src);
        return *this;
    }

    const char* toChar() const { return this->m_buf; }

    StringBase::SizeType getCapacity() const { return sizeof this->m_buf; }

  private:
    char m_buf[BUFFER_SIZE(STRING_SIZE)];
};
}  // namespace Fw

#endif
