#include <iostream>
#include <fstream>
#include <iterator>
#include <string.h>
#include <vector>
#include <list>
#include <regex>

#include "lib/quicksort.h"

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
    //SORT INT ARRAY
    quicksort(intArr,dupIndex.size());
\
    //SORT STRVEC BY STR CHAR[RC] ACCORDING TO INTARR
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
    
    //FIND DUPLICATE STR CHAR[RC] AND RECURSE
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
    std::string id;
    std::cout << "ENTER TEXTFILE IDENTIFIER\n";
    std::cin >> id;
    id.append(".txt");
    std::fstream dict;
    dict.open(id,std::ios::in);
    std::string rawStr;
    std::smatch matches;
    std::regex reg("([a-zA-Z]+)");
    int i=0;
    while(getline(dict,rawStr)){
        //getline(dict,rawStr);
        while (std::regex_search(rawStr,matches,reg)){
            strVec.push_back(matches.str());
            dupIndex.push_back(i);
            rawStr = matches.suffix().str();
            ++i;
        }
    }
}

int main(){
    std::vector<std::string> strVec;
    std::vector<int> dupIndex;

    writeVec(strVec,dupIndex);

    int rC = -1;
    rSort(strVec,dupIndex,rC);
    
    std::ofstream sorted;
    sorted.open("sorted.txt");
    
    for(int i=0;i<strVec.size();++i){
        sorted << strVec[i] << ", ";
        if((i%10)==0)
            sorted << "\n";
    }
    
    sorted.close();
    std::cout << "sorted.txt written\nCHARS WRITTEN: " << strVec.size() << "\tLINES: " << ((strVec.size()/10)+1) << "\n";
    return 0;
}