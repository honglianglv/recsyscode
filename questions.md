# Introduction #

Some questions when I implement the algorithms


# Details #
**问题如下
  1. KNN算法中，如何存储稀疏的rate矩阵，是否可以用一个map数组存储？对于netflix数据目前不可以，占用的内存超过3.2G（32位系统用户单进程最大内存），不知道64位系统是否可以。和xlvector交流他的好像可以。问题是，sgi的stl中，一个map节点占用内存多大呢？看了stl源码，仍然不清楚，谁能告诉我呢？**<br />
  1. 为什么movielens数据集中，有这么多item之间的相似度为1，难道我的算法实现有问题？<br />
  1. http://www.lifecrunch.biz/archives/194 Recommender system Handbook]P149, bi和bu的公式下面有一句话，"Average are shrunk towards zero by using the regularization parameters, [r1](https://code.google.com/p/recsyscode/source/detail?r=1),[r2](https://code.google.com/p/recsyscode/source/detail?r=2),which are determined by cross validation" ，如何通过交叉验证的方式选择参数呢，模型参数选择对结果影响非常大，大家实际怎么选择的这些参数呢？<br />