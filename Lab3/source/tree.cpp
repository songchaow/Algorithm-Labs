template <typename ElementT>
class BinSearchTree
{
  public:
    BinSearchTree() : head(nullptr){};
    typedef struct TreeNode
    {
        ElementT data;
        TreeNode *lchild;
        TreeNode *rchild;
        TreeNode *p;
        bool isred;
    } Node;
    
    Node *search(ElementT key)
    {
        return _search(key, head);
    }
    Node* insert(ElementT key)
    {
        auto node_ptr = new TreeNode;
        node_ptr->lchild = nullptr;
        node_ptr->rchild = nullptr;
        node_ptr->data = key;
        // locate
        Node *p = head;
        if (head == nullptr)
        {
            head = node_ptr;
            head->p = nullptr;
            return head;
        }
        while (true)
        {
            if (key <= p->data)
            {
                if (p->lchild)
                {
                    p = p->lchild;
                    continue;
                }
                else
                {
                    p->lchild = node_ptr;
                    node_ptr->p = p;
                    break;
                }
            }
            else
            {
                if (p->rchild)
                {
                    p = p->rchild;
                    continue;
                }
                else
                {
                    p->rchild = node_ptr;
                    node_ptr->p = p;
                    break;
                }
            }
        }
        return node_ptr;
    }
    void delete_(Node *p)
    {
        //  CASE I: there are no children in this node
        if (p->lchild == nullptr && p->rchild == nullptr)
        {
            if (p->p->rchild == p)
            {
                p->p->rchild = nullptr;
                delete p;
            }
            else if (p->p->lchild == p)
            {
                p->p->lchild = nullptr;
                delete p;
            }
        }
        //  CASE II: one of the two children is null
        else if (p->lchild == nullptr)
        {
            if (p->p->rchild == p)
            {
                p->p->rchild = p->rchild;
                p->rchild->p = p->p;
                delete p;
            }
            else if (p->p->lchild == p)
            {
                p->p->lchild = p->rchild;
                p->rchild->p = p->p;
                delete p;
            }
        }
        else if (p->rchild == nullptr)
        {
            if (p->p->rchild == p)
            {
                p->p->rchild = p->lchild;
                p->lchild->p = p->p;
                delete p;
            }
            else if (p->p->lchild == p)
            {
                p->p->lchild = p->lchild;
                p->lchild->p = p->p;
                delete p;
            }
        }
        else
        // CASE III: oops!! the most awful thing happens!
        {
            // first, we should find the successor of p
            auto p_succ = p->rchild;
            while (p_succ->lchild != nullptr)
                // travels to the left
                p_succ = p_succ->lchild;
            // there are three parts: p, p_succ, p_succ->right
            // 8 operations are needed
            // determine p's father
            if (p->p->lchild == p)
                p->p->lchild = p_succ;
            else if (p->p->rchild == p)
                p->p->rchild = p_succ;

            p_succ->lchild = p->lchild;
            p->lchild->p = p_succ;
            p_succ->p->lchild = p_succ->rchild;
            if (p_succ->rchild)
                p_succ->rchild->p = p_succ->p;
            p_succ->p = p->p;
            p_succ->rchild = p->rchild;
            p->rchild->p = p_succ;

            delete p;
        }
    }

  protected:
    // internal tree root

    Node *head;
    Node *_search(ElementT key, Node *root)
    {
        if (key == root->data)
            return root;
        if (key < root->data)
        {
            if (root->lchild)
                return _search(key, root->lchild);
            else
                return nullptr;
        }
        else
        {
            if (root->rchild)
                return _search(key, root->rchild);
            else
                return nullptr;
        }
    }
};

template <typename ElementT>
class RedBlackTree:BinSearchTree<ElementT>
{
private:
    void leftRotate(typename BinSearchTree<ElementT>::Node* x)
    {
        if(x->rchild == nullptr)
            return;
        
        auto new_root = x->rchild;
        if(BinSearchTree<ElementT>::head == x)
            BinSearchTree<ElementT>::head = new_root; // no need to change links to upper nodes
        else
        {
            if(x->p->lchild==x)
                x->p->lchild = new_root;
            else
                x->p->rchild = new_root;
        }
        new_root->p = x->p;
        
        x->rchild = new_root->lchild;
        if(new_root->lchild)
            new_root->lchild->p = x->rchild;

        x->p = new_root;
        new_root->lchild = x;
        
    }
    void rightRotate(typename BinSearchTree<ElementT>::Node* x)
    {
        if(x->lchild == nullptr)
            return;
        auto new_root = x->lchild;
        if(BinSearchTree<ElementT>::head == x)
            BinSearchTree<ElementT>::head = new_root; // no need to change links to upper nodes
        else
        {
            if(x->p->lchild==x)
                x->p->lchild = new_root;
            else
                x->p->rchild = new_root;
            new_root->p = x->p;
        }
        x->lchild = new_root->rchild;
        if(new_root->rchild)
            new_root->rchild->p = x->lchild;

        x->p = new_root;
        new_root->rchild = x;
    }
    
public:
    void insert(ElementT key)
    {
        auto node = BinSearchTree<ElementT>::insert(key);
        node->isred = true;
        if(node->p == nullptr) // it's root itself.
            {node->isred = false; return;}
        while(node->p && node->p->isred)
        // we can tell that node->p->p exists!
        {
            if(node->p == node->p->p->lchild)
            {
                auto y = node->p->p->rchild;
                if(y && y->isred)
                {
                    // CASE I
                    node->p->isred = false;
                    y->isred = false;
                    node->p->p->isred = true;
                    node = node->p->p;
                }
                else
                {
                    if(node == node->p->rchild)
                    {
                    // CASE II
                        node = node->p;
                        leftRotate(node);
                    }
                    // CASE III
                    node->p->isred = false;
                    node->p->p->isred = true;
                    rightRotate(node->p->p);
                }
            }
            else
            {
                auto y = node->p->p->lchild;
                if(y && y->isred)
                {
                    // CASE I
                    node->p->isred = false;
                    y->isred = false;
                    node->p->p->isred = true;
                    node = node->p->p;
                }
                else
                {
                    if(node == node->p->lchild)
                    {
                    // CASE II
                        node = node->p;
                        rightRotate(node);
                    }
                    // CASE III
                    node->p->isred = false;
                    node->p->p->isred = true;
                    leftRotate(node->p->p);

                }
            }
        }
        BinSearchTree<ElementT>::head->isred = false;
    }
    void delete_(typename BinSearchTree<ElementT>::Node* x)
    {
        ;
    }

};