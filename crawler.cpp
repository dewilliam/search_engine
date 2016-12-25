#include <iostream>
#include <curl/curl.h>
#include <queue>
#include <functional>
#include "lib/bloom_filter.h"
#include <boost/regex.hpp>
#include <fstream>
#include <sstream>
#define N 1000000000
using namespace std;
//
bloom_filter BF(N);
queue<string> Q;
//
size_t process_data(void *buffer, size_t size, size_t nmemb, void *user_p){
    	FILE *fp = (FILE *)user_p;
    	size_t return_size = fwrite(buffer, size, nmemb, fp);
    	string pattern="http://(?!s[0-9]\\.)[a-zA-Z_0-9/\\.]+\\.(com\\.cn|net\\.cn|com|cn|net|org|biz|info|cc|tv)((/[a-zA-Z_0-9]+)*/?\\.(html|php|jsp|aspx))?";
	boost::regex reg(pattern.c_str());
	string data=(char*)buffer;
    	string::const_iterator start(data.begin());
    	string::const_iterator end(data.end());
    	boost::match_results<string::const_iterator> what;
    	while(boost::regex_search(start,end,what,reg,boost::match_default)){
    		// cout<<what[1].str()<<" => "<<what[2].str()<<what[3].str()<<endl;
    		string next(what[0].first,what[0].second);
    		if(!BF.check(next)){
    			BF.insert(next);
    			Q.push(next);
    		}
    		// cout<<next<<endl;
    		start=what[0].second;
    	}
	return return_size;
}
void get_url(string& file){
	fstream out(file.c_str(),ios::in);
	stringstream buffer;
	buffer<<out.rdbuf();
	string data(buffer.str());
	string pattern="http://(?!s[0-9]\\.)[a-zA-Z_0-9/\\.]+\\.(com\\.cn|net\\.cn|com|cn|net|org|biz|info|cc|tv)((/[a-zA-Z_0-9]+)*/?\\.(html|php|jsp|aspx))?";
	boost::regex reg(pattern.c_str());
    	string::const_iterator start(data.begin());
    	string::const_iterator end(data.end());
    	boost::match_results<string::const_iterator> what;
    	while(boost::regex_search(start,end,what,reg,boost::match_default)){
    		// cout<<what[1].str()<<" => "<<what[2].str()<<what[3].str()<<endl;
    		string next(what[0].first,what[0].second);
    		if(!BF.check(next)){
    			BF.insert(next);
    			Q.push(next);
    		}
    		// cout<<next<<endl;
    		start=what[0].second;
    	}
}
int main(){
	string url="http://www.baidu.com";
	Q.push(url);
	BF.insert(url);
	url="http://www.sina.com";
	Q.push(url);
	BF.insert(url);
	url="http://music.163.com";
	Q.push(url);
	BF.insert(url);
	CURL* curl=curl_easy_init();
	CURLoption option;
	int nums=0;
	while(!Q.empty()){
		string u=Q.front();
		Q.pop();
		nums++;
		if(nums%50==0)
			cout<<nums<<endl;
		//生成存储文件名。。。
		int start=7;
		if(u.substr(0,5)=="https")
			start=8;
		string sub=u.substr(start);
		int end=sub.find("/");
		string file_name=sub.substr(0,end);
		hash<string>name_hash;
		size_t hash_value=name_hash(u);
		stringstream ss;
		ss<<file_name<<hash_value;
		file_name="web_data/"+ss.str();
		//生车结束
		FILE* fp=fopen(file_name.c_str(),"w+");
		curl_easy_setopt(curl,CURLOPT_URL,u.c_str());
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);//1 display info
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT,7);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,process_data);
		curl_easy_perform(curl);
		// get_url(file_name);
		fclose(fp);
	}
	curl_easy_cleanup(curl);
}
