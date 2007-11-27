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
        vector<string> comment;
        split(comment, line, is_any_of("#"));
        
        vector<string> set;
        split(set, comment[0], is_any_of("="));
        if (set.size() == 2) {
            trim(set[0]);
            trim(set[1]);

            vector<string> arr;
            split(arr, comment[0], is_any_of("[]"));
            if (arr.size() == 3) {
                trim(arr[0]);
                trim(arr[1]);
                istringstream i(arr[1]);
                int idx = 4;
                i >> idx;
                map<string,map<int,string>* >::iterator itr = lists.find(arr[0]);
                if (itr == lists.end())
                    lists.insert(make_pair(arr[0],new map<int,string>()));
                map<int,string>* m = lists.find(arr[0])->second;
                m->insert(make_pair(idx,set[1]));
            }

            data.insert(make_pair(set[0],set[1]));
        }
    }
        
    file->close();
    delete file;
}

PropertyList::~PropertyList() {}

bool PropertyList::HaveKey(string key) {
    map<string,string>::iterator iter = data.find(key);
    if (iter != data.end())
        return true;
    return false;
}

int PropertyList::ListSize(string key) {
    map<string,map<int,string>* >::iterator itr = lists.find(key);
    if (itr == lists.end())
        return 0;
    else 
        return itr->second->size();
}

string PropertyList::GetString(string key) {
    map<string,string>::iterator iter = data.find(key);
    if (iter != data.end()) {
        logger.info << "get [" << key << "] = [" 
                    << iter->second << "]" << logger.end; 
        return iter->second;
    }
    else
        throw KeyNotFound(key);
}
string PropertyList::GetString(string key, int idx) {
    if (idx == -1)
        return GetString(key);
    map<string,map<int,string>* >::iterator iter = lists.find(key);
    if (iter != lists.end()) {
        map<int,string>::iterator itr = iter->second->find(idx);
        if (itr != iter->second->end()) {
            return itr->second;
        }
        else throw KeyNotFound(key);
    }
    else
        throw KeyNotFound(key);
}

float PropertyList::GetFloat(string key, int idx) {
    istringstream i(GetString(key,idx));
    float f;
    i >> f;
    return f;
}

    int PropertyList::GetInt(string key, int idx) {
        istringstream i(GetString(key,idx));
    int f;
    i >> f;
    return f;
}
    
template<int N, class T>
Vector<N,T> PropertyList::GetVector(string key, int idx) {
    Vector<N,T> r;

    istringstream i(GetString(key,idx));
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

    template Vector<3,float> PropertyList::GetVector<3,float>(string,int);
    template Vector<4,float> PropertyList::GetVector<4,float>(string,int);
}
}
