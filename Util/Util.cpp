/*=============================================================================
# Filename: Util.cpp
# Author: Bookug Lobert
# Mail: 1181955272@qq.com
# Last Modified: 2015-10-16 10:43
# Description:
1. firstly written by liyouhuan, modified by zengli
2. achieve functions in Util.h
=============================================================================*/

#include "Util.h"

using namespace std;

//NOTICE:used in Database, Join and Strategy
int Util::triple_num = 0;
int Util::pre_num = 0;
int Util::entity_num = 0;
int Util::literal_num = 0;

//database home directory, which is an absolute path by config
//TODO:everywhere using database, the prefix should be it
string Util::db_home = ".";

//false:single true:distribute
bool Util::gStore_mode = false;

//string Util::tmp_path = "../.tmp/";
//string Util::debug_path = "../.debug/";
string Util::tmp_path = ".tmp/";
string Util::debug_path = ".debug/";

//string Util::profile = "../init.conf";
string Util::profile = "init.conf";

//QUERY: assign all in Util()?
//BETTER:assigned in KVstore, not one tree?
FILE* Util::debug_kvstore = NULL;            //used by KVstore
FILE* Util::debug_database = NULL;			 //used by Database
FILE* Util::debug_vstree = NULL;			 //used by VSTree

//set hash table
HashFunction Util::hash[] = { Util::simpleHash, Util::APHash, Util::BKDRHash, Util::DJBHash, Util::ELFHash, \
	Util::DEKHash, Util::BPHash, Util::FNVHash, Util::HFLPHash, Util::HFHash, Util::JSHash, \
	Util::PJWHash, Util::RSHash, Util::SDBMHash, Util::StrHash, Util::TianlHash, NULL};
//hash[0] = Util::simpleHash;
//hash[1] = Util::APHash;
//hash[2] = Util::BKDRHash;
//hash[3] = Util::DJBHash;
//hash[4] = Util::ELFHash;
//hash[5] = Util::DEKHash;
//hash[6] = Util::BPHash;
//hash[7] = Util::FNVHash;
//hash[8] = Util::HFLPHash;
//hash[9] = Util::HFHash;
//hash[10] = Util::JSHash;
//hash[11] = Util::PJWHash;
//hash[12] = Util::RSHash;
//hash[13] = Util::SDBMHash;
//hash[14] = Util::StrHash;
//hash[15] = Util::TianlHash;

//remove spaces in the left
char*
Util::l_trim(char* szOutput, const char* szInput)
{
    assert(szInput != NULL);
    assert(szOutput != NULL);
    assert(szOutput != szInput);
    for   (; *szInput != '\0' && isspace(*szInput); ++szInput);
    return strcpy(szOutput, szInput);
}

//remove spaces in the right
char*
Util::r_trim(char *szOutput, const char* szInput)
{
    char *p = NULL;
    assert(szInput != NULL);
    assert(szOutput != NULL);
    assert(szOutput != szInput);
    strcpy(szOutput, szInput);
    for(p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p);
    *(++p) = '\0';
    return szOutput;
}

//remove spaces in the two sides
char*
Util::a_trim(char * szOutput, const char * szInput)
{
    char *p = NULL;
    assert(szInput != NULL);
    assert(szOutput != NULL);
    l_trim(szOutput, szInput);
    for   (p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p);
    *(++p) = '\0';
    return szOutput;
}

bool
Util::configure()
{
	return Util::config_setting() && Util::config_debug() && Util::config_advanced();
}

bool
Util::config_debug()
{
    const unsigned len1 = 100;
    const unsigned len2 = 505;
	char AppName[] = "setting";
    char KeyName[] = "mode";
	char appname[len1], keyname[len1];
    char KeyVal[len1];
    char *buf, *c;
    char buf_i[len1], buf_o[len1];
    FILE *fp = NULL;
    int status = 0; // 1 AppName 2 KeyName
	return true;
}

bool
Util::config_advanced()
{
    const unsigned len1 = 100;
    const unsigned len2 = 505;
	char AppName[] = "setting";
    char KeyName[] = "mode";
	char appname[len1], keyname[len1];
    char KeyVal[len1];
    char *buf, *c;
    char buf_i[len1], buf_o[len1];
    FILE *fp = NULL;
    int status = 0; // 1 AppName 2 KeyName
	return true;
}

