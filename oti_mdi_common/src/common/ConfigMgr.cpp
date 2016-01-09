#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "Toolbox.h"
#include "ConfigMgr.h"
using namespace atu;
namespace atu {

Config::Config() {
}

Config::Config(const Config &src) {
	m_keyValuePairMap.insert(src.m_keyValuePairMap.begin(), src.m_keyValuePairMap.end());
}

string Config::getValue(string key)
{
	//old version
	//return m_keyValuePairMap[key];
	//new version, will check if key can be found, or it will return empty string
	if (m_keyValuePairMap.find(key)!=m_keyValuePairMap.end()) {
		return m_keyValuePairMap[key];
	} else {
		return "";
	}
}

void Config::getValueVector(string key, vector<string> &value_vec) {
	string value;
	int i=0;
	do {
		value = getValue(key + "." + Toolbox::itos(i++));
		if (value.length()>0) {
			value_vec.push_back(value);
		}
	} while (value.length()>0);

}
void Config::setKeyValuePair(string key,string value) {
	m_keyValuePairMap[key]=value;
}
void Config::addKeyValuePair(string key,string value) {
	if (m_keyValuePairMap.find(key)!=m_keyValuePairMap.end()) {
	 //throw exception
		cout << "Detected duplicate key " << key << " , program aborted";
		::exit(0);
	} else {
		m_keyValuePairMap[key]=value;
	}
}
void Config::removeKey(string key) {
	if (m_keyValuePairMap.find(key)==m_keyValuePairMap.end()) {
	 //throw exception
		cout << "Removing Non-exist key " << key << " , program aborted";
		::exit(0);
	} else {
		m_keyValuePairMap.erase(key);
	}
}

Config *Config::extractInstanceConfig(int instance) {
	Config *algo_instance_config=new Config(); // Algo will have their own copy of config
//	algo_instance_config = (*m_Config);
//	algo_instance_config.setKeyValuePair(initial_algo_instance->m_algoStrategyName + ".nInstance","1");
//	algo_instance_config.setKeyValuePair(initial_algo_instance->m_algoStrategyName + ".nInstance.0","1");
	map<string,string> *algo_instance_config_map = &m_keyValuePairMap;

	for (map<string,string>::iterator it = algo_instance_config_map->begin(); it != algo_instance_config_map->end(); it++) {
//		if (it->first.find(initial_algo_instance->m_algoStrategyName + ".") != string::npos) {
		vector<string> values_vec;
//				cout << "Changing key = " << it->first << endl;
		getValueVector(it->first,values_vec);
//				cout << "size=" << values_vec.size() << endl;
		if (values_vec.size() == 1) {
//					cout << "DEBUG2 " << it->first << endl;
			algo_instance_config->addKeyValuePair(it->first, values_vec.at(0));
			algo_instance_config->addKeyValuePair(it->first + ".0", values_vec.at(0));
		} else {
			if (values_vec.size()>instance) {
				algo_instance_config->addKeyValuePair(it->first,values_vec.at(instance));
				algo_instance_config->addKeyValuePair(it->first+".0",values_vec.at(instance));
			}
		}
	}
	return algo_instance_config;
}
ConfigMgr::ConfigMgr()
{
}
ConfigMgr::~ConfigMgr()
{
	map<string,Config*>::iterator it;
	for (it=m_file2ConfigMap.begin();it!=m_file2ConfigMap.end();it++) {
		Config *myconfig=(*it).second;
		delete myconfig;
	}
}
std::string ConfigMgr::trim(const std::string &str)
{
    size_t s = str.find_first_not_of(" \n\r\t");
    size_t e = str.find_last_not_of (" \n\r\t");

    if(( string::npos == s) || ( string::npos == e))
        return "";
    else
        return str.substr(s, e-s+1);
}
Config* ConfigMgr::getConfig(string filename)
{
	if (m_file2ConfigMap.find(filename)==m_file2ConfigMap.end()) {
		return readConfig(filename);
	}
	return m_file2ConfigMap[filename];
}
Config* ConfigMgr::readConfig(string filename)
{
	Config *myconfig;
	if (m_file2ConfigMap.find(filename)==m_file2ConfigMap.end()) {
		//ifconfig file not found create it
		myconfig=new Config();
		m_file2ConfigMap[filename]=myconfig;
	} else {
		myconfig=m_file2ConfigMap[filename];
	}
	FILE *file=fopen(filename.c_str(),"r");
	char line[5001];
	char section[1000];
	char dupline[5001];
	strcpy(section,"");
	while(!feof(file)) {
			fgets(line,5000,file);
			if (feof(file)) {
				break;
			}
			//empty the newline character
			if (line[strlen(line)-1]=='\n') {
				line[strlen(line)-1]=0;
			}
			//first filter the section
			if (line[0]=='[') {
				//get the section name
				//char *linetoprocess=strdupa(line);
				char *linetoprocess;
				strcpy(dupline, line);
				linetoprocess = &dupline[0];
				char *mysection=strsep(&linetoprocess,"[");
				mysection=strsep(&linetoprocess,"]");
				strcpy(section,mysection);
				//printf("section %s\n",section);
			}
			char *comment=strstr(line,"#");
			if (comment!=NULL) {
				//empty the comment
				strcpy(comment,"");
			}
			char *linetoprocess=strdupa(line);
			char *key = strsep(&linetoprocess, "=");
			if (key!=NULL && linetoprocess!=NULL) {
			//	printf("key %s value %s\n",key,linetoprocess2);
				char sectionkey[5000];
				sprintf(sectionkey,"%s.%s",section,key);
				myconfig->addKeyValuePair(trim(sectionkey),trim(linetoprocess));
				string value=trim(linetoprocess);


				vector<string> valuelist;
				boost::split(valuelist, value, boost::is_any_of(","));
				//cout << "valuelist.size " << valuelist.size() << endl;
				//if (valuelist.size()>1) {
						for (unsigned int i=0;i<valuelist.size();i++) {
							char sectionkeyvalue[5000];
							sprintf(sectionkeyvalue,"%s.%d",trim(sectionkey).c_str(),i);
							myconfig->addKeyValuePair(sectionkeyvalue,trim(valuelist[i]));
						//	cout << "valuelist[" << i << "] " << valuelist[i] << endl;
						}
				//}
			} else {
				//ignore if = is not found
				continue;
			}
			//myconfig->addKeyValuePair(key,linetoprocess2);

	}
	return m_file2ConfigMap[filename];
}

}
