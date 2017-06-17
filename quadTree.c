#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "quadTree.h"

/**
 * 插入元素
 * 1.判断是否已分裂，已分裂的选择适合的子结点，插入；
 * 2.未分裂的查看是否过载，过载的分裂结点，重新插入；
 * 3.未过载的直接添加
 *
 * @param node
 * @param ele
 */
void insertEle(struct QuadTreeNode *node, struct ElePoint ele) {
    if (1 == node->is_leaf) {
        if (node->ele_num + 1 > MAX_ELE_NUM) {
            splitNode(node);
            insertEle(node, ele);
        } else {
            struct ElePoint *ele_ptr = (struct ElePoint *) malloc(sizeof(struct ElePoint));
            ele_ptr->lat = ele.lat;
            ele_ptr->lng = ele.lng;
            strcpy(ele_ptr->desc, ele.desc);
            node->ele_list[node->ele_num] = ele_ptr;
            node->ele_num++;
        }

        return;
    }

    float mid_vertical = (node->region.up + node->region.bottom) / 2;
    float mid_horizontal = (node->region.left + node->region.right) / 2;
    if (ele.lat > mid_vertical) {
        if (ele.lng > mid_horizontal) {
            insertEle(node->RU, ele);
        } else {
            insertEle(node->LU, ele);
        }
    } else {
        if (ele.lng > mid_horizontal) {
            insertEle(node->RB, ele);
        } else {
            insertEle(node->LB, ele);
        }
    }
}

/**
 * 分裂结点
 * 1.通过父结点获取子结点的深度和范围
 * 2.生成四个结点，挂载到父结点下
 *
 * @param node
 */
void splitNode(struct QuadTreeNode *node) {
    float mid_vertical = (node->region.up + node->region.bottom) / 2;
    float mid_horizontal = (node->region.left + node->region.right) / 2;

    node->RU = createChildNode(node, mid_vertical, node->region.up, mid_horizontal, node->region.right);
    node->LU = createChildNode(node, mid_vertical, node->region.up, node->region.left, mid_horizontal);
    node->RU = createChildNode(node, node->region.bottom, mid_vertical, mid_horizontal, node->region.right);
    node->RU = createChildNode(node, node->region.bottom, mid_vertical, node->region.left, mid_horizontal);
    node->is_leaf = 0;
    for (int i = 0; i < node->ele_num; ++i) {
        insertEle(node, *node->ele_list[i]);
    }
}

struct QuadTreeNode *createChildNode(struct QuadTreeNode *node, float bottom, float up, float left, float right) {
    int depth = node->depth;
    struct QuadTreeNode *childNode = (struct QuadTreeNode *) malloc(sizeof(struct QuadTreeNode));
    struct Region *region = (struct Region *) malloc(sizeof(struct Region));
    initRegion(region, bottom, up, left, right);
    initNode(childNode, depth, *region);

    return childNode;
}

void deleteEle(struct QuadTreeNode *node, struct ElePoint ele) {
    /**
     * 1.遍历元素列表，删除对应元素
     * 2.检查兄弟象限元素总数，不超过最大量时组合兄弟象限
     */
}

void combineNode(struct QuadTreeNode *node) {
    /**
     * 遍历四个子象限的点，添加到象限点列表
     * 释放子象限的内存
     */
}

struct ElePoint *queryEle(struct QuadTreeNode *node, struct ElePoint ele) {
    if (node->is_leaf == 1) {
        return *node->ele_list;
    }

    float mid_vertical = (node->region.up + node->region.bottom) / 2;
    float mid_horizontal = (node->region.left + node->region.right) / 2;

    if (ele.lat > mid_vertical) {
        if (ele.lng > mid_horizontal) {
            return queryEle(node->RU, ele);
        } else {
            return queryEle(node->LU, ele);
        }
    } else {
        if (ele.lng > mid_horizontal) {
            return queryEle(node->RB, ele);
        } else {
            return queryEle(node->LB, ele);
        }
    }
}

void initNode(struct QuadTreeNode *node, int depth, struct Region region) {
    node->depth = depth;
    node->is_leaf = 1;
    node->ele_num = 0;
    node->region = region;
}

void initRegion(struct Region *region, float bottom, float up, float left, float right) {
    region->bottom = bottom;
    region->up = up;
    region->left = left;
    region->right = right;
}

int main() {
    struct QuadTreeNode root;
    struct Region root_region;

    struct ElePoint ele;
    initRegion(&root_region, -90, 90, -180, 180);

    int lng;
    int lat;
    char str[100];
    for (int i = 0; i < 5; i++) {
        ele.lng = rand()%180-rand()%360;
        ele.lat = rand()%90-rand()%180;
        strcpy(ele.desc, "hi");

        printf("%f,%f,%s", ele.lng,ele.lat,ele.desc);
        insertEle(&root,ele);
    }
}