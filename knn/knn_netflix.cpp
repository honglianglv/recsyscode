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
 * The purpose of this program is to implement the KNN model, using the netflix dataset.
 * 
 * Important Notice: This programme need be run on the machine with more than 6G RAM, 64bit Operating System.
 * Or there will be a memory overflow.
 */

#include "../commonHeader.h"
#include "../common.cpp"
#include "../netflixBase.cpp"
#define USER_NUM 480189  //10K:943 1M:6040
#define ITEM_NUM 17770 //10K:1682 1M:3900
#define K_NUM  50
#define MIN_RATE 1    // the minimum of the rate
#define MAX_RATE 5    // the maximum of the rate
#define TRAINING_SET "../dataset/netflix/data_without_prob.txt"
#define PROBE_SET "../dataset/netflix/probe_real.txt"
#define RATE_SP ","  //rate Separator
#include "./knnBase.cpp"

int main(int argc, char ** argv)
{
    time_t start,end;
    struct tm * timeStartInfo;
    struct tm * timeEndInfo;
    double duration; 
    start = time(NULL);
    timeStartInfo = localtime(&start);
    string timeStartStr = asctime(timeStartInfo);
    knn::model("netflix", "netflix_kmax");
    end = time(NULL);
    duration = (end-start);
    timeEndInfo = localtime(&end);
    cout << "Start at "<<timeStartStr<<", and end at "<< asctime(timeEndInfo);
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
