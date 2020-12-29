<!-- TOC -->

- [1. 第十次作业](#1-第十次作业)
    - [1.1. （一）题目](#11-一题目)
    - [1.2. （二）最原始的算法（对角数组+顺序局部搜索）](#12-二最原始的算法对角数组顺序局部搜索)
        - [1.2.1. 数组设计以及评估函数](#121-数组设计以及评估函数)
        - [1.2.2. 局部搜索策略](#122-局部搜索策略)
        - [1.2.3. 实验结果](#123-实验结果)
            - [1.2.3.1. 不同N值情况下的计算时间](#1231-不同n值情况下的计算时间)
    - [1.3. （三）改进算法（对角数组+局部随机化最小冲突搜索）](#13-三改进算法对角数组局部随机化最小冲突搜索)
        - [1.3.1. 核心改进：局部随机化最小冲突搜索](#131-核心改进局部随机化最小冲突搜索)
        - [1.3.2. 实验结果（相比之前的提高了两个数量级，5s求解百万N皇后问题）](#132-实验结果相比之前的提高了两个数量级5s求解百万n皇后问题)
    - [1.4. （四）NASA-N-Queen算法（1s求解千万N皇后问题）](#14-四nasa-n-queen算法1s求解千万n皇后问题)
        - [1.4.1. 核心改进](#141-核心改进)
            - [1.4.1.1. 棋盘初始化](#1411-棋盘初始化)
            - [1.4.1.2. 对角线计数以及行计数](#1412-对角线计数以及行计数)
            - [1.4.1.3. 新解最小冲突](#1413-新解最小冲突)
            - [1.4.1.4. 实验结果](#1414-实验结果)
    - [1.5. （五）结论](#15-五结论)
        - [1.5.1. **N的上限**](#151-n的上限)

<!-- /TOC -->

# 1. 第十次作业

## 1.1. （一）题目

> 局部搜索解n皇后，并测试n的极限
>
> n皇后问题：在一个NxN的棋盘上放置N个皇后，使得每个皇后都满足以下条件：
>
> 1. 不在同一行
> 2. 不在同一列
> 3. 不在同一对角线

## 1.2. （二）最原始的算法（对角数组+顺序局部搜索）

### 1.2.1. 数组设计以及评估函数

- **不在同一行**：可以使用一个长度为N的数组arr，其中arr[i]表示的含义是位置(i, arr[i])处放置一个皇后，由于下标都是不同的，所以皇后都不在同一行中。
- **不在同一列**：通过控制arr数组中每个元素的互不相同且值在1-N之间，从而可以达到不在同一列的目的。
- **不在同一对角线**：这个条件比较难以实现，我们可以将其作为评估一个放置方式好坏程度的数值。

> 通常的判断是否在同一对角线的方式是

```python
for i in range(N):
	for i in range(i+1, N):
		if abs(arr[i] - arr[j]) == asb(i - j):
			# 在同一个对角线发生冲突
```

上面方法的复杂度是$O(n^2)$，因为每次更新都需要进行评估，所以较高的复杂度影响会很大。

<img src="https://github.com/mj-love-life/NQueen/blob/main/assets/1606557757023.png?raw=true" width="500"  height = "400"  />

通过对上面的图进行观察我们可以发现在同一对角线的元素$(r_1, c_1)$和$(r_2, c_2)$满足以下两个条件：

- $r_1 + c_1 $与$r_2 + c_2$相同  **或**
- $r_1 - c_1 $与$r_2 - c_2$相同

所以可以得到新的计算规则

```python
for i in range(N):
    if Adds[arr[i] + i] > 0:
        # 在同一对角线有冲突
    if Minus[arr[i] - i] > 0:
        # 在同一对角线有冲突
    Adds[arr[i] + i]+=1
    Minus[arr[i] - i]+=1
```

这种方法的时间复杂度为O(n)，**另外可以通过计算变化量的方式使时间降为O(1)**

### 1.2.2. 局部搜索策略

> 在本人实现的局部搜索策略中，采用2-swap的操作，遍历选择两个arr元素进行对换，这可以避免产生列冲突的问题
>
> 同时因为局部搜索可能找不到最优解，所以才每次一次完整的2-swap操作并没有满足条件之后进行重新对arr数组重新放置，reset操作并继续训练。

```python
# 采用CurBest记录当前最优质

# 重置操作
def Reset():
	# 对数组重置
	shuffle_arr
	CurBest = GetEvaluation(arr)

while CurBest not 0:
	NextBest = CurBest
	for i in range(N):
		for j in range(i + 1, N):
			swap(arr[i], arr[j])
			if GetEvaluation(arr) < NextBest:
				NextBest = GetEvaluation(arr)
            else:
                swap(arr[i], arr[j])
    # 上面为一次完成的2-swap操作
    # 如果值保持不变，说明陷入局部最优解，所以进行重置重新训练。
    if NextBest >= CurBest:
        Reset()
    else :
        CurBest = NextBest
```



### 1.2.3. 实验结果

**本实验中对不同的N值进行多次实验并计算平均值**

#### 1.2.3.1. 不同N值情况下的计算时间

![1606671275066](https://github.com/mj-love-life/NQueen/blob/main/assets/1606671275066.png?raw=true)

![1606671308620](https://github.com/mj-love-life/NQueen/blob/main/assets/1606671308620.png?raw=true)

## 1.3. （三）改进算法（对角数组+局部随机化最小冲突搜索）

> 该算法是得到了后面nasa的算法的启发，在大规模问题下可以通过随机化以及冲突最小化的方式来进行交换。

### 1.3.1. 核心改进：局部随机化最小冲突搜索

- 前面的方法是两个for循环进行交换，但是这在N较大的情况下是不合适的，时间开销较高，所以可以产生一个与i不相同的随机数index，直接将两者进行交换。
- 直接交换有一个前提，当前的皇后是一个差解，我们可以利用对角线数组$Adds[i, NQueen[i]] + Minus[i, NQueen[i]] >2$时说明当前解并不是最优的，因为如果最优的话则两者都为1，相加为2。
- 直接交换还有一个问题需要考虑：就是交换之后的效果如何，我们需要交换后的效果比交换之前的好，这个可以利用前面求差值的函数来进行计算，当差值小于0时说明进行交换的结果更好。
  - 如果没有更好，可以使用while循环直到找到更好解。

代码如下：

```c++
for (int i = 0; i < N; i++)
{
    int temp = GetConfilict(i, NQueen[i]);
    // 大于2说明结果较差
    if (temp > 2)
    {   
       	// 直到结果变好跳出循环
        while(true) {
            index1 = NRand(e);
            // 产生随机值直到不相等
            while(index1 == i) index1 = NRand(e);
            // 评估交换结果
            eva = GetEvalution(index1, NQueen[index1], i, NQueen[i]);
            swap(NQueen[index1], NQueen[i]);
            // 如果交换结果更好，则break推出循环
            if (eva < 0) {
                NextBest = NextBest + eva;
                break;
            }
            else {
                // 交换结果较差，交换回来
                eva = GetEvalution(index1, NQueen[index1], i, NQueen[i]);
                swap(NQueen[index1], NQueen[i]);
            }
        }
    }
}
```

### 1.3.2. 实验结果（相比之前的提高了两个数量级，5s求解百万N皇后问题）

**只展示$10^3$以上的数据**

![1606885028442](https://github.com/mj-love-life/NQueen/blob/main/assets/1606885028442.png?raw=true)

## 1.4. （四）NASA-N-Queen算法（1s求解千万N皇后问题）

> 参考nasa的求解N皇后问题的文章
>
> [An extended abstract: A heuristic repair method for constraint-satisfaction and scheduling problems](https://ntrs.nasa.gov/citations/19930009498)

### 1.4.1. 核心改进

#### 1.4.1.1. 棋盘初始化

初始化阶段 **并不是随机初始化，而是通过极为简单的策略使得冲突降低**

如下图所示

<img src="https://github.com/mj-love-life/NQueen/blob/main/assets/1606837638604.png?raw=true" width="300"  height = "300"  />

先将皇后放在奇数列，再将皇后放置在偶数列。

```c++
int t = N-1;
for(int i = 0; i < N; i++) {
    if (t < 0) t = N - 2;
    NQueen[i] = t;
    t -= 2;
}
```

经过计算发现这种方法与shuffle的之后的冲突量对比如下：

| N       | $1 \times  10^3$ | $1 \times 10^4$ | $1 \times 10^5$ | $1 \times 10^6$ | $2 \times 10^7$ | $2  \times 10^8$ | $5 \times 10^8$ |
| ------- | ---------------- | --------------- | --------------- | --------------- | --------------- | ---------------- | --------------- |
| MHC     | 334              | 3334            | 333334          | 3333334         | 1               | 1                | 1               |
| shuffle | 668              | 6698            | 666752          | 6667895         | 13332270        | 133301573        | 333240013       |

可以看到大多数情况下冲突数量至少减半，并且在超大规模情况下冲突降为1。

#### 1.4.1.2. 对角线计数以及行计数

- 这个算法产生新解的时候不是通过两两交换得到的。而是寻找冲突最小的行。因此容忍行冲突的存在。
- 需要一个RowCnt记录每行的数量，以及对角线记录每条对角线的皇后数量。
- 用一个EmptyRow记录不放置皇后的行的id，每次寻找新的位置时先从这里寻找。

#### 1.4.1.3. 新解最小冲突

冲突函数如下：

```c++
int inConflict(int row, int col) {
    return RowCnt[col] + Adds[AddsIndex(row, col)] + Minus[MinusIndex(row, col)];
}
```

当该函数返回值为3时，说明当前row以及col位置放置皇后不存在冲突。

当寻找放置位置时，则尽可能是的该函数返回值为1，也就是 **inConflict(row, TestCol)** 为1，表示这一行要么有一个元素但不存在对角冲突，要么只有对角冲突不存在元素等。所以搜索的代码如下：

```
int FindMinConfictY(int x) {
	// 在emptyRows中先进行寻找冲突最小的
    for(int i = 0; i < emptyRows.size(); i++) {
        if (inConflict(x, emptyRows[i]) == 0) {
            return emptyRows[i];
        }
    }
    while(true){
    	// 产生随机数在解空间内随找冲突满足最小的位置
        int y = NRand(e);
        if (inConflict(x, y) == 1 && y != NQueen[x]) {
            return y; 
        }
    }
}
```

#### 1.4.1.4. 实验结果

![1606843139426](https://github.com/mj-love-life/NQueen/blob/main/assets/1606843139426.png?raw=true)

## 1.5. （五）结论

### 1.5.1. **N的上限**

1. **最原始的算法中当$N=10^4$时时间开销为6s**
2. **改进的算法中当$N=10^6$时时间开销为5s**
3. **nasa的算法中当$N=2 * 10^8$时时间开销为8s，而且当$N=10^9$时时间开销为361s，此时消耗内存大约20g，如果用数组存储应该可以减少部分的内存开销。所以限制nasa算法解决的n皇后问题的极大一部分是因为内存大小的原因。**