bool
Util::config_setting()
{
	//TODO:deal with more cases, need to be classified
	//
	//BETTER:different functions and function pointer array
	//
	//WARN:currently, we can call this function with different parameters
	//to acquire different config module

    const unsigned len1 = 100;
    const unsigned len2 = 505;
	char AppName[] = "setting";
    char KeyName[] = "mode";
	char appname[len1], keyname[len1];
    char KeyVal[len1];
    char *buf, *c;
    char buf_i[len2], buf_o[len2];
    FILE *fp = NULL;
    int status = 0; // 1 AppName 2 KeyName

#ifdef DEBUG
	fprintf(stderr, "profile: %s\n", profile.c_str());
#endif
    if((fp = fopen(profile.c_str(), "r")) == NULL)  //NOTICE: this is not a binary file
    {
#ifdef DEBUG
        fprintf(stderr, "openfile [%s] error [%s]\n", profile.c_str(), strerror(errno));
#endif
        return false;
    }
    fseek(fp, 0, SEEK_SET);
	memset(appname, 0, sizeof(appname));
	sprintf(appname,"[%s]", AppName);
#ifdef DEBUG
	fprintf(stderr, "appname: %s\n", appname);
#endif

    while(!feof(fp) && fgets(buf_i, len2, fp) != NULL)
    {
		//fprintf(stderr, "buffer: %s\n", buf_i);
        Util::l_trim(buf_o, buf_i);
        if(strlen(buf_o) <= 0)
            continue;
        buf = NULL;
        buf = buf_o;
		if(buf[0] == '#')
		{
			continue;
		}
        if(status == 0)
        {
            if(strncmp(buf, appname, strlen(appname)) == 0)
            {
#ifdef DEBUG
				fprintf(stderr, "app found!\n");
#endif
                status = 1;
                continue;
            }
        }
        else if(status == 1)
        {
			if(buf[0] == '[') 
			{
				//NOTICE: nested module is not allowed
                break;
            } 
			else 
			{
                if((c = (char*)strchr(buf, '=')) == NULL)
                    continue;
                memset(keyname, 0, sizeof(keyname));
                sscanf(buf, "%[^=|^ |^\t]", keyname);
#ifdef DEBUG
				fprintf(stderr, "keyname: %s\n", keyname);
#endif
                if(strcmp(keyname, KeyName) == 0) 
				{
#ifdef DEBUG
					fprintf(stderr, "key found!\n");
#endif
                    sscanf(++c, "%[^\n]", KeyVal);
                    char *KeyVal_o = (char *)calloc(strlen(KeyVal) + 1, sizeof(char));
                    if(KeyVal_o != NULL) 
					{
                        Util::a_trim(KeyVal_o, KeyVal);
#ifdef DEBUG
						fprintf(stderr, "KeyVal: %s\n", KeyVal_o);
#endif
                        if(KeyVal_o && strlen(KeyVal_o) > 0)
                            strcpy(KeyVal, KeyVal_o);
                        xfree(KeyVal_o);
                    }
                    status = 2;
                    break;
                } 
            }
        }
    }
    fclose(fp);
    //if(found == 2)
        //return(0);
    //else
        //return(-1);
    //fprintf(stderr, "%s\n", KeyVal);
	if(strcmp(KeyVal, "distribute") == 0)
	{
#ifdef DEBUG
		fprintf(stderr, "the gStore will run in distributed mode!\n");
#endif
		Util::gStore_mode = true;
	}

    return true;   //config success
}

Util::Util()
{
    Util::configure();
#ifdef DEBUG_KVSTORE
    if(this->debug_kvstore == NULL)
    {
        string s = this->debug_path + "kv.log";
        this->debug_kvstore = fopen(s.c_str(), "w+");
        if(this->debug_kvstore == NULL)
        {
            printf("open error: kv.log\n");
            this->debug_kvstore = stderr;
        }
    }
#endif
#ifdef DEBUG_DATABASE
    if(this->debug_database == NULL)
    {
        string s = this->debug_path + "db.log";
        this->debug_database = fopen(s.c_str(), "w+");
        if(this->debug_database == NULL)
        {
            printf("open error: db.log\n");
            this->debug_database = stderr;
        }
    }
#endif
#ifdef DEBUG_VSTREE
    if(this->debug_vstree == NULL)
    {
        string s = this->debug_path + "vs.log";
        this->debug_vstree = fopen(s.c_str(), "w+");
        if(this->debug_vstree == NULL)
        {
            printf("open error: vs.log\n");
            this->debug_vstree = stderr;
        }
    }
#endif
}

