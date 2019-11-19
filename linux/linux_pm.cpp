#include "linux_pm.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>


std::string grep(std::string text, std::string keyword, int id=0){
    size_t pos1, pos2;
    for(int i=0;i<id+1;i++){
        std::size_t found = text.find(keyword,found+1);
        
        if(found==std::string::npos){
            throw 1;
        }
        
        std::string first_substring = text.substr(0,found);
        
        pos1 = first_substring.find("\n");
        while(first_substring.find("\n", pos1+1)!=std::string::npos){
            pos1 = first_substring.find("\n",pos1+1);
        }
        std::string second_substring = text.substr(found,text.length());
        pos2 = second_substring.find("\n");
        if(pos2==std::string::npos){
            pos2 = text.length();
        }
    }
    // std::cout << pos1 << std::endl << pos2 << std::endl;
    std::string res = text.substr(pos1+1,pos2+1);
    return res;
}

int get_cpu_count(){
    std::ifstream ifs("/proc/stat");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    std::string cur_cpu = grep(content, "cpu");
    int count = 0;
    while(true){
        count++;
        try{
            cur_cpu = grep(content, "cpu", count);
        }catch(int){
            break;
        }
    }
    return count;
}


std::vector<std::string> get_cpu_model(bool per_cpu=false){

    std::ifstream ifs("/proc/cpuinfo");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    std::vector<std::string> models;
    
    std::string temp;
    if(per_cpu==true){
        int cpu_count = get_cpu_count();
        for(int i=0;i<cpu_count;i++){
            temp = grep(content, "model name", i);
            temp = temp.substr(temp.find(":")+2,temp.length());
            models.push_back(temp);
        }
    }else{
        temp = grep(content, "model name");
        temp = temp.substr(temp.find(":")+2,temp.length());
        models.push_back(temp);
    }
    return models;
}

std::vector<int> get_cpu_usage(bool per_cpu=false){
    std::ifstream ifs("/proc/stat");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    

    std raw_cpu
    if(per_cpu==true){
        int count = get_cpu_count();

        for(int i=0;i<count;i++){
            
        }
    }
}

int main(){
    // std::cout << get_cpu_count() << std::endl;
    // std::vector<std::string> models = get_cpu_model(true);
    // for(int i=0;i<models.size();i++){
    //     std::cout << models[i];
    // }
    
    return 0;
}