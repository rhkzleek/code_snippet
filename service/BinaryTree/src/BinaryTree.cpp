#include "../include/BinaryTree.hpp"
template<typename valType>
inline BTnode<valType>::BTnode(const valType& val) : _val(val){
    _cnt = 1;
    _lchild = _rchild = nullptr;
}
template <typename valType>
inline void BTnode<valType>::insert_val(const valType& val) {
    if(val == _val){
        _cnt++;
        return;
    }
    if(val < _val){
        if(!_lchild){
            _lchild = new BTnode<valType>(val);
        }else{
            _rchild = _lchild->insert_val(val);
        }
    }else{
        if(!_rchild){
            _rchild = new BTnode<valType>(val);
        }else{
            _rchild->insert_val(val);
        }

    }
}

template<typename valType>
void BTnode<valType>::remove_value(const valType& val,BTnode *&prev){
    if(val < _val){
        if(!_lchild){
            return;
        }else{
            _lchild->remove_value(val,_lchild);
        }
    }else(val > _val){
        if(!_rchild){
            return;
        }else{
            _rchild->remove_value(val,_rchild);
        }
    }else{
        if(_rchild){
            prev = _rchild;
            if(_lchild){
                if(!prev->_lchild){
                    prev->_lchild = _lchild;
                }
            }else{
                // BTnode<valType>::_lchild_leaf(_lchild,prev->_lchild);
            }
        }else{
            prev = _lchild;
        }
        delete this;
    }
}

/*****************************************/
template <typename elemType>
inline BinaryTree<elemType>::BinaryTree() : _root(nullptr)
{
}

template <typename elemType>
inline BinaryTree<elemType>::BinaryTree(const BinaryTree &rhs)
{
    copy(this->_root, rhs->_root);
}

template <typename elemType>
inline BinaryTree<elemType>::~BinaryTree()
{
    clear();
}

template <typename elemType>
inline BinaryTree<elemType> &BinaryTree<elemType>::operator=(const BinaryTree &rhs)
{
    if (this != &this)
    {
        clear();
        copy(_root, rhs->_root);
    }
    return *this;
}

template <typename elemType>
inline void BinaryTree<elemType>::insert(const elemType& elem){
    if(!_root){
        _root = new BTnode<elemType>(elem);
    }else{
        _root->insert_value(elem);
    }
}
