**重要更新:** svd++ 和 combine(svd++和lfGNbr的结合）算法有重要bug，在更新sumQE的时候不对，现在已经修正，请大家更新代码svn update.
更新的内容如下："`sumQE[K_NUM+1] = eui * q[itemI][k];`”  改成  "`sumQE[k] += eui * q[itemI][k];`"



**important update:** There are very big bugs on svd++ model and combine model(svd++ and latent factor global neighborhood model! Please update your code using "svn update" command!
The details of change is: "`sumQE[K_NUM+1] = eui * q[itemI][k];`" was changed to "`sumQE[k] += eui * q[itemI][k];`"



[English Version](http://code.google.com/p/recsyscode/#English_version:)


**项目发起的缘由**<br />
在实现koren论文的算法的时候我遇到了很多问题：<br />
（1）针对大规模数据的时候（100M的打分数据），以前那种粗放型的使用cpu和内存的方法完全行不通，因为数据量大，算法和数据结构不考虑周到则时间和空间消耗都难以忍受。<br />
（2）数据的初始化和参数的设置对结果有很大的影响，为了复现koren的结果，我第一个svd的程序大概花了2周时间才搞定，中间走了很多弯路，光调参数就花了4天。<br />
（3）其他一些困难，此处省略1000字:-) ……

为了减少推荐系统领域的朋友入门的难度，我将一些推荐算法的细节展现出来，通过代码的形式呈现给大家，给大家一个好的参照，使大家能尽快上手，减小入门的门槛，希望能为推荐系统领域的发展尽一些绵薄之力！希望有更多的人研究这个有趣且有用的领域！


**代码说明**：<br />
(1)所有的代码都是用c++实现（c++效率高，对于像netflix这样的大规模数据，脚本语言处理起来太慢）<br />
(2)代码使用GPL V3协议发布，大家在使用的时候请保留版权信息。<br />
(3)目前实现的算法有：baseline predictor, knn, svd, svd++, asymmetric-svd, global neighborhood based model(gNbr), combine of svd++ and gNbr.<br />
(4)[编码规范](codestyle.md)<br />
(5)代码中肯定有很多不完善和错漏的地方，如果发现，请给我发邮件，也希望大家和我一起完善这个project。<br />
(6)所有代码在debian 6.0和RHEL AS4（gcc3.4.6)上测试通过，其他环境因为条件所限没有测试<br />


**一些有用的链接**<br />
(1) 新人第一步：[快速使用本project的入门指南](quickstart.md)<br />
(2) 获取本project中用到的netflix的测试集和训练集数据的方法：[netflix数据预处理方法](netflixpreprocess.md)<br />
(3) [本project中knn model使用步骤](knnstep.md)<br />
(4) [在代码实现过程中遇到的问题](questions.md)<br />
(5) [knn算法执行的一些结果](knnResults.md)<br />
(6) [svd算法执行的一些结果](SVDResult.md)<br />
(7) [目前能够获得的一些测试数据集](dataset.md)<br />
(8) [Recommender system Handbook下载](http://www.lifecrunch.biz/archives/194)<br />
(9) [Recommender Systems:An Introduction下载](http://www.lifecrunch.biz/archives/234)<br />
(10) [一些相关的论文资料](papers.md)

**希望有更多的人加入这个project，将更多的算法代码贡献出来，比如目前尚缺RBM model，temporal model的实现**<br />
想加入开发的或者交流的朋友可以从这里很方便的联系我:[我的联系方式](http://www.lifecrunch.biz/contactme) 或者直接给我发mail，honglianglv at gmai的邮箱

ps: 这也是我的第一个开源项目，用了这么多的开源软件，今天算是迈出了回馈开源界的第一步，以后如果有好的东西我也会分享给大家

# English version: #

**Why start this project?** <br />

I encountered a lot of difficulties when I implement the  classic algorithms of recommender system:

(1) for large-scale data (the netflix dataset,100M scoring data), the way of arbitrary using the cpu and memory does not work. Because the large amount of data, algorithms and data structures should be compactly designed to avoid too large time and space consumption  to accept.

(2) data initialization and parameter setting has a great influence on the results, in order to reproduce the results of koren, my first svd model implementation took about 2 weeks, including  4 days of tuning parameters.

(3) some other difficulties, ......

In order to reduce the difficulty of entering the field of recommendation system. I provide some details of the algorithms as long as the koren's papers in the form of code. So that the newbie of recommender system can get started as soon as possible.And also give the friends in the recommender system a good reference. Also hope there are  more and more people to enter this interesting and useful area! <br />

**Code Description:** <br />
(1) all of the code is written by c + + (c + +, with high efficiency for large dataset such as netflix dataset. Scripting language is too slow)

(2) The code released under the license GPL V3. Please retain the copyright information when use the code.

(3)Current finished algorithms: baseline predictor, knn, svd, svd + +, asymmetric-svd, global neighborhood based model (gNbr), combine of svd + + and gNbr.

(4) [code style](codestyle.md)

(5) the code must have a lot of imperfections and mistakes here, if you find some bug or mistake, please email me. I also hope you join me to perfect this project.

(6)all the code are tested under the enviroment of debain 6.0 and RHEL AS4(gcc 3.4.6).<br />

**Some useful links:**<br />
(1) quick start:  [quick start of this project](quickstart.md)

(2) The way to get the training set and test set of netflix dataset:[netflix dataset preprocessing](netflixpreprocess.md)

(3) [the steps of using knn model in this project](knnstep.md)

(4) [the problems I encountered in the implementation](questions.md)

(5) [the results of knn algorithm](knnResults.md)

(6) [the results of svd algorithm](SVDResult.md)

(7) [the datasets available now](dataset.md)

(8) [Recommender system Handbook download](http://www.lifecrunch.biz/archives/194)

(9) [some papers related to this project](papers.md)

**Hope more people to join this project, contribute more code to this project. Such as the current shortfall: the implimentation of RBM model and temporal model**<br />

Friends who want to join this project can [contact me here](http://www.lifecrunch.biz/contactme), or contact me directly via email, honglianglv at gmail<br />

ps: This is my first open source project after I have benefited from open source softwear for so many years. This is my first step on the way of open source and I hope to contribute more in the future.