Util::~Util()
{
#ifdef DEBUG_KVSTORE
    fclose(this->debug_kvstore);	//NULL is ok, just like free(NULL)
    this->debug_kvstore = NULL;
#endif
#ifdef DEBUG_DATABASE
    fclose(this->debug_database);	//NULL is ok, just like free(NULL)
    this->debug_database = NULL;
#endif
}

int
Util::memUsedPercentage()
{
    FILE* fp = fopen("/proc/meminfo", "r");
    if(fp == NULL)
        return -1;
    char str[20], tail[3];
    unsigned t, sum, used = 0;		//WARN:unsigned,memory cant be too large!
    fscanf(fp, "%s%u%s", str, &sum, tail);       //MemTotal, KB
    fscanf(fp, "%s%u%s", str, &used, tail);		//MemFree
    fscanf(fp, "%s%u%s", str, &t, tail);
    if(strcmp(str, "MemAvailable") == 0)
    {
        //QUERY:what is the relation between MemFree and MemAvailable?
        used = t;
        //scanf("%s%u%s", str, &t, tail);		//Buffers
        //used += t;
        //scanf("%s%u%s", str, &t, tail);		//Cached
        //used += t;
    }
    //else							//Buffers
    //{
    //	scanf("%s%u%s", str, &t, tail);		//Cached
    //	used += t;
    //}
    used = sum - used;
    fclose(fp);
    return (int)(used * 100.0 / sum);
}

int
Util::memoryLeft()
{
    FILE* fp = fopen("/proc/meminfo", "r");
    if(fp == NULL)
        return -1;
    char str[20], tail[3];
    unsigned t, sum, unuse = 0;		//WARN:unsigned,memory cant be too large!
    fscanf(fp, "%s%u%s", str, &sum, tail);       //MemTotal, KB
    fscanf(fp, "%s%u%s", str, &unuse, tail);		//MemFree
    fscanf(fp, "%s%u%s", str, &t, tail);
    if(strcmp(str, "MemAvailable") == 0)
    {
        unuse = t;
    }
    fclose(fp);
    return unuse / Util::MB;
}

bool
Util::is_literal_ele(int _id)
{
    return _id >= Util::LITERAL_FIRST_ID;
}

//NOTICE: require that the list is ordered
int
Util::removeDuplicate(int* _list, int _len)
{
	int valid = 0, limit = _len - 1;
	for(int i = 0; i < limit; ++i)
	{
		if(_list[i] != _list[i+1])
		{
			_list[valid++] = _list[i];
		}
	}
	_list[valid++] = _list[limit];
	return valid;
}

int
Util::cmp_int(const void* _i1, const void* _i2)
{
    return *(int*)_i1  -  *(int*)_i2;
}

void
Util::sort(int*& _id_list, int _list_len)
{
    qsort(_id_list, _list_len, sizeof(int), Util::cmp_int);
}

