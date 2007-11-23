
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
    string filename;
public:

    PropertyList(string file);
    ~PropertyList();

    void Reload();
    string GetString(string key);
    float GetFloat(string key);
    int GetInt(string key);
    template<int N, class T>
    Vector<N,T> GetVector(string key);
};

}
}

#endif
