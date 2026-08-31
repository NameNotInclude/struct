CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
LDLIBS = -lm

LIBS = \
	linkedlist/linkedlist.c \
	stack_queue/stack_queue.c \
	heap/heap.c \
	hash/hash_open_address.c \
	hash/hash_robinhood.c \
	hash/hash_sepqrate_chaining.c \
	disjointset/disjointset.c \
	tree/binarytree.c \
	tree/zigzaggingtree.c \
	graph/graph.c \
	graph/maxflow/maxflow.c

# 综合示例：演示如何调用各数据结构库
example: example.c $(LIBS)
	$(CC) $(CFLAGS) -o example example.c $(LIBS) $(LDLIBS)

# 额外的独立示例
linkedlist-demo: linkedlist/linkedlist.c linkedlist/main.c
	$(CC) $(CFLAGS) -o linkedlist-demo linkedlist/linkedlist.c linkedlist/main.c

infix-postfix: stack_queue/infix-postfix.c
	$(CC) $(CFLAGS) -o infix-postfix stack_queue/infix-postfix.c

.PHONY: clean
clean:
	rm -f example linkedlist-demo infix-postfix
