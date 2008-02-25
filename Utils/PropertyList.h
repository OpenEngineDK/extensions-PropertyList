
#ifndef _PROPERTYLIST_H_
#define _PROPERTYLIST_H_

#include <Core/Exceptions.h>
#include <string>
#include <map>
#include <Math/Vector.h>

namespace OpenEngine {
namespace Utils {

using namespace std;
using OpenEngine::Core::Exception;
using OpenEngine::Math::Vector;

class KeyNotFound : public Exception {
public:
    KeyNotFound(string key) : Exception(key + " not found") {};
};

class PropertyList {
private:    
    map<string,string> data;
    map<string,map<int,string>* > lists;
    map<string,pair<int,pair<string,void*> > > fetchPointers;
    // key, idx, type, pointer
    string filename;
public:

    PropertyList(string file);
    ~PropertyList();

    void Reload();
    void Save();
    void FetchPointers();
    bool HaveKey(string key);
    string GetString(string key);
    string GetString(string key, int idx);
    float GetFloat(string key, int idx = -1);
    void SetFloat(float f, string key, int idx = -1);
    void SetFloatP(float* p, string key, int idx = -1);
    int GetInt(string key, int idx = -1);
    void SetIntP(int* p, string key, int idx = -1);
    template<int N, class T>
    Vector<N,T> GetVector(string key, int idx = -1);
    int ListSize(string key);
    // set
    void SetString(string key,string value, int idx = -1);
    template<int N, class T>
    void SetVector(string key,Vector<N,T>v, int idx = -1);
    
};

}
}

#endif
