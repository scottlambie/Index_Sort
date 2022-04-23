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
    std::fstream dict;
    std::string id, rawStr, vectorElement;
    std::smatch wrdCharMatches;
    std::regex wrdChar("&q[a-zA-Z]+");
    std::cout << "ENTER TEXTFILE IDENTIFIER:\n";
    std::cin >> id;
    id.append(".txt");
    
    //ADD MATCHED CHARS ONLY TO VEC
    dict.open(id,std::ios::in);
    int i=0;
    while(getline(dict,rawStr)){
        while(std::regex_search(rawStr,wrdCharMatches,wrdChar)){
            vectorElement = wrdCharMatches.str().erase(0,2);
            strVec.push_back(vectorElement);
            dupIndex.push_back(i);
            rawStr = wrdCharMatches.suffix().str();
            ++i;
        }
    }
    dict.close();
}

int main(){
    std::vector<std::string> strVec;
    std::vector<int> dupIndex;

    writeVec(strVec,dupIndex);

    int rC = -1;
    rSort(strVec,dupIndex,rC);
    
    std::ofstream sorted;
    sorted.open("sorted.txt");
    
    for(int i=1;i<strVec.size()+1;++i){
        sorted << strVec[i-1] << ", ";
        if((i%10)==0)
            sorted << "\n";
    }
    
    sorted.close();
    std::cout << "sorted.txt written\nCHARS WRITTEN: " << strVec.size() << "\tLINES: " << ((strVec.size()/10)+1) << "\n";
    return 0;
}