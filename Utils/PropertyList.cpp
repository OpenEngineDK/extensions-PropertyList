#include <Utils/PropertyList.h>
#include <Resources/File.h>
#include <Logging/Logger.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <vector>
#include <iostream>
#include <sstream>
namespace OpenEngine {
namespace Utils {

using OpenEngine::Resources::File;
using namespace boost;    
using namespace std;

PropertyList::PropertyList(string filename) : filename(filename) {
    Reload();
}

void PropertyList::Reload() {
    ifstream* file = File::Open(filename,ios::binary);
    string line;

    data.clear();
    
    while(getline(*file, line)) {
        if (line[0] == '#')
            continue;
        vector<string> set;
        split(set, line, is_any_of("="));
        if (set.size() == 2) {
            trim(set[0]);
            trim(set[1]);
            data.insert(make_pair(set[0],set[1]));
        }
    }
        
    file->close();
    delete file;
}

PropertyList::~PropertyList() {}

string PropertyList::GetString(string key) {
    map<string,string>::iterator iter = data.find(key);
    if (iter != data.end())
        return iter->second;
    else
        throw KeyNotFound(key);
}

float PropertyList::GetFloat(string key) {
    istringstream i(GetString(key));
    float f;
    i >> f;
    return f;
}

int PropertyList::GetInt(string key) {
    istringstream i(GetString(key));
    int f;
    i >> f;
    return f;
}
    
template<int N, class T>
Vector<N,T> PropertyList::GetVector(string key) {
    Vector<N,T> r;

    istringstream i(GetString(key));
    char v; i >> v;
    if (v != 'v')
        throw Exception("not a vector");
    
    i.ignore();
    
    for (int j = 0; j<N; j++) {
        T f;
        i >> f;
        i.ignore();
        r[j] = f;
    }
    return r;
}

    template Vector<3,float> PropertyList::GetVector<3,float>(string key);
    template Vector<4,float> PropertyList::GetVector<4,float>(string key);
}
}
