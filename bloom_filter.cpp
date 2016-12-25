#define BIT_NUM (8*sizeof(unsigned int))
using namespace std;
bloom_filter::bloom_filter(int n){
	bitmap.reserve(n);
}
unsigned int bloom_filter::Hash_01(string& data,unsigned int len){
	unsigned int b    = 378551;    
	unsigned int a    = 63689;    
	unsigned int hash = 0;    
	unsigned int i    = 0;    
	for(i = 0; i < len;i++){
		hash = hash * a + data[i];
		a    = a * b;    
	}    
	return hash;
}
unsigned int bloom_filter::Hash_02(string& data,unsigned int len){
	unsigned int hash = 1315423911;    
	unsigned int i    = 0;    
	for(i = 0; i < len;i++){
		hash ^= ((hash << 5) + data[i] + (hash >> 2));    
	}
	return hash;
}
unsigned int bloom_filter::Hash_03(string& data,unsigned int len){
	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);    
	const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);    
	const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);    
	const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);    
	unsigned int hash              = 0;    
	unsigned int test              = 0;    
	unsigned int i                 = 0;    
	for(i = 0; i < len;i++){
		hash = (hash << OneEighth) + data[i];    
		if((test = hash & HighBits)  != 0){
			hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));    
		}    
	}    
	return hash;
}
unsigned int bloom_filter::Hash_04(string& data,unsigned int len){
	unsigned int hash = 0;    
	unsigned int x    = 0;    
	unsigned int i    = 0;    
	for(i = 0; i < len;i++){
		hash = (hash << 4) + data[i];    
		if((x = hash & 0xF0000000L) != 0){
			hash ^= (x >> 24);
		}    
		hash &= ~x;    
	}    
	return hash;
}
unsigned int bloom_filter::Hash_05(string& data,unsigned int len){
	unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */    
	unsigned int hash = 0;    
	unsigned int i    = 0;    
	for(i = 0; i < len;i++){
		hash = (hash * seed) + data[i];
	}
	return hash;
}
unsigned int bloom_filter::Hash_06(string& data,unsigned int len){
	unsigned int hash = 0;    
	unsigned int i    = 0;    
	for(i = 0; i < len;i++){
		hash = data[i] + (hash << 6) + (hash << 16) - hash;    
	}    
	return hash; 
}
unsigned int bloom_filter::Hash_07(string& data,unsigned int len){
	unsigned int hash = 5381;    
	unsigned int i    = 0;    
	for(i = 0; i < len; i++){
		hash = ((hash << 5) + hash) + data[i];
	}    
	return hash;
}
unsigned int bloom_filter::Hash_08(string& data,unsigned int len){
	unsigned int hash = len;    
	unsigned int i    = 0;    
	for(i = 0; i < len;i++){
		hash = ((hash << 5) ^ (hash >> 27)) ^ data[i];
	}    
	return hash;
}
unsigned int bloom_filter::Hash_09(string& data,unsigned int len){
	unsigned int hash = 0;    
	unsigned int i    = 0;    
	for(i = 0; i < len;i++){
		hash = hash << 7 ^ data[i];
	}    
	return hash; 
}
unsigned int bloom_filter::Hash_10(string& data,unsigned int len){
	const unsigned int fnv_prime = 0x811C9DC5;    
	unsigned int hash      = 0;    
	unsigned int i         = 0;    
	for(i = 0; i < len;i++){
		hash *= fnv_prime;    
		hash ^= data[i];
	}    
	return hash;
}
unsigned int bloom_filter::Hash_11(string& data,unsigned int len){
	unsigned int hash = 0xAAAAAAAA;    
	unsigned int i    = 0;    
	for(i = 0; i < len;i++){
		hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ data[i] * (hash >> 3)):(~((hash << 11) + data[i] ^ (hash >> 5)));
	}
	return hash;
}
unsigned int bloom_filter::generate_hash(string&data,unsigned int len,int num){
	switch(num){
		case 1:
			return this->Hash_01(data,len);
		case 2:
			return this->Hash_02(data,len);
		case 3:
			return this->Hash_03(data,len);
		case 4:
			return this->Hash_04(data,len);
		case 5:
			return this->Hash_05(data,len);
		case 6:
			return this->Hash_06(data,len);
		case 7:
			return this->Hash_07(data,len);
		case 8:
			return this->Hash_08(data,len);
		case 9:
			return this->Hash_09(data,len);
		case 10:
			return this->Hash_10(data,len);
		case 11:
			return this->Hash_11(data,len);
	}
}
void bloom_filter::insert(string& data){
	for(int i=1;i<=11;++i){
		unsigned int hash_value=generate_hash(data,data.size(),i);
		bitmap[hash_value/BIT_NUM]|=(0x01<<hash_value%BIT_NUM);
	}
}
bool bloom_filter::check(string& data){
	for(int i=1;i<=11;++i){
		unsigned int hash_value=generate_hash(data,data.size(),i);
		int check_value=0x01<<hash_value%BIT_NUM;
		int bitmap_value=bitmap[hash_value/BIT_NUM];
		int flag=bitmap_value&check_value;
		if(!flag)
			return false;
	}
	return true;
}
