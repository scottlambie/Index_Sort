#include <iostream>
#include <fstream>
#include <iterator>
#include <string.h>
#include <vector>
#include <list>
#include <regex>

#include "lib/quicksort.h"

struct dictItem{
    public: //Needed?
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
    
    //TODO: Cleanup, include next series of inline chars, new syntax for inline chars(HTML-like(it works))
    std::fstream dict;
    std::string docId, docStream;
    std::smatch wrdCharMatches, descStringMatches;
    std::regex wrdChar("<w>[a-zA-Z]+");
    std::regex descReg("<d>(.*?)</d>");
    std::cout << "ENTER TEXTFILE IDENTIFIER:\n";
    std::cin >> docId;
    docId.append(".txt");
    
    //ADD MATCHED CHARS ONLY TO VEC
    dict.open(docId,std::ios::in);
    int i=0;
    while(getline(dict,docStream)){
        while(std::regex_search(docStream,wrdCharMatches,wrdChar)){
            dictionary.push_back({(wrdCharMatches.str().erase(0,3)),""});
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
            //TODO: Cleanup and compartmentalize(use functions)
            //REMOVE INLINE CHARS - NOT WORKING
            /*int i=0;
            while(i<tempString.length()){
                if((tempString[i]==38)&&(tempString[i+1]==113)) //CONDITIONAL, CHARS ARE ASCII VALUES &q
                    tempString.erase(i,2);
                ++i;
            }
            */
            tempString.erase(tempString.begin(),tempString.begin()+3);
            tempString.erase(tempString.end()-4,tempString.end());
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
    //TODO: Multiple descriptions mapping to same definition
    //TODO: definitions linking to one another
    std::vector<int> dupIndex;

    //TODO: input validation, file exists validation, filetype validation
    writeVec(dupIndex,dictionary);

    int rC = -1;
    rSort(dupIndex,rC,dictionary);
    
    //TODO: learn ofstream :)
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