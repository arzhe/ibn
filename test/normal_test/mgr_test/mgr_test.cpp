#include <iostream>

#include "mgr/policy_mgr.h"
#include "log/logging.h"

using namespace ibn;

int main()
{
    std::cout << "---------- Hello IBN ----------" << std::endl;

    auto pmgr = PolicyMgr("../../ibn_policy.json");

    if(pmgr.HasLink("demo", "Wuhan", "Guangdong")) {
        LOG_INFO << "Demo: Wuhan has a link to Guangdong.";
    }

    if(pmgr.IsLegalBandwidth("demo", 45)) {
        LOG_INFO << "Demo: 45Gb/s is legal to this domain.";
    }

    return 0;
}

