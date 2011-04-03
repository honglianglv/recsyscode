/* This file is Copyright (C) 2011 Lv Hongliang. All Rights Reserved.
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
 * the GNU General Public License as published by the Free Software
 * Foundation; the version is 3.
 *
 * 本程序的目的是实现koren在SIGKDD'08论文中的SVD方法, 使用的数据是netflix的数据
 */

#include "../commonHeader.h"
#include "../common.cpp"
#define USER_NUM 480189  //10K:943 1M:6040
#define ITEM_NUM 17770 //10K:1682 1M:3900
#define K_NUM  30
#define TRAINING_SET "../dataset/netflix/data_without_prob.txt"
#define PROBE_SET "../dataset/netflix/probe_real.txt"



namespace svd{
    //使用一些全局变量，存储需要估计的参数，bu，bi,wij
    double bu[USER_NUM+1] = {0};
    double bi[ITEM_NUM+1] = {0};       //baseline预测器中的用户偏置和item偏置
    
    int buNum[USER_NUM+1] = {0};    //用户u打分的item总数，
    int biNum[ITEM_NUM+1] = {0};   //打过item i分的用户总数
    
    double p[USER_NUM+1][K_NUM+1] = {0};   //用于存储用户的属性描述p
    double q[ITEM_NUM+1][K_NUM+1] = {0};   //用于item的属性描述q
    
    vector < vector<rateNode> > rateMatrix(USER_NUM+1);           //使用一个map数组存储稀疏的打分矩阵                     
    float mean = 0;                         //全局的平均值
    
    //函数声明
    void RMSEProbe(const char* probeSet);
    float predictRate(int user,int item);
    void loadRating(char * fileNam, vector< vector<rateNode> >& rateMatrixLocal, const char* probe);
    
    void model(int dim, float  alpha1, float alpha2, float beta1, float beta2)
    {
        cout << "begin initialization: " << endl;
        
        loadRating(TRAINING_SET,rateMatrix,",");  //初始化完成
        
        mean = setMeanRating(USER_NUM,rateMatrix); //求平均值，求bu和bi的值
        
        
        int i,u,j,k;
        
        srand((unsigned)time(0));
        //@TODO 不知道是否能针对初始化的过程做一些优化
        //对p,q进行初始化，初始化的方法是随机函数，不知道这种方法是否好，是否会影响结果？？？？？？？
        for(int i = 1; i < ITEM_NUM+1; ++i){
            setRand(q[i],K_NUM+1,0);    //初始化q[i]
        }
        
        for(int i = 1; i < USER_NUM+1; ++i){
            setRand(p[i],K_NUM+1,0);    //初始化p[i]
        }
       
        cout <<"initialization end!"<<endl<< "begin iteration: " << endl;
        
        float pui = 0.0 ; // 预测的u对i的打分
        double preRmse = 1000000000000.0; //用于记录上一个rmse，作为终止条件的一种，如果rmse上升了，则停止
        double nowRmse = 0.0;
        
        cout <<"begin testRMSEProbe(): " << endl;
        RMSEProbe(PROBE_SET);
        
        //main loop
        for(int step = 0; step < 60; ++step){  //只迭代60次
            long double rmse = 0.0;
            int n = 0;
            for( u = 1; u < USER_NUM+1; ++u) {   //循环处理每一个用户 
                
                   int RuNum = rateMatrix[u].size(); //用户u打过分的item数目
                   float sqrtRuNum = 0.0;
                   if(RuNum>1) sqrtRuNum = (1.0/sqrt(RuNum));
                   
                //迭代处理
                for(i=0; i < RuNum; ++i) {// 循环处理u打分过的每一个item
                    int itemI = rateMatrix[u][i].item;
                    short rui = rateMatrix[u][i].rate; //实际的打分
                    double bui = mean + bu[u] + bi[itemI];
                    pui = predictRate(u,itemI);
                    
                    float eui = rui - pui;
                    
                    if( isnan(eui) ) {// fabs(eui) >= 4.2 || 
                        cout<<u<<'\t'<<i<<'\t'<<pui<<'\t'<<rui<<"    "<<bu[u]<<"    "<<bi[itemI]<<"    "<<mean<<endl;
                        //printArray(q[itemI],p[u],K_NUM+1);
                        exit(1);
                    }
                    rmse += eui * eui; ++n;
                    if(n % 10000000 == 0)cout<<"step:"<<step<<"    n:"<<n<<" dealed!"<<endl;
                    
                    bu[u] += alpha1 * (eui - beta1 * bu[u]);
                    bi[itemI] += alpha1 * (eui - beta1 * bi[itemI]);
                    
                    for( k=1; k< K_NUM+1; ++k) {
                           //double tempPu = p[u][k];
                           p[u][k] += alpha2 * (eui*q[itemI][k] - beta2*p[u][k]);
                           q[itemI][k] += alpha2 * (eui*p[u][k] - beta2*q[itemI][k]);
                       }
                } 
            }
            nowRmse =  sqrt( rmse / n);
            
            if( nowRmse >= preRmse && step >= 3) break; //如果rmse已经开始上升了，则跳出循环
            else
                preRmse = nowRmse;
            cout << step << "\t" << nowRmse <<'\t'<< preRmse<<"     n:"<<n<<endl;
            RMSEProbe(PROBE_SET);  // 检查训练集情况
            
            alpha1 *= 0.9;    //逐步减小学习速率
            alpha2 *= 0.9;
        }
        RMSEProbe(PROBE_SET);  // 检查训练集情况
        return;
        
    }
    
