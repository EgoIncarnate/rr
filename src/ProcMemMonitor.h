/* -*- Mode: C++; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#ifndef RR_PROC_MEM_MONITOR_H_
#define RR_PROC_MEM_MONITOR_H_

#include "FileMonitor.h"
#include "TaskishUid.h"

namespace rr {

/**
 * A FileMonitor to track writes to /proc/<pid>/mem so they can be replayed
 * when <pid> is a replayed tracee.
 */
class ProcMemMonitor : public FileMonitor {
public:
  ProcMemMonitor(Task* t, const std::string& pathname);

  virtual Type type() { return ProcMem; }

  // We need to PREVENT_SWITCH, since the timing of the write is otherwise
  // unpredictable from our perspective.
  virtual Switchable will_write(Task*) { return PREVENT_SWITCH; }

  /**
   * During replay, copy writes to tracee |tid|'s memory.
   */
  virtual bool needs_offset(Task* t, bool);
  virtual void did_write(Task* t, const std::vector<Range>& ranges,
                         LazyOffset& lazy_offset);

private:
  // 0 if this doesn't object doesn't refer to a tracee's proc-mem.
  TaskUid tuid;
};

} // namespace rr

#endif /* RR_PROC_MEM_MONITOR_H_ */