int
Util::bsearch_int_uporder(int _key, const int* _array,int _array_num)
{
    if (_array_num == 0)
    {
        return -1;
    }
    if (_array == NULL)
    {
        return -1;
    }
    int _first = _array[0];
    int _last = _array[_array_num - 1];

    if (_last == _key)
    {
        return _array_num - 1;
    }

    if (_last < _key || _first > _key)
    {
        return -1;
    }

    int low = 0;
    int high = _array_num - 1;

    int mid;
    while (low <= high)
    {
        mid = (high - low) / 2 + low;
        if (_array[mid] == _key)
        {
            return mid;
        }
        if (_array[mid] > _key)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}

bool
Util::bsearch_preid_uporder(int _preid, int* _pair_idlist, int _list_len)
{
    if(_list_len == 0)
    {
        return false;
    }
    int pair_num = _list_len / 2;
    int _first = _pair_idlist[2*0 + 0];
    int _last = _pair_idlist[2*(pair_num-1) + 0];

    if(_preid == _last)
    {
        return true;
    }

    bool not_find = (_last < _preid || _first > _preid);
    if(not_find)
    {
        return false;
    }

    int low = 0;
    int high = pair_num - 1;
    int mid;

    while(low <= high)
    {
        mid = (high - low) / 2 + low;
        if(_pair_idlist[2*mid + 0] == _preid)
        {
            return true;
        }

        if(_pair_idlist[2*mid + 0] > _preid)
        {
            high = mid - 1;
        } 
		else
        {
            low = mid + 1;
        }
    }

    return false;
}

int
Util::bsearch_vec_uporder(int _key, const vector<int>* _vec)
{
    int tmp_size = _vec->size();
    if (tmp_size == 0)
    {
        return -1;
    }

    int _first = (*_vec)[0];
    int _last = (*_vec)[tmp_size - 1];

    if (_key == _last)
    {
        return tmp_size - 1;
    }

    bool not_find = (_last < _key || _first > _key);
    if (not_find)
    {
        return -1;
    }

    int low = 0;
    int high = tmp_size - 1;
    int mid;

    while (low <= high)
    {
        mid = (high - low) / 2 + low;
        if ((*_vec)[mid] == _key)
        {
            return mid;
        }

        if ((*_vec)[mid] > _key)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}

string
Util::result_id_str(vector<int*>& _v, int _var_num)
{
    stringstream _ss;

    for(unsigned i = 0; i < _v.size(); i ++)
    {
        int* _p_int = _v[i];
        _ss << "[";
        for(int j = 0; j < _var_num-1; j ++)
        {
            _ss << _p_int[j] << ",";
        }
        _ss << _p_int[_var_num-1] << "]\t";
    }

    return _ss.str();
}

bool
Util::dir_exist(const string _dir)
{
    return (opendir(_dir.c_str()) != NULL);
}

bool
Util::create_dir(const  string _dir)
{
    if(! Util::dir_exist(_dir))
    {
        mkdir(_dir.c_str(), 0755);
        return true;
    }

    return false;
}

long
Util::get_cur_time()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

bool
Util::save_to_file(const char* _dir, const string _content)
{
    ofstream fout(_dir);

    if (fout.is_open())
    {
        fout << _content;
        fout.close();
    }

    return false;
}

int
Util::compare(const char* _str1, unsigned _len1, const char* _str2, unsigned _len2)
{
    int ifswap = 1;		//1 indicate: not swapped
    if(_len1 > _len2)
    {
        const char* str = _str1;
        _str1 = _str2;
        _str2 = str;
        unsigned len = _len1;
        _len1 = _len2;
        _len2 = len;
        ifswap = -1;
    }
    unsigned i;
    //DEBUG: if char can be negative, which cause problem when comparing(128+)
    //
    //NOTICE:little-endian-storage, when string buffer poniter is changed to
    //unsigned long long*, the first char is the lowest byte!
    /*
       unsigned long long *p1 = (unsigned long long*)_str1, *p2 = (unsigned long long*)_str2;
       unsigned limit = _len1/8;
       for(i = 0; i < limit; ++i, ++p1, ++p2)
       {
       if((*p1 ^ *p2) == 0)	continue;
       else
       {
       if(*p1 < *p2)	return -1 * ifswap;
       else			return 1 * ifswap;
       }
       }
       for(i = 8 * limit; i < _len1; ++i)
       {
       if(_str1[i] < _str2[i])	return -1 * ifswap;
       else if(_str1[i] > _str2[i])	return 1 * ifswap;
       else continue;
       }
       if(i == _len2)	return 0;
       else	return -1 * ifswap;
       */
    for(i = 0; i < _len1; ++i)
    {   //ASCII: 0~127 but c: 0~255(-1) all transfered to unsigned char when comparing
        if((unsigned char)_str1[i] < (unsigned char)_str2[i])
            return -1 * ifswap;
        else if((unsigned char)_str1[i] > (unsigned char)_str2[i])
            return 1 * ifswap;
        else;
    }
    if(i == _len2)
        return 0;
    else
        return -1 * ifswap;
}

int
Util::string2int(string s)
{
    return atoi(s.c_str());
}

string
Util::int2string(long n)
{
    string s;
    stringstream ss;
    ss<<n;
    ss>>s;
    return s;
}

string
Util::showtime()
{
//	fputs("\n\n", logsfp);
    time_t now;
    time(&now);
//	fputs(ctime(&now), logsfp);
    return string("\n\n") + ctime(&now);
}

string
Util::getQueryFromFile(const char* _file_path)
{
#ifdef DEBUG_PRECISE
    printf("file to open: %s\n", _file_path);
#endif
    char buf[10000];
    std::string query_file;

    ifstream fin(_file_path);
    if(!fin)
    {
        printf("can not open: %s\n", _file_path);
        return "";
    }

    memset(buf, 0, sizeof(buf));
    stringstream _ss;
    while(!fin.eof())
    {
        fin.getline(buf, 9999);
        _ss << buf << "\n";
    }
    fin.close();

    return _ss.str();
}

string
Util::getItemsFromDir(string _path)
{
	DIR* dp = NULL;
	struct dirent* entry;
	string ret = "";
	if((dp = opendir(_path.c_str())) == NULL)
	{
		fprintf(stderr, "error opening directory!\n");
	}
	else
	{
		while((entry = readdir(dp)) != NULL)
		{
#ifdef DEBUG_PRECISE
			fprintf(stderr, "%s\n", entry->d_name);
#endif
			string name= string(entry->d_name);
			int len = name.length();
			if(len <= 3)
			{
				continue;
			}
			if(name.substr(len-3, 3) == ".db")
			{
				if(ret == "")
					ret = name;
				else
					ret = ret + "  " + name;
			}
		}
		closedir(dp);
	}
#ifdef DEBUG_PRECISE
	fprintf(stderr, "OUT getItemsFromDir\n");
#endif
	return ret;
}

string
Util::getSystemOutput(string cmd)
{
    string ans = "";
    string file = Util::tmp_path;
    file += "ans.txt";
    cmd += " > ";
    cmd += file;
    cerr << cmd << endl;
    int ret = system(cmd.c_str());
    cmd = "rm -rf " + file;
    if(ret < 0)
    {
        fprintf(stderr, "system call failed!\n");
        system(cmd.c_str());
        return NULL;
    }

    ifstream fin(file.c_str());
    if(!fin)
    {
        cerr << "getSystemOutput: Fail to open : " << file << endl;
        return NULL;
    }

	string temp;
	getline(fin, temp);
	while(!fin.eof())
	{
		//cout<<"system line"<<endl;
		if(ans == "")
			ans = temp;
		else
			ans = ans + "\n" + temp;
		getline(fin, temp);
	}
    fin.close();
    //FILE *fp = NULL;
    //if((fp = fopen(file.c_str(), "r")) == NULL)
    //{
    //fprintf(stderr, "unbale to open file: %s\n", file.c_str());
    //}
    //else
    //{
    //char *ans = (char *)malloc(100);
    //fgets(path, 100, fp);
    //char *find = strchr(path, '\n');
    //if(find != NULL)
    //*find = '\0';
    //fclose(fp);
    //}
    system(cmd.c_str());
	cout<<"ans: "<<ans<<endl;
    return ans;
}

// Get the exact file path from given string: ~, ., symbol links
string
Util::getExactPath(const char *str)
{
    string cmd = "realpath ";
    cmd += string(str);

    return getSystemOutput(cmd);
}

void
Util::logging(string _str)
{
    _str += "\n";
#ifdef DEBUG_DATABASE
    fputs(_str.c_str(), Util::debug_database);
    fflush(Util::debug_database);
#endif

#ifdef DEBUG_VSTREE
    fputs(_str.c_str(), Util::debug_vstree);
    fflush(Util::debug_vstree);
#endif
}

unsigned
Util::BKDRHash(const char *_str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int key = 0;

    //for(unsigned i = 0; i < _len; ++i)
	while(*_str)
    {
        //key = key * seed + _str[i];
		key = key * seed + *(_str++);
    }

    return (key & 0x7FFFFFFF);
}

unsigned
Util::simpleHash(const char *_str)
{
    unsigned int key;
    unsigned char *p;

    for(key = 0, p = (unsigned char *)_str; *p; p++)
        key = 31 * key + *p;

    return (key & 0x7FFFFFFF);
}

unsigned
Util::RSHash(const char *_str)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int key = 0;

    while (*_str)
    {
        key = key * a + (*_str++);
        a *= b;
    }

    return (key & 0x7FFFFFFF);
}

unsigned
Util::JSHash(const char *_str)
{
    unsigned int key = 1315423911;

    while (*_str)
    {
        key ^= ((key << 5) + (*_str++) + (key >> 2));
    }

    return (key & 0x7FFFFFFF);
}

unsigned
Util::PJWHash(const char *_str)
{
    unsigned int bits_in_unsigned_int = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int three_quarters = (unsigned int)((bits_in_unsigned_int * 3) / 4);
    unsigned int one_eighth = (unsigned int)(bits_in_unsigned_int / 8);

    unsigned int high_bits = (unsigned int)(0xFFFFFFFF) << (bits_in_unsigned_int - one_eighth);
    unsigned int key = 0;
    unsigned int test = 0;

    while (*_str)
    {
        key = (key << one_eighth) + (*_str++);
        if ((test = key & high_bits) != 0)
        {
            key = ((key ^ (test >> three_quarters)) & (~high_bits));
        }
    }

    return (key & 0x7FFFFFFF);
}

unsigned
Util::ELFHash(const char *_str)
{
    unsigned int key = 0;
    unsigned int x  = 0;

    while (*_str)
    {
        key = (key << 4) + (*_str++);
        if ((x = key & 0xF0000000L) != 0)
        {
            key ^= (x >> 24);
            key &= ~x;
        }
    }

    return (key & 0x7FFFFFFF);
}

unsigned
Util::SDBMHash(const char *_str)
{
    unsigned int key = 0;

    while (*_str)
    {
        key = (*_str++) + (key << 6) + (key << 16) - key;
    }

    return (key & 0x7FFFFFFF);
}

unsigned
Util::DJBHash(const char *_str)
{
    unsigned int key = 5381;
    while (*_str) {
        key += (key << 5) + (*_str++);
    }
    return (key & 0x7FFFFFFF);
}

unsigned
Util::APHash(const char *_str)
{
    unsigned int key = 0;
    int i;

    for (i=0; *_str; i++)
    {
        if ((i & 1) == 0)
        {
            key ^= ((key << 7) ^ (*_str++) ^ (key >> 3));
        }
        else
        {
            key ^= (~((key << 11) ^ (*_str++) ^ (key >> 5)));
        }
    }

    return (key & 0x7FFFFFFF);
}

unsigned
Util::DEKHash(const char* _str)
{
    unsigned int hash = strlen(_str);
    for(; *_str; _str++) 
	{
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*_str);
    }
    return hash;
}

