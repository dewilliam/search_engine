#ifndef __BLOOM_H__  
#define __BLOOM_H__ 
#include <iostream>
#include <vector>
using namespace std;
class bloom_filter{
private:
	vector<unsigned int> bitmap;
public:
	bloom_filter(int n);
	/* data */
	unsigned int Hash_01(string& data,unsigned int len);
	unsigned int Hash_02(string& data,unsigned int len);
	unsigned int Hash_03(string& data,unsigned int len);
	unsigned int Hash_04(string& data,unsigned int len);
	unsigned int Hash_05(string& data,unsigned int len);
	unsigned int Hash_06(string& data,unsigned int len);
	unsigned int Hash_07(string& data,unsigned int len);
	unsigned int Hash_08(string& data,unsigned int len);
	unsigned int Hash_09(string& data,unsigned int len);
	unsigned int Hash_10(string& data,unsigned int len);
	unsigned int Hash_11(string& data,unsigned int len);
	//
	unsigned int generate_hash(string&data,unsigned int len,int num);
	void insert(string& data);
	bool check(string& data);
};
#include "bloom_filter.cpp"
#endif
