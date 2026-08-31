# Data Structures in C

A collection of classic data structures and algorithms implemented in C, organized by module. Each module is self-contained and includes a `main()` for demonstration, so every source file can be compiled and run independently.

## Directory Structure

```
.
├── struct.c                    # C 结构体基础示例
├── linkedlist/                 # 链表
├── stack_queue/                # 栈、队列、中缀转后缀
├── heap/                       # 二叉堆
├── hash/                       # 哈希表（多种冲突解决策略）
├── disjointset/                # 并查集
├── tree/                       # 二叉树
└── graph/                      # 图论算法
    └── maxflow/                # 最大流
```

## Modules

### Linked List — `linkedlist/`

单链表的完整实现，头文件与实现分离。

- `linkedlist.h` / `linkedlist.c`：链表 API（创建、释放、追加、插入、按索引读写、查找、删除、遍历）。
- `main.c`：基于 API 的测试程序。

### Stack & Queue — `stack_queue/`

- `stack_queue.h` / `stack_queue.c`：基于数组的栈与队列（`push` / `pop` / `enqueue` / `dequeue` 等）。
- `infix-postfix.c`：中缀表达式转后缀表达式（支持 `+ - * /` 及括号，基于运算符优先级）。

### Heap — `heap/`

二叉堆（最小堆）实现：

- 上滤（`percolateup`）与下滤（`percolatedown`）
- 建堆 `build_heap`
- 插入、删除最小值
- 堆排序 `heapsort`

### Hash Table — `hash/`

三种哈希表冲突解决策略：

- `hash_sepqrate_chaining.c`：分离链接法（链表）
- `hash_open_address.c`：开放定址法（线性探测 / 平方探测 / 双散列，可切换 `mode`）
- `hash_robinhood.c`：Robin Hood 哈希（按探测距离 `dist` 平衡）

均提供 `insert` / `delete` / `search` 接口。

### Disjoint Set — `disjointset/`

并查集实现，采用按大小合并（`unionset_size`），支持 `find` 路径压缩与连通性查询 `check`。

### Tree — `tree/`

- `binarytree.c`：完整二叉搜索树，包含：
  - 由数组构建（`build_tree`）
  - 递归 / 非递归的前、中、后序遍历
  - 中序与前序线索化（Threaded Tree）及线索遍历
  - 查找、插入、删除、求深度
- `zigzaggingtree.c`：由中序 + 后序序列重建二叉树，并进行「之字形」（Zigzag）层序遍历。

### Graph — `graph/`

- `toposort.c`：拓扑排序（基于入度）。
- `strong_connect.c`：Tarjan 算法求强连通分量。
- `AOE.c`：AOE 网关键路径（最早/最晚发生时间、关键活动）。
- `Dijkstra.c`：Dijkstra 单源最短路径（最小堆优化）。
- `min_spanning_tree.c`：最小生成树（Prim 与 Kruskal）。
- `maxflow/`：
  - `Edmonds-Karp.c`：Edmonds-Karp 最大流（顶点名哈希映射）。
  - `maxflow.c`：Edmonds-Karp 与 Dinic 算法实现。
  - `pseudo_code.txt`：Dinic 算法伪代码。

## Compilation & Run

每个源文件均自带 `main`，可直接编译运行，例如：

```bash
# 链表
gcc -o linkedlist linkedlist/linkedlist.c linkedlist/main.c && ./linkedlist

# 堆
gcc -o heap heap/heap.c && ./heap

# 哈希表
gcc -o hash hash/hash_robinhood.c && ./hash

# 图论
gcc -o dijkstra graph/Dijkstra.c && ./dijkstra
```

> 提示：部分程序（如图算法）在运行时按提示输入顶点、边等信息；`linkedlist/main.c` 的测试程序需要一个输出文件路径作为参数。

## Notes

- 代码主要用于学习与教学，聚焦算法思想，未做过多工程化封装。
- 所有实现使用标准 C（`gcc` 即可编译），无第三方依赖。
