#include <iostream>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "config/config.h"
#include "mgr/diag_mgr.h"

using namespace std;
using namespace ibn;

/* enum class T { */
/*     domain, */
/*     subdomain */
/* }; */

int main()
{
    /* std::unordered_map<T, std::unordered_map<string, string>> m; */


    /* auto cfg = Config::NewConfigFromFile(DiagMgr::path_); */
    
    /* Document& dom = cfg->dom_; */

    /* Value& domain = dom["domain"]; */

    /* m[T::domain]["perfectly_legal"] = domain["perfectly_legal"].GetString(); */

    /* cout << m[T::domain]["perfectly_legal"] << endl; */
    
    /* if(dom.HasMember("subdomain")) { */
    /*     cout << "true" << endl; */
    /* } */

    auto dmgr = DiagMgr::NewDiagMgr();

    std::string str = "partly_illegal";

    cout << dmgr->GetDiag(Diag::domain, str) << endl;

    return 0;
}

