#ifndef _CONFIGMGR_H_
#define _CONFIGMGR_H_
#include "PCH.h"
#include "Constants.h"
#include "boost/algorithm/string.hpp"

/*

	ConfigMgr_sample.ini format example:

[DataBase]					# This is comment
IP address = 10.0.0.68
Login = appuser

# This is comment too
[DDS Server]
DDS IP Address = 10.0.0.62

[Setting]
Cancel Order before new order = No
Cancel (MMOG) Order before new order = No

[Test1]
abc = 100,200

================================================
Output:
DataBase.IP address=10.0.0.68
DataBase.IP address.0=10.0.0.68
DataBase.Login=appuser
DataBase.Login.0=appuser
DDS Server.DDS IP Address=10.0.0.62
DDS Server.DDS IP Address.0=10.0.0.62
Setting.Cancel Order before new order=No
Setting.Cancel Order before new order.0=No
Setting.Cancel (MMOG) Order before new order=No
Setting.Cancel (MMOG) Order before new order.0=No
Test1.abc.0 = 100
Test1.abc.1 = 200

*/

using namespace std;
namespace atu 
{
class Config
{
public: 
	Config();
	Config(const Config &src);
	map<string,string> m_keyValuePairMap;
	string getValue(string key);
	void getValueVector(string key, vector<string> &value_vec);
	void setKeyValuePair(string key,string value);
	void addKeyValuePair(string key,string value);
	void removeKey(string key);
	Config *extractInstanceConfig(int instance);
};

class ConfigMgr 
{
public: 
	ConfigMgr();
	~ConfigMgr();
	map<string,Config*> m_file2ConfigMap;
	string trim(const std::string &str);
	Config* getConfig(string filename);
	Config* readConfig(string filename);
};

}
#endif //_CONFIGMGR_H_
