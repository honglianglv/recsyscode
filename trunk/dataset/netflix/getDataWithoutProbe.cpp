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
 * function:
 *   get the training set without probe ratings
 */
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string>
#include <map>
#include <errno.h>
#include <stdlib.h>
#define USER_NUM 480189 
#define ITEM_NUM 17770
#define DIR_PATH "./transfer_set/"
using namespace std;

map<int,short> probeMatrix[USER_NUM+1];   
int main()
{
    DIR *dp;
    char rateStr[256];
    struct dirent *dirp;
    
    //first load the file:probe_real.txt 首先读取probe_real.txt 文件
    ifstream in("probe_real.txt");
    int pos1,pos2;
    string strTemp;
    int rateValue,itemId,userId,probeNum;
    float pRate,err;
    
    while(in.getline(rateStr,256)){
        strTemp = rateStr;
        pos1 = strTemp.find(",");
        pos2 = strTemp.find(",",pos1+1);
        itemId = atoi(strTemp.substr(0,pos1).c_str());
        userId = atoi(strTemp.substr(pos1+1,pos2-pos1-1).c_str());
        rateValue = atoi(strTemp.substr(pos2+1).c_str());
        probeMatrix[userId][itemId] = rateValue;
    }
    in.close(); //finish loading userId map
   
    if((dp  = opendir(DIR_PATH)) == NULL) {
        cout << "Error(" << errno << ") opening " << DIR_PATH << endl;
        return 0;
    }
    int fileNum = 0;
 std:ofstream result("data_without_prob.txt");
    while ((dirp = readdir(dp)) != NULL) {
        if(string(dirp->d_name).length() < 3)continue;
        string fileName = DIR_PATH + string(dirp->d_name);
        //cout <<fileName<<endl;
        std::ifstream from (fileName.c_str());
        string strTemp(dirp->d_name);
        int pos = strTemp.find(".");
        int itemId = atoi(strTemp.substr(0,pos).c_str());
       
        result<<itemId<<":"<<'\n';
        while(from.getline(rateStr,256)){
            strTemp = rateStr;
            pos1 = strTemp.find(",");
            userId = atoi(strTemp.substr(0,pos1).c_str());
            if(probeMatrix[userId].find(itemId) != probeMatrix[userId].end()) continue;
            result<<rateStr<<'\n';
            //cout<<rateStr<<'\n';
        }
        from.close();
        ++fileNum;     
        //if(fileNum == 1)break;
        if(fileNum %100 ==0)cout<<"merge file "<<fileNum<<" sucessfully!"<<endl;
    }
    result.close();
    return 0;
}
