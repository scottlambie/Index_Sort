#include <iostream>
#include <fstream>
#include <iterator>
#include <string.h>
#include <vector>
#include <list>
#include <regex>

#include "lib/quicksort.h"

struct dictItem{
    public:
    std::string dictWrd;
    std::string dictDesc;
};

void swap(std::vector<dictItem> &dictionary, int a, int b){
    dictItem temp = dictionary[a];
    dictionary[a] = dictionary[b];
    dictionary[b] = temp;
}

void rSort(std::vector<int> dupIndex, int rC, std::vector<dictItem> &dictionary){
    ++rC;
    //FILL INT ARRAY
    int intArr[dupIndex.size()];
    for (int i=0;i<dupIndex.size();++i){
        intArr[i] = dictionary[dupIndex[i]].dictWrd[rC];
    }
    //SORT INT ARRAY
    quicksort(intArr,dupIndex.size());
\
    //SORT DICTIONARY BY STR CHAR[RC] ACCORDING TO INTARR
    int c=0;
    for (int i=dupIndex[0];i<dupIndex[0]+dupIndex.size();++i){
        for (int j=dupIndex[0]+c;j<dupIndex[0]+dupIndex.size();++j){
            if(dictionary[j].dictWrd[rC]==intArr[c]){
                swap(dictionary,i,j);
                intArr[c] = 0;
                ++c;
                break;
            }
        }
    }
    
    //FIND DUPLICATE STR CHAR[RC] AND RECURSE
    std::vector<int> rDupIndex;
    for(int i=dupIndex[0];i<dupIndex.back();++i){ 
        if(dictionary[i].dictWrd[rC]==dictionary[i+1].dictWrd[rC]){
            rDupIndex.push_back(i);
            while(i<dupIndex.back() && dictionary[i].dictWrd[rC]==dictionary[i+1].dictWrd[rC]){
                rDupIndex.push_back(i+1);
                ++i;
            }
            rSort(rDupIndex,rC,dictionary);
            rDupIndex.clear();
        }
    }
}

void writeVec(std::vector<int> &dupIndex, std::vector<dictItem> &dictionary){
    
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
            dictionary.push_back({(wrdCharMatches.str().erase(0,2)),""});
            dupIndex.push_back(i);
            docStream = wrdCharMatches.suffix().str();
            ++i;
        }
    }
    dict.close();

    //INLINE OPEN/CLOSING TAG MATCHING FOR DESCRIPTIONS
    std::string tempString;
    std::string userInput;
    dict.open(docId,std::ios::in);
    while(getline(dict,docStream)){
        while(std::regex_search(docStream,descStringMatches, descReg)){
            tempString = descStringMatches.str();
            tempString.erase(tempString.begin(),tempString.begin()+2);
            tempString.erase(tempString.end()-2,tempString.end());
            std::cout << "\nDescription:\n'" << tempString << "'\nFound.\nEnter associated word to match description to:";
            std::cin >> userInput;
            for(int i=0;i<dictionary.size();++i){
                if(userInput == dictionary[i].dictWrd)
                    dictionary[i].dictDesc = tempString;
            }
            docStream = descStringMatches.suffix().str();
        }
    }
}

int main(){
    std::vector<dictItem> dictionary;
    //TEMPORARY DESCRIPTION VECTOR UNORDERED
    //TODO: MAPPING(STRVEC & RELATED DESCSTRVEC ELEMENTS)
    std::vector<int> dupIndex;

    writeVec(dupIndex,dictionary);

    int rC = -1;
    rSort(dupIndex,rC,dictionary);
    
    std::ofstream sorted;
    sorted.open("sorted.txt");
    
    for(int i=1;i<dictionary.size()+1;++i){
        if (dictionary[i-1].dictDesc != ""){
            sorted << "\n" << dictionary[i-1].dictWrd << ":\n" << dictionary[i-1].dictDesc << "\n\n";
        }else{
            sorted << dictionary[i-1].dictWrd << ", ";
        }
    }
    
    std::cout << "\nsorted.txt written\nCHARS WRITTEN: " << dictionary.size();
    return 0;
}