unsigned
Util::BPHash(const char* _str)
{
    unsigned int hash = 0;
    for(; *_str; _str++) 
	{
        hash = hash << 7 ^ (*_str);
    }

    return hash;
}

unsigned
Util::FNVHash(const char* _str)
{
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash = 0;

    for(; *_str; _str++) 
	{
        hash *= fnv_prime;
        hash ^= (*_str);
    }

    return hash;
}

unsigned
Util::HFLPHash(const char* _str)
{
    unsigned int n = 0;
    char* b = (char*)&n;
    unsigned int len = strlen(_str);
    for(unsigned i = 0; i < len; ++i)
    {
        b[i%4] ^= _str[i];
    }
    return n%len;
}

unsigned
Util::HFHash(const char* _str)
{
    int result=0;
    const char* ptr = _str;
    int c;
    unsigned int len = strlen(_str);
    for(int i=1; (c=*ptr++); i++)
        result += c*3*i;
    if (result<0)
        result = -result;
    return result%len;
}

unsigned
Util::StrHash(const char* _str)
{
    register unsigned int   h;
    register unsigned char *p;
    for(h = 0, p = (unsigned char *)_str; *p; p++) 
	{
        h = 31 * h + *p;
    }

    return h;

}

unsigned
Util::TianlHash(const char* _str)
{
    unsigned long urlHashValue=0;
    int ilength=strlen(_str);
    int i;
    unsigned char ucChar;
    if(!ilength)  {
        return 0;
    }
    if(ilength<=256)  {
        urlHashValue=16777216*(ilength-1);
    } else {
        urlHashValue = 42781900080;
    }
    if(ilength<=96) {
        for(i=1; i<=ilength; i++) {
            ucChar = _str[i-1];
            if(ucChar<='Z'&&ucChar>='A')  {
                ucChar=ucChar+32;
            }
            urlHashValue+=(3*i*ucChar*ucChar+5*i*ucChar+7*i+11*ucChar)%1677216;
        }
    } else  {
        for(i=1; i<=96; i++)
        {
            ucChar = _str[i+ilength-96-1];
            if(ucChar<='Z'&&ucChar>='A')
            {
                ucChar=ucChar+32;
            }
            urlHashValue+=(3*i*ucChar*ucChar+5*i*ucChar+7*i+11*ucChar)%1677216;
        }
    }

    return urlHashValue;
}

