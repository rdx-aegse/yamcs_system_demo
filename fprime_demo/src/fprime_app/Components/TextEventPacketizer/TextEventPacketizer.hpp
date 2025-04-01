// ======================================================================
// \title  TextEventPacketizer.hpp
// \author hiladmin
// \brief  hpp file for TextEventPacketizer component implementation class
// ======================================================================

#ifndef Svc_TextEventPacketizer_HPP
#define Svc_TextEventPacketizer_HPP

#include "Components/TextEventPacketizer/TextEventPacketizerComponentAc.hpp"
#include "Components/TextEventPacketizer/TextEventPacket.hpp"
#include <ActiveLoggerImplCfg.hpp> //Copied from ActiveLogger but seems like it should be in cpp instead

namespace Svc {

  class TextEventPacketizer :
    public TextEventPacketizerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct TextEventPacketizer object
      TextEventPacketizer(
          const char* const compName //!< The component name
      );

      //! Destroy TextEventPacketizer object
      virtual ~TextEventPacketizer();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for LogRecv
      //!
      //! Event input port
      void LogRecv_handler(
          FwIndexType portNum, //!< The port number
          FwEventIdType id, //!< Log ID
          Fw::Time& timeTag, //!< Time Tag
          const Fw::LogSeverity& severity, //!< The severity argument
          Fw::TextLogString& text //!< Text of log message
      ) override;

      //! Handler implementation for pingIn
      //!
      //! Ping input port
      void pingIn_handler(
          FwIndexType portNum, //!< The port number
          U32 key //!< Value to return to pinger
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command SET_EVENT_FILTER
      //!
      //! Set filter for reporting events. Events are not stored in component.
      void SET_EVENT_FILTER_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          Svc::TextEventPacketizer_FilterSeverity filterLevel, //!< Filter level
          Svc::TextEventPacketizer_Enabled filterEnabled //!< Filter state
      ) override;

      //! Handler implementation for command SET_ID_FILTER
      //!
      //! Filter a particular ID
      void SET_ID_FILTER_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          U32 ID,
          Svc::TextEventPacketizer_Enabled idFilterEnabled //!< ID filter state
      ) override;

      //! Handler implementation for command DUMP_FILTER_STATE
      //!
      //! Dump the filter states via events
      void DUMP_FILTER_STATE_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined internal interfaces
      // ----------------------------------------------------------------------

      //! Handler implementation for loqQueue
      //!
      //! Internal interface to send log messages to component thread
      void loqQueue_internalInterfaceHandler(
          FwEventIdType id, //!< Log ID
          const Fw::Time& timeTag, //!< Time Tag
          const Fw::LogSeverity& severity, //!< The severity argument
          const Fw::TextLogString& text //!< Text of log message
      ) override;

      // Filter state
      struct t_filterState {
        TextEventPacketizer_Enabled enabled; //<! filter is enabled
      } m_filterState[TextEventPacketizer_FilterSeverity::NUM_CONSTANTS];

      // Working members
      Fw::TextEventPacket m_logPacket; //!< custom packet type for assembling the packet by serialisation
      Fw::ComBuffer m_comBuffer; //!< com buffer for sending event buffers

      // array of filtered event IDs.
      // value of 0 means no entry
      FwEventIdType m_filteredIDs[TELEM_ID_FILTER_SIZE];
  };

}

#endif
