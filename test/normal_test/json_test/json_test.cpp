#include <string>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace std;

string path = "../../ibn_policy.json";

void ParseJson(Document& dom, const std::string& path) {
    FILE* fp = fopen(path.data(), "r");
    char buffer[65536];
    FileReadStream frs(fp, buffer, sizeof(buffer));

    dom.ParseStream(frs);
    fclose(fp);
}

int main() {
    Document dom;

    ParseJson(dom, path);
    /* FILE* fp = fopen("../../ibn_policy.json", "r"); */
    /* char buffer[65536]; */
    /* FileReadStream frs(fp, buffer, sizeof(buffer)); */

    /* dom.ParseStream(frs); */
    /* fclose(fp); */

    Value& domains = dom["domain"];

    Value domain;
    Value topo;
    for(SizeType i = 0; i < domains.Size(); ++i) {
        domain = domains[i];
        if(domain.HasMember("name")) cout << "yes" << endl;
        if(domain.HasMember("topology")) cout << "year" << endl;
        cout << "domain: name        : " << domain["name"].GetString() << endl;
        cout << "domain: bandwidth   : " << domain["bandwidth"].GetString() << endl;
        cout << "domain: description : " << domain["description"].GetString() << endl;

        topo = domain["topology"];
        for(SizeType j = 0; j < topo.Size(); ++j) {
            cout << "domain: topology    : " << endl;
            cout << "                     path: " << topo[j]["path"].GetString() << endl;
        }
        if(i != domains.Size() - 1) {
            cout << "--------------------------------------------------" << endl;
        }

    }

}
