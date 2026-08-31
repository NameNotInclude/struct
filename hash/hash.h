#ifndef HASH_H
#define HASH_H

/*
 * 哈希表集合（key 为 int），提供三种冲突解决策略：
 *   1. 分离链接法   ch_*
 *   2. 开放定址法   oa_*
 *   3. Robin Hood  rh_*
 */

/* 默认哈希函数：key % capacity（结果非负） */
static inline int hash_default(int key, int capacity)
{
    if (capacity <= 0)
        return 0;
    int v = key % capacity;
    return v < 0 ? v + capacity : v;
}

/* ---------------- 分离链接法 ---------------- */
typedef struct CHHash CHHash;

CHHash* ch_create(int capacity);
void ch_free(CHHash *h);
void ch_insert(CHHash *h, int value);
int  ch_search(CHHash *h, int value);   /* 存在返回 1，否则 0 */
void ch_delete(CHHash *h, int value);

/* ---------------- 开放定址法 ---------------- */
typedef struct OAHash OAHash;

/* mode: 0 线性探测, 1 平方探测, 2 双散列 */
OAHash* oa_create(int capacity, int mode);
void oa_free(OAHash *h);
void oa_insert(OAHash *h, int value);
int  oa_search(OAHash *h, int value);   /* 返回下标或 -1 */
void oa_delete(OAHash *h, int value);

/* ---------------- Robin Hood 哈希 ---------------- */
typedef struct RHHash RHHash;

RHHash* rh_create(int capacity);
void rh_free(RHHash *h);
void rh_insert(RHHash *h, int value);
int  rh_search(RHHash *h, int value);   /* 返回下标或 -1 */
void rh_delete(RHHash *h, int value);

#endif /* HASH_H */
