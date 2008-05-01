#include <Utils/PropertyList.h>
#include <Resources/File.h>
#include <Logging/Logger.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <Resources/Exceptions.h>
#include <Resources/DirectoryManager.h>

#include <vector>
#include <iostream>
#include <sstream>

#include <Utils/TypeName.h>

namespace OpenEngine {
namespace Utils {

using namespace OpenEngine::Resources;
using namespace boost;    
using namespace std;

    template <typename T> string nameoftype();
    
    
PropertyList::PropertyList(string filename) : filename(filename) {
    Reload();
}

string PropertyList::GetFileName() {
    return filename;
}

void PropertyList::Save() {
    FetchPointers();
    ofstream ofs(filename.c_str());
    for(map<string,string>::iterator itr = data.begin();
        itr != data.end();
        itr++) {
        ofs << (*itr).first << " = " << (*itr).second << endl;
    }
    ofs.close();
}

map<string,pair<int,pair<string,void*> > > PropertyList::GetFetctPointers() {
    return fetchPointers;
}

void PropertyList::FetchPointers() {
    for (map<string,pair<int,pair<string,void*> > >::iterator
             itr = fetchPointers.begin();
         itr != fetchPointers.end();
         itr++) {
        pair<string,pair<int,pair<string,void*> > > elm = *itr;
        string key = elm.first;
        int idx = elm.second.first;
        string type = elm.second.second.first;
        void* p = elm.second.second.second;
        if (type == "float") {
            SetFloat(*((float*)p), key, idx);
        } else {
            logger.error << "Unknown type: " << type << logger.end;
        }
    }
}

void PropertyList::Reload() {
	data.clear();
    lists.clear();

	ReadFile(filename);
}
void PropertyList::ReadFile(string afilename) {
    ifstream* file;
    try {
        string fname = DirectoryManager::FindFileInPath(afilename);
        file = File::Open(fname,ios::binary);
    } catch (ResourceException e) {
        return;
    }
    string line;

    
    while(getline(*file, line)) {
        vector<string> comment;
        split(comment, line, is_any_of("#"));
        
		vector<string> cmd;
		split(cmd, comment[0], is_any_of("*"));
		if (cmd.size() == 2) {
			trim(cmd[0]);
			trim(cmd[1]);
			if (cmd[0] == "include") {
				logger.info << "include [" << cmd[1] << "]" << logger.end;
				ReadFile(cmd[1]);
			}
		}
		else {
			
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

void PropertyList::SetFloat(float f, string key, int idx) {
    ostringstream ost;
    ost << f;    
    SetString(key,ost.str(),idx);   
}

void PropertyList::SetFloatP(float* p, string key, int idx) {
    *p = GetFloat(key,idx);
    fetchPointers.erase(key);
    pair<string,pair<int,pair<string,void*> > > elm;
    elm = make_pair<string,
        pair<int,
        pair<string,
        void*> > >(key,
                   make_pair<int,
                   pair<string,
                   void*> >(idx,
                            make_pair<string,
                            void*>("float",
                                   (void*)p)));

    fetchPointers.insert(elm);
}

int PropertyList::GetInt(string key, int idx) {
    istringstream i(GetString(key,idx));
    int f;
    i >> f;
    return f;
}
	
int* PropertyList::GetIntP(string key, int idx) {
	int* p = new int(0);
	SetIntP(p,key,idx);
	return p;
}

float* PropertyList::GetFloatP(string key, int idx) {
	float* p = new float(0);
	SetFloatP(p,key,idx);
	return p;
}
	
bool PropertyList::GetBool(string key, int idx) {
	string s = GetString(key, idx);
	if (s == "true" || s == "1" )
		return true;
	else 
		return false;
}
void PropertyList::SetBoolP(bool* p, string key, int idx) {
	*p = GetBool(key,idx);
    fetchPointers.erase(key);
    pair<string,pair<int,pair<string,void*> > > elm;
    elm = make_pair<string,
	pair<int,
	pair<string,
	void*> > >(key,
			   make_pair<int,
			   pair<string,
			   void*> >(idx,
						make_pair<string,
						void*>("bool",
							   (void*)p)));
	
    fetchPointers.insert(elm);
	
}
	
template <int N, class T>
Vector<N,T> *PropertyList::GetVectorP(string key, int idx) {
	Vector<N,T>* p = new Vector<N,T>();
	SetVectorP<N,T>(p,key,idx);
	return p;
}

template Vector<3,float> *PropertyList::GetVectorP<3,float>(string,int);

    
template <int N, class T>
void PropertyList::SetVectorP(Vector<N,T>* p, string key, int idx) {
    
    string dim; 
    stringstream out;
    out << N;
    dim = out.str();
    T x;
    dim += typeName(x);
    
    Vector<N,T> vec = GetVector<N,T>(key, idx);

    for (int i =0; i<N; i++) {
        (*p)[i] = vec[i];
    }
    
    fetchPointers.erase(key);
    pair<string,pair<int,pair<string,void*> > > elm;
    elm = make_pair<string,
    pair<int,
    pair<string,
    void*> > >(key,
               make_pair<int,
               pair<string,
               void*> >(idx,
                        make_pair<string,
                        void*>(string("vector") + dim ,
                               (void*)p)));
    
    fetchPointers.insert(elm);
}
	
	
void PropertyList::SetIntP(int* p, string key, int idx) {
	*p = GetInt(key,idx);
    fetchPointers.erase(key);
    pair<string,pair<int,pair<string,void*> > > elm;
    elm = make_pair<string,
	pair<int,
	pair<string,
	void*> > >(key,
			   make_pair<int,
			   pair<string,
			   void*> >(idx,
						make_pair<string,
						void*>("int",
							   (void*)p)));
	
    fetchPointers.insert(elm);
	
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

void PropertyList::SetString(string key, string value, int idx) {
    if (idx == -1) {
        data.erase(key);
        data.insert(make_pair(key,value));
    } else {
        ostringstream ost;
        ost << key << "[" << idx << "]";
        data.insert(make_pair(ost.str(),value));
        map<string,map<int,string>* >::iterator itr = lists.find(key);
        if (itr == lists.end())
            lists.insert(make_pair(key,new map<int,string>()));
        map<int,string>* m = lists.find(key)->second;
        m->erase(idx);
        m->insert(make_pair(idx,value));
    }
}

template<int N, class T>
void PropertyList::SetVector(string key,Vector<N,T> v, int idx) {
    ostringstream ost("v(",ios::ate);
    for (int j=0;j<N;j++) {
        ost << v[j] ;
        if (j+1 != N)
            ost << ",";
    }
    ost << ")";
    
    SetString(key,ost.str(),idx);

}    

template void PropertyList::SetVector<3,float>(string,Vector<3,float>,int);

string PropertyList::GroupOf(string key) {
	unsigned int idx = key.find_last_of(".");
	if (idx == string::npos)
		return string("");
	else
		return key.substr(0, idx);
}
string PropertyList::NameOf(string key) {
	unsigned int idx = key.find_last_of(".");
	if (idx == string::npos)
		return string("");
	else
		return key.substr(idx+1);
}
	
}
}
