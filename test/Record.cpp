// ****************************************************************************
// * Author:      Hikawa Rin
// * Create Time: 2020-1-8 15:25
// * Last Change: 2020-1-8 15:42
// * Description: method for recoding individual data.
// * 
// ****************************************************************************
// This file implement methods for recoding individual data
// data will be generated to CSV format as below:
//
//  parameters,objective,generation
//  [X],Y,Int
//
// Due to this algorithm is a simple generation algorihtm. It will only have 
// one objective but may more than one parameter. Use generation recode it`s 
// change process
#ifndef SGA_TEST_RECORD_CPP
#define SGA_TEST_RECORD_CPP

#include "../src/sga.hpp"
#include <fstream>
#include <iostream>

#define AllPopPath "./AllPop.csv"
bool isExist = false;
int Record(std::vector<sIndividual> inds, int generation){
    std::ofstream file;
    if (!isExist){
        file.open(AllPopPath, std::ios::out | std::ios::trunc);
        isExist = true;
    }else{
        file.open(AllPopPath, std::ios::app);
    }

    if (file.is_open()){
        file << "parameters,objective,generation" << std::endl;
        for (auto ind : inds){
            file << '[';
            for (int i = 0; i < ind.values.size(); i++){
                if (i != 0){
                    file << ',';
                }
                file << ind.values[i];
            }
            file << "],";
            file << ind.objective << ",";
            file << generation << std::endl;
        }
    }else{
        FLog("open file error, please check path");
        exit(1);
    }

    file.close();
    return 0;
}

#endif // SGA_TEST_RECORD_CPP