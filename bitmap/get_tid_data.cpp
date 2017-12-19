#include <iostream>  
#include <fstream>  
#include <string>  
#include "string.h"
#include "stdio.h"
#include "bitmap.h"
#include <vector>
  
//using namespace std;  

namespace mycode {
void split (const std::string &input,const char *regex,std::vector<std::string> &result) {
    int pos = 0;  
    int npos = 0;  
    int regexlen = ::strlen(regex);  

    result.clear();
    while((npos=input.find(regex, pos))!=-1)  
    {  
        result.push_back(input.substr(pos,npos-pos));  
        pos = npos+regexlen;  
    }  
    result.push_back(input.substr(pos,input.length()-pos));  
} 
} // namespace mycode

int main(int argc, char *argv[])
{  
  	const char *bitmap_file = "/home/fengyuwei/netdisk/tieba_data_1/tid.txt";  
  	const char *data_file = "/home/fengyuwei/netdisk/tieba_data_1/data.txt";  
    const char *result_file = "/home/fengyuwei/netdisk/tieba_data_1/result.txt";

    std::ifstream file;  
	file.open(bitmap_file,std::ios::in);  

    std::ofstream ofile;
    ofile.open(result_file,std::ios_base::app);

	if (!file.is_open()) {
        std::cout << "cannot open:" << bitmap_file << std::endl;
		return 0;  
	}

    mycode::bitmap bitmap;
    uint64_t max_tid = 5472508508;
    bitmap.create(max_tid);

    std::string line;  
    std::vector<std::string> result;
	while (std::getline(file,line))  
	{  

		if (line.empty()) { 
			continue;  
		}
        uint64_t tid = ::atoll(line.c_str());
        //printf("%lu\n",tid);
        bitmap.set(tid);
	}  
	file.close();

    // 打开文件
	file.open(data_file,std::ios::in);  

	if(!file.is_open()) {
        // printf("[%s] cannot open!",data_file.c_str());
        std::cout << "cannot open:" << data_file << std::endl;
		return 0;  
	}

    const char *sep = "\t";
	while (std::getline(file,line))  
	{  

		if (line.empty()) { 
			continue;  
		}
        mycode::split(line,sep,result);	
        uint64_t tid = ::atoll(result[0].c_str());

        bool flag = bitmap.test(tid);

        //printf("%lu",tid);
        //cout << ",flag:" << flag << endl;

        if (!flag) {
            ofile << line << std::endl;
            ofile.flush();
        }
	}  
    bitmap.destroy();
	file.close();
    ofile.close();

    return 0;
}  

