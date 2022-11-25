#ifndef IBN_PARSE_FSM_H
#define IBN_PARSE_FSM_H

#include <memory>
#include <string>

#include "parse/cache.h"
#include "mgr/policy_mgr.h"
#include "log/logging.h"
#include "mgr/diag_mgr.h"

namespace ibn {

class Fsm;

class FsmState {
public:
    static void DomainState(std::shared_ptr<Fsm> fsm);
    static void SubDomainState(std::shared_ptr<Fsm> fsm);

    static void ServiceState(std::shared_ptr<Fsm> fsm);
    static void SubServiceState(std::shared_ptr<Fsm> fsm);
    static void ServiceRollbackState(std::shared_ptr<Fsm> fsm);

    static void BandwidthState(std::shared_ptr<Fsm> fsm);
    static void BandwidthRollbackState(std::shared_ptr<Fsm> fsm);
};


using State = void(*)(std::shared_ptr<Fsm>);

class Fsm : public std::enable_shared_from_this<Fsm> {
public:
    static std::shared_ptr<Fsm> NewFsm();

    static std::shared_ptr<Fsm> NewFsmFromFile(const std::string& path);

    void Init();

    void ChangeState(State state) { state_ = state; }

    void Handle(std::string intend);

    void CountIncrement() { ++count_; }
    bool CountTrigger() { return count_ >= 3; }

    void SetReply(const std::string& reply) { reply_ = reply; }
    std::string GetReply() { return reply_; }

    std::string GetIntend() { return intend_; }

    void RestoreDomain(const std::string& domain);
    std::string GetDomainFromCache();

    void RestoreNodePair(const std::string& node1, const std::string& node2);
    std::string GetFirstNodeFromCache();
    std::string GetSecondNodeFromCache();

    std::string GetDiag(Diag diag, const std::string& condition);

    std::shared_ptr<PolicyMgr> pmgr_;

private:
    State state_;
    
    std::shared_ptr<Cache> cache_;
    std::shared_ptr<DiagMgr> dmgr_;

    static int count_;
    std::string reply_;
    std::string intend_;
};

} // !namespace ibn

#endif // !IBN_PARSE_FSM_H
