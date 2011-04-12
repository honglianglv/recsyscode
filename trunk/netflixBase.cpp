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
 * This file contains some common operations of netflix dataset, for example reading the test set
 * and reading the test set
 *
 * the ratings of  training set store in the array (USER X ITEM, user is the row ,and item is the column)
 */
#ifndef NETFLIXBASE_CPP_
#define NETFLIXBASE_CPP_

/**
 * load the training set of netflix dataset
 * 
 */
void loadRating(char * fileName, vector< vector<rateNode> >& rateMatrixLocal, const char* separator)
{
    char rateStr[256];
    char* pch;    
    vector<string> rateDetail;
    int fileNum = 0;
    std::ifstream from (fileName);
    if (!from.is_open()) {
        cout << "can't open  operation failed!\n";
        exit(1);
    }
    
    int itemId = 0, userId = 0, rate = 0;
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
                cout<<"read file "<<fileNum<<" sucessfully!"<<endl;
            }
            continue;
        }
        if(strTemp.length() < 3)continue;
        int i = 0;
        pch = strtok (rateStr,separator);
        while (pch != NULL) {
            if(0 == i) userId = atoi(pch);
            else if(1 == i) rate = atoi(pch);
            else if(i > 1) break;
            ++i;
            pch = strtok (NULL,separator);
        }
        if(0 == itemId || 0 == userId ||  0 == rate) {
            cout<<strTemp<<"#####################"<<endl;
            exit(1);
        }        
        //initialization rateMatrix
        try {
            rateNode tmpNode;
            tmpNode.item = itemId;
            tmpNode.rate = (short)rate;
            rateMatrixLocal[userId].push_back(tmpNode);
        }
        catch (bad_alloc& ba) {
            cerr << "bad_alloc caught: " << ba.what() << endl;
            cout << "Can't allocate the momery!" << endl; exit(1);
        }
    }
    from.close();
       cout<<"read file sucessfully!"<<endl;
    return;
}


//load test set of netflix dataset
void loadProbe(char * fileName,vector<testSetNode>& probeSet, const char* separator)
{
    ifstream in(fileName);
    if (!in.is_open()) {
        cout << "can't open test set file!\n";
        exit(1);
    }
    char rateStr[256];
    char* pch ; // store a token of a string

    string strTemp;
    int rateValue,itemId,userId,probeNum=0;
    
    while(in.getline(rateStr,256)){
        strTemp = rateStr;
        if(strTemp.length() < 4) continue;
        int i = 0;
        pch = strtok (rateStr,separator);
        while (pch != NULL) {
            if(0 == i) itemId = atoi(pch);
            else if(1 == i) userId = atoi(pch);
            else if(2 == i) rateValue = atoi(pch);
            else if(i > 2) break;
            ++i;
            pch = strtok (NULL,separator);
        }
        try {
            testSetNode tmpNode;
            tmpNode.item = itemId;
            tmpNode.rate = (short)rateValue;
            tmpNode.user = userId;
            probeSet.push_back(tmpNode);
            ++probeNum;
        }
        catch (bad_alloc& ba) {
            cerr << "bad_alloc caught: " << ba.what() << endl;
            cout << "Can't allocate the momery!" << endl; exit(1);
        }
    }
    cout << "Load " << probeNum << " test ratings successfully!"<<endl;
    in.close(); 
}
#endif // NETFLIXBASE_CPP_
