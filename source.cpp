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

void writeVec(std::vector<std::string> &strVec, std::vector<int> &dupIndex, std::vector<std::string> &descStrVec){
    std::fstream dict;
    std::string docId, docStream;
    std::smatch wrdCharMatches, descStringMatches;
    std::regex wrdChar("&q[a-zA-Z]+");
    std::regex descReg("&d(.*?)&d");
    std::cout << "ENTER TEXTFILE IDENTIFIER:\n";
    std::cin >> docId;
    docId.append(".txt");
    
    //ADD MATCHED CHARS ONLY TO VEC
    dict.open(docId,std::ios::in);
    int i=0;
    while(getline(dict,docStream)){
        while(std::regex_search(docStream,wrdCharMatches,wrdChar)){
            strVec.push_back(wrdCharMatches.str().erase(0,2));
            dupIndex.push_back(i);
            docStream = wrdCharMatches.suffix().str();
            ++i;
        }
    }
    dict.close();

    //INLINE OPEN/CLOSING TAG MATCHING FOR DESCRIPTIONS
    std::string tempstring;
    dict.open(docId,std::ios::in);
    while(getline(dict,docStream)){
        while(std::regex_search(docStream,descStringMatches, descReg)){
            tempstring = descStringMatches.str();
            tempstring.erase(tempstring.begin(),tempstring.begin()+2);
            tempstring.erase(tempstring.end()-2,tempstring.end());
            descStrVec.push_back(tempstring);
            docStream = descStringMatches.suffix().str();
        }
    }
}

int main(){
    std::vector<std::string> strVec;
    //TEMPORARY DESCRIPTION VECTOR UNORDERED
    //TODO: MAPPING(STRVEC & RELATED DESCSTRVEC ELEMENTS)
    std::vector<std::string> descStrVec;
    std::vector<int> dupIndex;

    writeVec(strVec,dupIndex,descStrVec);

    int rC = -1;
    rSort(strVec,dupIndex,rC);
    
    std::ofstream sorted;
    sorted.open("sorted.txt");
    
    for(int i=1;i<strVec.size()+1;++i){
        sorted << strVec[i-1] << ", ";
        if((i%10)==0)
            sorted << "\n";
    }

    sorted << "\n\n";
    
    for (int i=0;i<descStrVec.size();++i)
        sorted << descStrVec[i] << "\n";
    sorted.close();
    
    std::cout << "\nsorted.txt written\nCHARS WRITTEN: " << strVec.size() << "\tLINES: " << ((strVec.size()/10)+1) << "\n";
    return 0;
}