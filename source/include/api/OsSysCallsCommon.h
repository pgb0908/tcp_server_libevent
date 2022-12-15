//
// Created by bong on 22. 12. 15.
//

#ifndef LIBEVENT_TEST1_OSSYSCALLSCOMMON_H
#define LIBEVENT_TEST1_OSSYSCALLSCOMMON_H

#include <memory>
#include <string>
#include "../common/Platform.h"

namespace Api {
/**
 * SysCallResult holds the rc and errno values resulting from a system call.
 */
    template <typename T> struct SysCallResult {

        /**
         * The return code from the system call.
         */
        T return_value_;

        /**
         * The errno value as captured after the system call.
         */
        int errno_;
    };

    using SysCallIntResult = SysCallResult<int>;
    using SysCallSizeResult = SysCallResult<ssize_t>;
    using SysCallPtrResult = SysCallResult<void*>;
    using SysCallStringResult = SysCallResult<std::string>;
    using SysCallBoolResult = SysCallResult<bool>;
    using SysCallSocketResult = SysCallResult<os_fd_t>;

} // namespace Api

#endif //LIBEVENT_TEST1_OSSYSCALLSCOMMON_H
