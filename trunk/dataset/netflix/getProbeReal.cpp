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
 *   get the rate of probe set from data.txt
 */
#include <iostream>
#include <string>
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
#define K_NUM  100
#define DIR_PATH "./transfer_set/"
#define TRAINING_SET "./data.txt"
using namespace std;
void getProbeReal();
struct rateNode
{
    short item;
    short rate;
};
int getRate(vector<rateNode> &v, int item)
{
    int size = v.size();
    for(int i = 0; i < size; ++i) {
        if(item == v[i].item)return v[i].rate;
    }
    cout<<"**********************error!!!can't get what you look for!!***********"<<endl;
    return 0;
}



/**
 * load filePath中的数据到rateMatrix中
 * load the the ratings in TRAINING_SET to rateMatrix
 */
void loadRating(char * dirPath, vector< vector<rateNode> >& rateMatrixLocal)
{
    char rateStr[256];    
    vector<string> rateDetail(10);
    int fileNum = 0;
    std::ifstream from (TRAINING_SET);
    if (!from.is_open())
        {
            cout << "Output operation failed!\n";
            exit(1);
        }
    
    int itemId = 0, userId = 0, rate = 0;
    int pos1=0,pos2=0,pos3=0;
    string strTemp = "";
    int totalM = 0;
    int i = 0;
    while(from.getline(rateStr,256)){
        string strTemp(rateStr);
        int pos = strTemp.find(":");
        if(-1 != pos) {
            itemId = atoi(strTemp.substr(0,pos).c_str());
            if(0 == itemId ) {
                cout<<strTemp<<"#####################"<<pos<<"####"<<strTemp.substr(0,pos).c_str()<<endl;
                exit(1);
            }        
            ++fileNum;     
            if(fileNum %3000 ==0) {
                //malloc_stats();  
                //mallinfo();
                cout<<"read file "<<fileNum<<" sucessfully!"<<endl;
            }
            continue;
        }
        if(strTemp.length() < 3)continue;
        pos1 = strTemp.find(",");
        userId = atoi(strTemp.substr(0,pos1).c_str());
        pos2 = strTemp.find(",",pos1+1);
        rate = atoi(strTemp.substr(pos1+1,pos2-pos1-1).c_str());
        //cout<<userId<<'\t'<<itemId<<'\t'<<rate<<endl;exit(1);
        if(0 == itemId || 0 == userId ||  0 == rate ) {
            cout<<userId<<"#####################"<<endl;
            exit(1);
        }        
        //初始化rateMatrix
        try {
            rateNode tmpNode;
            tmpNode.item = itemId;
            tmpNode.rate = (short)rate;
            rateMatrixLocal[userId].push_back(tmpNode);
        }
        catch (bad_alloc& ba)
            {
                cerr << "bad_alloc caught: " << ba.what() << endl;
            }
    }
    from.close();
       cout<<"read file sucessfully!"<<endl;
    return;
}


vector < vector<rateNode> > rateMatrix(USER_NUM+1);

int main()
{   
    loadRating(DIR_PATH,rateMatrix);  //finish loading all the data
    getProbeReal();
}


void getProbeReal()
{
    //首先读取probe_t.txt 文件，对于每一个rate，找出对应的实际打分
    //first read the file probe_t.txt, for every record, find the real rate.
    ifstream in("probe_t.txt");
    ofstream out("probe_real.txt");
    ofstream logNP("n_p.txt");
    char rateStr[256];
    int pos1,pos2;
    string strTemp;
    int rateValue,itemId,userId;
    //load userIdMap
    while(in.getline(rateStr,256)){
        strTemp = rateStr;
        pos1 = strTemp.find(",");
        itemId = atoi(strTemp.substr(0,pos1).c_str());
        userId = atoi(strTemp.substr(pos1+1).c_str());
        //cout<<itemId<<'\t'<<userId<<'\t'<<endl;            exit(0);
        rateValue = getRate(rateMatrix[userId],itemId);
        if(rateValue == 0) {
            logNP<<itemId<<','<<userId<<','<<rateValue<<endl;
        }
        out<<itemId<<','<<userId<<','<<rateValue<<endl;
    }
    in.close(); 
    out.close();
    logNP.close();
}