   /**
	 * load filePath中的数据到data这个vector中和 rateMatrix中
	 * 
	 */
	void loadRating(char * fileNam, vector< vector<rateNode> >& rateMatrixLocal, const char* probe)
	{
	    char rateStr[256];    
	    vector<string> rateDetail;
	    int fileNum = 0;
	    std::ifstream from (TRAINING_SET);
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
	    	explode(probe,strTemp,rateDetail);
	    	userId = atoi(rateDetail[0].c_str());
	    	rate = atoi(rateDetail[1].c_str());
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
	    	catch (bad_alloc& ba) {
	    		cerr << "bad_alloc caught: " << ba.what() << endl;
	    	}
	    }
	    from.close();
	   	cout<<"read file sucessfully!"<<endl;
	    return;
	}
    
    float predictRate(int user,int item)
    {
        int RuNum = rateMatrix[user].size(); //用户u打过分的item数目
        float sqrtRuNum = 0.0;
        double ret; 
        if(RuNum > 1) {
            ret = mean + bu[user] + bi[item] +  dot(p[user],q[item],K_NUM);//这里先不对k进行变化，先取k=无穷大
        }
        else ret  = mean+bu[user] + bi[item];
        if(ret < 1.0) ret = 1;
        if(ret > 5.0) ret = 5;
        return ret;
    }
    
    //检查测试集情况
    void RMSEProbe(const char* probeSet)
    {
        
        /*  1、load test数据集，
            2、针对每一条数据，计算预测值，然后计算误差的平方和，计算总数量
            3、输出rmse  
        */
         //首先读取probe_real.txt 文件，对于每一个rate，算出预测的打分，根据对应的实际打分计算RMSE
        ifstream in(probeSet);
        char rateStr[256];
        vector<string> rateDetail;
        string strTemp;
        int rateValue,itemId,userId,probeNum=0;
        float pRate,err;
        long double rmse = 0;
        
        while(in.getline(rateStr,256)){
            strTemp = rateStr;
            if(strTemp.length() < 4) continue;
            explode(",",strTemp,rateDetail);
            itemId = atoi(rateDetail[0].c_str());
            userId = atoi(rateDetail[1].c_str());
            rateValue = atoi(rateDetail[2].c_str()); //real rate
            pRate = predictRate(userId,itemId);      //predict rate
            err = pRate-rateValue;
            rmse += err*err;++probeNum;
        }
        rmse = sqrt( rmse / probeNum);
        cout<<"RMSE:"<<rmse<<" probeNum:"<<probeNum<<endl;
        in.close(); 
    }
};

int main(int argc, char ** argv)
{
    time_t start,end;
    struct tm* startInfo;
    struct tm* endInfo;
    double duration;
    start = time(NULL);
    startInfo = localtime(&start);
    string startStr = asctime(startInfo);
    float alpha1 = 0.008;    //according suggestion of xlvector
    float alpha2 = 0.008;    //according suggestion of xlvector
    float beta1  = 0.01;     //according suggestion of xlvector
    float beta2  = 0.01;     //according suggestion of xlvector
    
    //for(int i=0; i < 10; i++)
    {
    //    beta = i*0.001 + 0.002;
    //    cout << beta << endl;
        svd::model(K_NUM,alpha1,alpha2,beta1,beta2);
        
    }
    end = time(NULL);
    duration = end-start;
    endInfo =   localtime(&end);
    cout << "start at" << startStr << ". And end at "<< asctime(endInfo) <<endl;
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