//NOTICE: the time of log() and sqrt() in C can be seen as constant

//NOTICE:_b must >= 1
double
Util::logarithm(double _a, double _b)
{
    //REFRENCE: http://blog.csdn.net/liyuanbhu/article/details/8997850
    //a>0 != 1; b>0 (b>=2 using log/log10/change, 1<b<2 using log1p, b<=1?)
    if(_a <= 1 || _b < 1)
        return -1.0;
    double under = log(_a);
    if(_b == 1)
        return 0.0;
    else if(_b < 2)
        return log1p(_b - 1) / under;
    else //_b >= 2
        return log(_b) / under;
    return -1.0;
}

void
Util::intersect(int*& _id_list, int& _id_list_len, const int* _list1, int _len1, const int* _list2, int _len2)
{
	vector<int> res;
	//cout<<"intersect prevar: "<<_len1<<"   "<<_len2<<endl;
	if(_list1 == NULL || _len1 == 0 || _list2 == NULL || _len2 == 0)
	{
		_id_list = NULL;
		_id_list_len = 0;
	}

	//when size is almost the same, intersect O(n)
	//when one size is small ratio, search in the larger one O(mlogn)
	//
	//n>0 m=nk(0<k<1) 
	//compare n(k+1) and nklogn: k0 = log(n/2)2 requiring that n>2
	//k<=k0 binary search; k>k0 intersect
	int method = -1; //0: intersect 1: search in list1 2: search in list2
	int n = _len1;
	double k = 0;
	if(n < _len2)
	{
		k = (double)n / (double)_len2;
		n = _len2;
		method = 2;
	}
	else
	{
		k = (double)_len2 / (double)n;
		method = 1;
	}
	if(n <= 2)
		method = 0;
	else
	{
		double limit = Util::logarithm(n/2, 2);
		if(k > limit)
			method = 0;
	}

	switch(method)
	{
	case 0:
	{   //this bracket is needed if vars are defined in case
		int id_i = 0;
		int num = _len1;
		for(int i = 0; i < num; ++i)
		{
			int can_id = _list1[i];
			while((id_i < _len2) && (_list2[id_i] < can_id))
			{
				id_i ++;
			}

			if(id_i == _len2)
			{
				break;
			}

			if(can_id == _list2[id_i])
			{
				res.push_back(can_id);
				id_i ++;
			}
		}
		break;
	}
	case 1:
	{
		for(int i = 0; i < _len2; ++i)
		{
			if(Util::bsearch_int_uporder(_list2[i], _list1, _len1) != -1)
				res.push_back(_list2[i]);
		}
		break;
	}
	case 2:
	{
		int m = _len1, i;
		for(i = 0; i < m; ++i)
		{
			int t = _list1[i];
			if(Util::bsearch_int_uporder(t, _list2, _len2) != -1)
				res.push_back(t);
		}
		break;
	}
	default:
		cerr << "no such method in Util::intersect()" << endl;
		break;
	}

	_id_list_len = res.size();
	_id_list = new int[_id_list_len];
	for(int i = 0; i < _id_list_len; ++i)
		_id_list[i] = res[i];
	delete[] _list1;
	delete[] _list2;
}

