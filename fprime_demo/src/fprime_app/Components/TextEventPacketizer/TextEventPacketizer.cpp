// ======================================================================
// \title  TextEventPacketizer.cpp
// \author hiladmin
// \brief  cpp file for TextEventPacketizer component implementation class
// ======================================================================

#include "Components/TextEventPacketizer/TextEventPacketizer.hpp"

namespace Svc {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  TextEventPacketizer ::
    TextEventPacketizer(const char* const compName) :
      TextEventPacketizerComponentBase(compName)
  {

  }

  TextEventPacketizer ::
    ~TextEventPacketizer()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void TextEventPacketizer ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
