#spoc-lec20
##请举一个比较具体的例子，说明银行家算法判断出不安全的状态下不会出现死锁
例子为ppt 20-3 11页：  
![banker](https://github.com/chenyl13/os_spoc/blob/master/banker.JPG)  
此时可用资源小于任意线程的请求，是不安全的。  
但可能不出现死锁，比如线程T2由于某些原因被杀死，并释放了自身所有资源，此后按照T4、T3、T1的请求顺序都能正常运行完毕。