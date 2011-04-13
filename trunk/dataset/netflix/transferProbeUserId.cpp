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
 *  Transfer the probe.txt of netflix dataset  to probe_t.txt with  sequential userIds
 */
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <math.h>
#include <map>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <dirent.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>
#define USER_NUM 480189
#define ITEM_NUM 17770
using namespace std;

int main (void)
{
    std::ifstream userIdFile("userIds");
    std::ifstream in("probe.txt");
    std::ofstream out("probe_t.txt");
    map<int,int> userMap;
    char rateStr[256];
    int pos1,pos2,str1,str2;
    string strTemp;
    //load userIdMap
    while(userIdFile.getline(rateStr,256)){
        strTemp = rateStr;
        pos1 = strTemp.find('\t');
        str1 = atoi(strTemp.substr(0,pos1).c_str());
        str2 = atoi(strTemp.substr(pos1+1).c_str());
        //cout<<str1<<'\t'<<str2<<'\t'<<endl;exit(1);    
        //initialize rateMatrix
        try {
            userMap[str1] = str2;
        }
        catch (bad_alloc& ba){
            cerr << "bad_alloc caught: " << ba.what() << endl;
        }
    }
    userIdFile.close(); //finish loading userId map
    
    int itemId = 0;
    int itemNum = 0;
    int pNum = 0;
    int newUserId = 0;
    while(in.getline(rateStr,256)){
        strTemp = rateStr;
        int pos = strTemp.find(":");
        if(-1 != pos) {
            itemId = atoi(strTemp.substr(0,pos).c_str());
            if(0 == itemId ) {
                cout<<strTemp<<"#####################"<<pos<<"####"<<strTemp.substr(0,pos).c_str()<<endl;
                exit(1);
            }        
            ++itemNum;     
            continue;
        }
        if(strTemp.length() < 1)continue;
        int userId = atoi(rateStr);
        if(0 == itemId ) {
            cout<<userId<<"#####################"<<endl;
            exit(1);
        }        
        //initialize rateMatrix
        try {
            newUserId = userMap[userId];
            out<<itemId<<","<<newUserId<<endl;
            ++pNum;
        }
        catch (bad_alloc& ba)
            {
                cerr << "bad_alloc caught: " << ba.what() << endl;
            }
    }
    in.close();
    out.close();
    cout<<"Transfer sucessful! There are "<<itemNum<<" items and "<<pNum<<" probe ratings!"<<endl;
}
