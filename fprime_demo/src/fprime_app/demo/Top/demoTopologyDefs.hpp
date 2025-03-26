// ======================================================================
// \title  demoTopologyDefs.hpp
// \brief required header file containing the required definitions for the topology autocoder
//
// ======================================================================
#ifndef DEMO_DEMOTOPOLOGYDEFS_HPP
#define DEMO_DEMOTOPOLOGYDEFS_HPP

#include "Drv/BlockDriver/BlockDriver.hpp"
#include "Fw/Types/MallocAllocator.hpp"
#include "demo/Top/FppConstantsAc.hpp"
#include "Svc/FramingProtocol/FprimeProtocol.hpp"
#include "Svc/Health/Health.hpp"

// Definitions are placed within a namespace named after the deployment
namespace demo {

/**
 * \brief required type definition to carry state
 *
 * The topology autocoder requires an object that carries state with the name `demo::TopologyState`. Only the type
 * definition is required by the autocoder and the contents of this object are otherwise opaque to the autocoder. The contents are entirely up
 * to the definition of the project. Here, they are derived from command line inputs.
 */
struct TopologyState {
    const CHAR* hostname;
    U16 port;
};

/**
 * \brief required ping constants
 *
 * The topology autocoder requires a WARN and FATAL constant definition for each component that supports the health-ping
 * interface. These are expressed as enum constants placed in a namespace named for the component instance. These
 * are all placed in the PingEntries namespace.
 *
 * Each constant specifies how many missed pings are allowed before a WARNING_HI/FATAL event is triggered. In the
 * following example, the health component will emit a WARNING_HI event if the component instance cmdDisp does not
 * respond for 3 pings and will FATAL if responses are not received after a total of 5 pings.
 *
 * ```c++
 * namespace PingEntries {
 * namespace cmdDisp {
 *     enum { WARN = 3, FATAL = 5 };
 * }
 * }
 * ```
 */
namespace PingEntries {
namespace demo_blockDrv {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_tlmSend {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_cmdDisp {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_cmdSeq {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_eventLogger {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_fileDownlink {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_fileManager {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_fileUplink {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_prmDb {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_rateGroup1 {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_rateGroup2 {
enum { WARN = 3, FATAL = 5 };
}
namespace demo_rateGroup3 {
enum { WARN = 3, FATAL = 5 };
}
}  // namespace PingEntries
}  // namespace demo
#endif
