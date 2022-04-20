#include <iostream>
#include <iterator>
#include <string.h>
#include <vector>
#include <list>
#include <fstream>
#include <regex>

#include "quicksort.h"

const int arrLength = 23;

void swap(std::vector<std::string> &strVec, int a, int b){
    std::string temp = strVec[a];
    strVec[a] = strVec[b];
    strVec[b] = temp;
}

void rSort(std::vector<std::string> &strVec, std::vector<int> dupIndex, int rC){
    ++rC;
    //FILL INT ARRAY
    int intArr[dupIndex.size()];
    for (int i=0;i<dupIndex.size();++i){
        intArr[i] = strVec[dupIndex[i]][rC];
    }
    quicksort(intArr,dupIndex.size());

    int c=0;
    for (int i=dupIndex[0];i<dupIndex[0]+dupIndex.size();++i){
        for (int j=dupIndex[0]+c;j<dupIndex[0]+dupIndex.size();++j){
            if(strVec[j][rC]==intArr[c]){
                swap(strVec,i,j);
                intArr[c] = 0;
                ++c;      
                break;
            }
        }
    }
    
    std::vector<int> rDupIndex;
    for(int i=dupIndex[0];i<dupIndex.back();++i){ 
        if(strVec[i][rC]==strVec[i+1][rC]){
            rDupIndex.push_back(i);
            while(i<dupIndex.back() && strVec[i][rC]==strVec[i+1][rC]){
                rDupIndex.push_back(i+1);
                ++i;
            }
            rSort(strVec,rDupIndex,rC);
            rDupIndex.clear();
        }
    }
}

void writeVec(std::vector<std::string> &strVec, std::vector<int> &dupIndex){
    std::fstream dict;
    dict.open("alice29.txt",std::ios::in);
    std::string rawStr;
    getline(dict, rawStr);
    std::smatch matches;
    std::regex reg("([a-zA-Z]+)");
    int i=0;
    while (std::regex_search(rawStr,matches,reg)){
        strVec.push_back(matches.str());
        dupIndex.push_back(i);
        rawStr = matches.suffix().str();
        ++i;
    }
}

int main(){
    std::vector<std::string> strVec;
    std::vector<int> dupIndex;

    //Regex matching
    writeVec(strVec,dupIndex);

    int rC = -1;
    rSort(strVec,dupIndex,rC);

    for (int i=0;i<arrLength;++i){
        std::cout << strVec[i] << ",";
    }
    std::cout << "\n";
}