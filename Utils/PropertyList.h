
#ifndef _PROPERTYLIST_H_
#define _PROPERTYLIST_H_

#include <Core/Exceptions.h>
#include <string>
#include <map>
#include <set>
#include <Math/Vector.h>
#include <Utils/BindingsManager.h>

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
    BindingsManager *mgr;

    map<string,string> data;
    map<string,map<int,string>* > lists;


    set<string> boundPointers;

    string filename;
public:

    PropertyList(string file);
    ~PropertyList();

    set<string> GetBoundKeys();

    string GetFileName();
    void Reload();
	void ReadFile(string afilename);
    void Save();
    void FetchPointers();

    bool HaveKey(string key);
    bool IsList(string key);
    string GetString(string key);
    string GetString(string key, int idx);
    float GetFloat(string key, int idx = -1);
    void SetFloat(float f, string key, int idx = -1);
	float* GetFloatP(string key, int idx = -1);
    void SetFloatP(float* p, string key, int idx = -1);
    int GetInt(string key, int idx = -1);
	int* GetIntP(string key, int idx = -1);
    void SetIntP(int* p, string key, int idx = -1);
    void SetInt(int i, string key, int idx = -1);
	bool GetBool(string key, int idx = -1);
	void SetBoolP(bool* p, string key, int idx = -1);
    void SetBool(bool b, string key, int idx = -1);
    void SetColorP(Vector<4,float>* p, string key, int idx = -1);
    
    BindingsManager* GetBindingsManager();

    template<int N, class T>
    Vector<N,T> *GetVectorP(string key, int idx = -1);
    template<int N, class T>
    void SetVectorP(Vector<N,T> *p, string key, int idx = -1);
    template<int N, class T>
    Vector<N,T> GetVector(string key, int idx = -1);
    int ListSize(string key);
    // set
    void SetString(string key,string value, int idx = -1);

    template<int N, class T>
    void SetVector(Vector<N,T>v,string key, int idx = -1);
    
	
	static string GroupOf(string key);
	static string NameOf(string key);
    static string RemoveIdx(string key);
    static string KeyWithIndex(string key, int idx);
    static int IndexFromKey(string key);
};

}
}

#endif
