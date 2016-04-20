#lab6
##stride调度算法是如何避免stride溢出问题

在每次调度中，最大与最小步进值之差始终满足：  
‘’‘
STRIDE_MAX – STRIDE_MIN <= PASS_MAX<= BIG_STRIDE
’‘’

只要保证int(BIG_STRIDE)>0，即BIG_STRIDE<=0x7fffffff可用差值与0比较判断大小。  
当实际stride值为a，b时，假设a已经溢出，其表示值为a-2^32，a-b=a-b-2^32，表示为a-b>0。

实际是保证当前所有stride的取值范围只落在stride可能取值区间的一半以内，就可以在存在溢出时判断其相对大小。