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
 *  merge the rating files to a single one
 */
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#define DIR_PATH "./transfer_set/"
using namespace std;


int main()
{
    DIR *dp;
    char rateStr[256];
    struct dirent *dirp;
   
    if((dp  = opendir(DIR_PATH)) == NULL) {
        cout << "Error(" << errno << ") opening " << DIR_PATH << endl;
           return 0;
    }
    int fileNum = 0;
    ofstream result("data.txt");
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
