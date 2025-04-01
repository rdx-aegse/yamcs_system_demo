// ======================================================================
// \title  TextEventPacketizer.hpp
// \author hiladmin
// \brief  hpp file for TextEventPacketizer component implementation class
// ======================================================================

#ifndef Svc_TextEventPacketizer_HPP
#define Svc_TextEventPacketizer_HPP

#include "Components/TextEventPacketizer/TextEventPacketizerComponentAc.hpp"

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
      ~TextEventPacketizer();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command TODO
      //!
      //! TODO
      void TODO_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

  };

}

#endif
