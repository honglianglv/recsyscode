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
 * 
 * The purpose of this program is to implement the in the baseline method,which 
 * is descripted in the page 148 of Recommender system Handbook. 
 * using the movielens dataset.
 *
 */
 
#include "../commonHeader.h"
#include "../common.cpp"
#include "../mlBase.cpp"
#define USER_NUM 6040 //10K:943 1M:6040
#define ITEM_NUM 3900 //10K:1682 1M:3900
#define K_NUM  50     //dimension
#define TRAINING_SET "../dataset/movielens/test.txt" //training set
#define PROBE_SET "../dataset/movielens/training.txt"  //test set
#define RATE_SP "	"  //rate Separator
#include "./baselineBase.cpp"

int main(int argc, char ** argv)
{
    time_t start,end;
    struct tm * timeStartInfo;
    struct tm * timeEndInfo;
    double duration; 
    start = time(NULL);
    timeStartInfo = localtime(&start);
    string timeStartStr = asctime(timeStartInfo);
    float alpha = 0.01;  
    float beta = 0.05;  

    //for(int i=0; i < 10; i++)
    {
        //alpha = i*0.0005 + 0.0025;
        //cout << alpha << endl;
        //void model(int dim, float alpha, float beta, int maxStep=60, double slowRate=1,bool isUpdateBias=true)
        svd::model(K_NUM,alpha,beta,60,1,false);    
    }
    end = time(NULL);
    duration = (end-start);
    timeEndInfo = localtime(&end);
    cout << "Start at "<<timeStartStr<<", and end at "<< asctime(timeEndInfo);
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
