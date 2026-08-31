# Data Structures in C

用 C 语言实现的经典数据结构与算法集合。每个数据结构都被封装成**可复用的库**（`头文件 + 实现文件`），其他代码只需 `#include` 对应头文件并链接实现文件即可调用。根目录的 `example.c` 是一个综合示例，演示了如何调用所有库。

## 目录结构

```
.
├── example.c                  # 综合示例：演示如何调用各库
├── Makefile                   # 构建脚本
├── struct.c                   # C 结构体基础示例（独立 demo）
├── linkedlist/                # 链表库
├── stack_queue/               # 栈与队列库
├── heap/                      # 二叉堆库
├── hash/                      # 哈希表库（三种冲突解决策略）
├── disjointset/               # 并查集库
├── tree/                      # 二叉树 / 之字形树库
└── graph/                     # 图库（含最大流）
    └── maxflow/
```

## 库与 API 概览

| 模块 | 头文件 | 主要接口 |
| --- | --- | --- |
| 链表 | `linkedlist/linkedlist.h` | `list_create` / `list_append` / `list_insert` / `list_get` / `list_set` / `list_remove` / `list_find` / `list_iterate` / `list_free` |
| 栈与队列 | `stack_queue/stack_queue.h` | `creatS` / `push` / `pop` / `visitS` / `freeS`，`creatQ` / `enqueue` / `dequeue` / `visitQ` / `freeQ` |
| 堆 | `heap/heap.h` | `heap_create` / `heap_build` / `heap_insert` / `heap_delete_min` / `heap_min` / `heap_sort` |
| 哈希表 | `hash/hash.h` | 分离链接法 `ch_*`；开放定址法 `oa_*`；Robin Hood `rh_*` |
| 并查集 | `disjointset/disjointset.h` | `disjointset_create` / `disjointset_find` / `disjointset_union` / `disjointset_connected` |
| 二叉搜索树 | `tree/binarytree.h` | `bt_build` / `bt_insert` / `bt_delete` / `bt_find` / 各类遍历 / 线索化 |
| 之字形树 | `tree/zigzaggingtree.h` | `zigzag_build` / `zigzag_print` / `zigzag_free` |
| 图 | `graph/graph.h` | `graph_create` / `graph_add_edge` / `graph_dijkstra` / `graph_prim` / `graph_kruskal` / `graph_toposort` / `graph_scc` / `graph_critical_path` |
| 最大流 | `graph/maxflow/maxflow.h` | `flow_create` / `flow_add_edge` / `flow_edmonds_karp` / `flow_dinic` |

## 各模块说明

- **链表** `linkedlist/`：带头尾指针的单链表，支持增删改查与遍历；`main.c` 是独立的测试程序。
- **栈与队列** `stack_queue/`：基于数组的栈与队列；`infix-postfix.c` 是中缀转后缀的独立示例（用栈实现）。
- **堆** `heap/`：最小堆（上滤 / 下滤），支持建堆、插入、删除最小值和堆排序。
- **哈希表** `hash/`：一个头文件 `hash.h` 统一暴露三种冲突解决策略——分离链接法、开放定址法（线性 / 平方 / 双散列）、Robin Hood 哈希。
- **并查集** `disjointset/`：按大小合并 + 路径压缩。
- **二叉搜索树** `tree/binarytree.c`：构建平衡 BST，支持递归 / 非递归 / 层序遍历、查找、插入、删除，以及中序线索化与线索遍历。
- **之字形树** `tree/zigzaggingtree.c`：由中序 + 后序序列重建二叉树，并进行之字形层序遍历。
- **图** `graph/`：统一邻接表实现（带权，支持有向 / 无向），提供 Dijkstra、Prim、Kruskal、拓扑排序、Tarjan 强连通分量、AOE 关键路径。
- **最大流** `graph/maxflow/`：邻接矩阵实现的容量网络，提供 Edmonds-Karp 与 Dinic 算法；`pseudo_code.txt` 为 Dinic 伪代码。

## 编译与运行

环境要求：支持 C99 的 C 编译器（如 GCC / MinGW）。

### 方式一：使用 Makefile

```bash
make            # 编译 example
./example       # 运行综合示例
make clean      # 清理
```

### 方式二：直接使用 gcc

```bash
gcc -std=c99 -Wall -Wextra -I. -o example example.c \
    linkedlist/linkedlist.c stack_queue/stack_queue.c \
    heap/heap.c hash/hash_open_address.c hash/hash_robinhood.c \
    hash/hash_sepqrate_chaining.c disjointset/disjointset.c \
    tree/binarytree.c tree/zigzaggingtree.c \
    graph/graph.c graph/maxflow/maxflow.c -lm
```

> 说明：`tree/binarytree.c` 使用了 `log2` / `pow`，需要链接数学库 `-lm`。

## 在你的代码中使用

以图库为例，只需包含头文件并链接对应实现：

```c
#include "graph/graph.h"

int main(void)
{
    Graph *g = graph_create(4, 0);       /* 4 个顶点的无向图 */
    graph_add_edge(g, 1, 2, 3);
    graph_add_edge(g, 2, 3, 4);

    int *dist = graph_dijkstra(g, 1);    /* 顶点 1 到各点的最短距离 */
    /* ... 使用 dist ... */

    graph_free(g);
    return 0;
}
```

完整的调用示例见根目录 [`example.c`](example.c)。

## 运行输出示例

```
== Linked List ==
size=3, find(20)=2, iterate: 5 10 20
after remove(20): 5 10

== Stack & Queue ==
stack pop: 3 2
queue dequeue: 7 8

== Heap ==
min=5, delete-min=5
after insert 1, min=1
heap_sort: 1 2 5 7 9

== Graph ==
dijkstra from 1: 0 3 1 5 7
prim total=10, kruskal total=10
...
```

## 注意事项

- 顶点编号约定：图与并查集、最大流中的顶点均从 **1** 开始编号；数组返回值（如 `graph_dijkstra` 的 `dist`）下标从 0 开始。
- 库函数中的 `free` / `_free` 接口负责释放对应结构，请勿重复释放。
- `bt_inorder_thread` 会修改树结构（线索化），其后应使用 `bt_free_threaded` 而非 `bt_free` 释放。
- 某些哨兵返回值（如 `heap_delete_min` 在空堆时返回 `-1`）仅适用于学习场景，生产使用时请自行约定。
