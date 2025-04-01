// ======================================================================
// \title  TextEventPacketizer.cpp
// \author hiladmin
// \brief  cpp file for TextEventPacketizer component implementation class
// ======================================================================

#include "Components/TextEventPacketizer/TextEventPacketizer.hpp"
#include <Fw/Types/Assert.hpp> //for FW_ASSERT
#include <cstdio> //For memset

namespace Svc {

  typedef TextEventPacketizer_Enabled Enabled;
  typedef TextEventPacketizer_FilterSeverity FilterSeverity;

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  TextEventPacketizer ::
    TextEventPacketizer(const char* const compName) :
      TextEventPacketizerComponentBase(compName)
  {
    // set filter defaults
    this->m_filterState[FilterSeverity::WARNING_HI].enabled =
          FILTER_WARNING_HI_DEFAULT?Enabled::ENABLED:Enabled::DISABLED;
    this->m_filterState[FilterSeverity::WARNING_LO].enabled =
          FILTER_WARNING_LO_DEFAULT?Enabled::ENABLED:Enabled::DISABLED;
    this->m_filterState[FilterSeverity::COMMAND].enabled =
          FILTER_COMMAND_DEFAULT?Enabled::ENABLED:Enabled::DISABLED;
    this->m_filterState[FilterSeverity::ACTIVITY_HI].enabled =
          FILTER_ACTIVITY_HI_DEFAULT?Enabled::ENABLED:Enabled::DISABLED;
    this->m_filterState[FilterSeverity::ACTIVITY_LO].enabled =
          FILTER_ACTIVITY_LO_DEFAULT?Enabled::ENABLED:Enabled::DISABLED;
    this->m_filterState[FilterSeverity::DIAGNOSTIC].enabled =
          FILTER_DIAGNOSTIC_DEFAULT?Enabled::ENABLED:Enabled::DISABLED;

    memset(m_filteredIDs,0,sizeof(m_filteredIDs));
  }

  TextEventPacketizer ::
    ~TextEventPacketizer()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------

  void TextEventPacketizer ::
    LogRecv_handler(
        FwIndexType portNum,
        FwEventIdType id,
        Fw::Time& timeTag,
        const Fw::LogSeverity& severity,
        Fw::TextLogString& text
    )
  {
    // make sure ID is not zero. Zero is reserved for ID filter.
    FW_ASSERT(id != 0);

    switch (severity.e) {
        case Fw::LogSeverity::FATAL: // always pass FATAL
            break;
        case Fw::LogSeverity::WARNING_HI:
            if (this->m_filterState[FilterSeverity::WARNING_HI].enabled == Enabled::DISABLED) {
               return;
            }
            break;
        case Fw::LogSeverity::WARNING_LO:
            if (this->m_filterState[FilterSeverity::WARNING_LO].enabled == Enabled::DISABLED) {
                return;
            }
            break;
        case Fw::LogSeverity::COMMAND:
            if (this->m_filterState[FilterSeverity::COMMAND].enabled == Enabled::DISABLED) {
                return;
            }
            break;
        case Fw::LogSeverity::ACTIVITY_HI:
            if (this->m_filterState[FilterSeverity::ACTIVITY_HI].enabled == Enabled::DISABLED) {
                return;
            }
            break;
        case Fw::LogSeverity::ACTIVITY_LO:
            if (this->m_filterState[FilterSeverity::ACTIVITY_LO].enabled == Enabled::DISABLED) {
                return;
            }
            break;
        case Fw::LogSeverity::DIAGNOSTIC:
            if (this->m_filterState[FilterSeverity::DIAGNOSTIC].enabled == Enabled::DISABLED) {
                return;
            }
            break;
        default:
            FW_ASSERT(0,static_cast<NATIVE_INT_TYPE>(severity.e));
            return;
    }

    // check ID filters
    for (NATIVE_INT_TYPE entry = 0; entry < TELEM_ID_FILTER_SIZE; entry++) {
        if (
          (m_filteredIDs[entry] == id) &&
          (severity != Fw::LogSeverity::FATAL)
          ) {
            return;
        }
    }

    // send event to the logger thread
    this->loqQueue_internalInterfaceInvoke(id,timeTag,severity,text);

    // if connected, announce the FATAL
    if (Fw::LogSeverity::FATAL == severity.e) {
        if (this->isConnected_FatalAnnounce_OutputPort(0)) {
            this->FatalAnnounce_out(0,id);
        }
    }
  }

  void TextEventPacketizer ::
    pingIn_handler(
        FwIndexType portNum,
        U32 key
    )
  {
    // TODO
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void TextEventPacketizer ::
    SET_EVENT_FILTER_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        Svc::TextEventPacketizer_FilterSeverity filterLevel,
        Svc::TextEventPacketizer_Enabled filterEnabled
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void TextEventPacketizer ::
    SET_ID_FILTER_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U32 ID,
        Svc::TextEventPacketizer_Enabled idFilterEnabled
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void TextEventPacketizer ::
    DUMP_FILTER_STATE_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined internal interfaces
  // ----------------------------------------------------------------------

  void TextEventPacketizer ::
    loqQueue_internalInterfaceHandler(
        FwEventIdType id,
        const Fw::Time& timeTag,
        const Fw::LogSeverity& severity,
        const Fw::TextLogString& text
    )
  {
    // Serialize event
    this->m_logPacket.setId(id);
    this->m_logPacket.setTimeTag(timeTag);
    this->m_logPacket.setSeverity(severity);
    this->m_logPacket.setText(text);
    this->m_comBuffer.resetSer();
    Fw::SerializeStatus stat = this->m_logPacket.serialize(this->m_comBuffer);
    FW_ASSERT(Fw::FW_SERIALIZE_OK == stat,static_cast<NATIVE_INT_TYPE>(stat));

    if (this->isConnected_PktSend_OutputPort(0)) {
        this->PktSend_out(0, this->m_comBuffer,0);
    }
  }

}
