#ifndef IBN_PARSE_FSM_H
#define IBN_PARSE_FSM_H

#include <memory>
#include <string>

#include "base/util/util.h"
#include "mgr/policy_mgr.h"
#include "parse/cache.h"

namespace ibn {

class State;

class FSM : public std::enable_shared_from_this<FSM> {
public:
    FSM();
    FSM(std::shared_ptr<PolicyMgr> pmgr);
    FSM(const std::string& path);

    static std::shared_ptr<FSM> NewFSM();

    static std::shared_ptr<FSM> NewFSMFromFile(const std::string& path);

    void Handle(std::string intend);

    void StateChange(std::shared_ptr<State> state);

    const std::string& GetRequest() { return request_; }

    void SetRequest(const std::string& request) { request_ = request; }

    std::shared_ptr<PolicyMgr> pmgr_;

    std::shared_ptr<Cache> cache_;
private:
    std::string request_;

    std::shared_ptr<State> state_;
};

class State {
public:
    virtual ~State() {}

    virtual void Handle(std::shared_ptr<FSM> fsm, std::string intend) = 0;

protected:
    std::weak_ptr<FSM> fsm_;

};

class DomainState : public State {
public:
    void Handle(std::shared_ptr<FSM> fsm, std::string intend);
};

class SubDomainState : public State {
public:
    void Handle(std::shared_ptr<FSM> fsm, std::string intend);   
};

class ServiceState : public State {
public:
    void Handle(std::shared_ptr<FSM> fsm, std::string intend);
};

class SubServiceState : public State {
public:
    void Handle(std::shared_ptr<FSM> fsm, std::string intend);
};

class ConnectState : public State {
public:
    void Handle(std::shared_ptr<FSM> fsm, std::string intend);
};

class SubConnectState : public State {
public:
    void Handle(std::shared_ptr<FSM> fsm, std::string intend);
};

class BandwidthState : public State {
public:
    void Handle(std::shared_ptr<FSM> fsm, std::string intend);
};

class InteractSayings {
public:
    class Similarity {
    public:
        static const int RATIO;
    };

    class KeyWord {
    public:
        static const std::string CONNECT;
        static const std::string DISCONNECT;
        static const std::string TO_AND;
        static const std::string TO;
        static const std::string FROM;
    };

    class Symbol {
    public:
        static const std::string ENDL;
        static const std::string COLON;
    };

    class Character {
    public:
        static const std::string IBN;
        static const std::string HINT;
    };
    class DomainState {
    public:
        static const std::string REQUEST;
    };
    class SubDomainState {
    public:
        static const std::string REQUEST;
        static const std::string HINT;
    };
    class ServiceState {
    public:
        static const std::string REQUEST;
        static const std::string HINT;
    };
    class SubServiceState {
    public:
        static const std::string REQUEST;
        static const std::string HINT1;
        static const std::string HINT2;
    };
    class ConnectState {
    public:
        static const std::string REQUEST;
        static const std::string CHECK;
        static const std::string HINT;
    };
    class SubConnectState {
    public:
        static const std::string REQUEST;
        static const std::string HINT;
    };
    class BandwidthState {
    public:
        static const std::string REQUEST;
    };
};

} // !namespace ibn


#endif // !IBN_PARSE_FSM_H
