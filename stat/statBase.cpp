/* Copyright (C) 2011 Lv Hongliang. All Rights Reserved.
 * please maitain the copyright information completely when you redistribute the code.
 * 
 * If there are some bugs, please contact me via email honglianglv@gmail.com or submit the bugs 
 * in the google code project http://code.google.com/p/recsyscode/issues/list
 * 
 * my blog: http://lifecrunch.biz
 * my twitter: http://twitter.com/honglianglv
 * my google profile:https://profiles.google.com/honglianglv/about?hl=en
 *
 * It is free software; you can redistribute it and/or modify it under 
 * the license GPLV3.
 *
 * statBase.cpp contains some common functions of stat functions.
 * This file is used for stat the dataset character
 */
#ifndef STAT_STATBASE_CPP_
#define STAT_STATBASE_CPP_
namespace stat{
    //use some global variables，store the parameter bu, bi, p, q
    //double bu[USER_NUM+1] = {0};       // the user bias in the baseline predictor
    //double bi[ITEM_NUM+1] = {0};       // the item bias in the baseline predictor
    
    int buNum[USER_NUM+1] = {0};       //ratings num of every user 用户u打分的item总数， 
    int biNum[ITEM_NUM+1] = {0};       //rating num of every item  打过item i分的用户总数 
    
    vector < vector<rateNode> > rateMatrix(USER_NUM+1);   //store training set
    
    //stat the rate num of users and items
    void statRateNum(const char* itemNumFile, const char* userNumFile)
    {
        using namespace stat;
        int i,j;
        for(i = 1; i < USER_NUM+1; ++i){
            int vSize = rateMatrix[i].size();
            for(j=0; j < vSize; ++j) {
                biNum[rateMatrix[i][j].item] += 1;
                buNum[i] += 1;
            }            
        }
        
        qsort(biNum,1, ITEM_NUM); //sort the array
        ofstream biNumStream(itemNumFile);
        for(i = 1; i < ITEM_NUM+1; ++i) {
            biNumStream<<i<<'\t'<<biNum[i];
        }
        biNumStream.close();
        
        qsort(buNum,1, USER_NUM); //sort the array
        ofstream buNumStream(userNumFile);
        for(i = 1; i < USER_NUM+1; ++i) {
            buNumStream<<i<<'\t'<<buNum[i];
        }
        buNumStream.close();
    }
    
    void model(const char* itemNumFile, const char* userNumFile)
    {
        cout << "load data!" << endl;
        loadRating(TRAINING_SET,rateMatrix,RATE_SP);  //load training set
        cout << "end load data!" << endl;
        
        statRateNum(itemNumFile, userNumFile); //stat the num
    }
};

/**
 * predict the rate
 */
float predictRate(int user, int item,int dim)
{
    using namespace stat;
    return 0.0;
}
#endif // STAT_STATBASE_CPP_ 
