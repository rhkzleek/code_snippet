/**
 * @file BinaryTree.hpp
 * @author caogh (caoguanghuaplus@163.com)
 * @brief
 * @version 0.1
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __BINARYTREE_HPP__
#define __BINARYTREE_HPP__
template <typename Type>
class BinaryTree;

template <typename valType>
class BTnode
{
public:
    BTnode(const valType &val);

public:
    friend class BinaryTree;

private:
    void insert_val(const valType &);
    void remove_value(const valType &, BTnode *&);

private:
    valType _val;
    int _cnt;
    BTnode *_lchild;
    BTnode *_rchild;
};

template <typename elemType>
class BinaryTree
{
public:
    BinaryTree();
    BinaryTree(const BinaryTree &);
    ~BinaryTree();
    BinaryTree &operator=(const BinaryTree &);

    bool empty() const { return _root == nullptr; }
    void clear();

    void insert(const elemType &elem);

private:
    BTnode<elemType> *_root;

    void copy(BinaryTree<elemType> *tar, BinaryTree<elemType> *src);
};

#endif /* __BINARYTREE_HPP__ */