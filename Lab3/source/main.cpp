template <typename ElementT>
class BinSearchTree
{
  public:
    Node *search(ElementT key)
    {
        return _search(key, head);
    }
    void insert(ElementT key)
    {
        auto node_ptr = new TreeNode;
        node_ptr->lchild = nullptr;
        node_ptr->rchild = nullptr;
        // locate
        Node *p = head;
        if (head == nullptr)
        {
            head = node_ptr;
            return;
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
            else if(p->p->lchild == p)
            {
                p->p->lchild = nullptr;
                delete p;
            }
        }
        //  CASE II: one of the two children is null
        else if(p->lchild==nullptr)
        {
            if(p->p->rchild == p)
            {
                p->p->rchild = p->rchild;
                p->rchild->p = p->p;
                delete p;
            }
            else if(p->p->lchild == p)
            {
                p->p->lchild = p->rchild;
                p->rchild->p = p->p;
                delete p;
            }
        }
        else if(p->rchild==nullptr)
        {
            if(p->p->rchild == p)
            {
                p->p->rchild = p->lchild;
                p->lchild->p = p->p;
                delete p;
            }
            else if(p->p->lchild == p)
            {
                p->p->lchild = p->lchild;
                p->lchild->p = p->p;
                delete p;
            }
        }
        else
        // oops!! the most awful thing happens!
        {
            
        }
    }

  protected:
    // internal tree root
    typedef struct TreeNode
    {
        ElementT data;
        TreeNode *lchild;
        TreeNode *rchild;
        TreeNode *p;
        bool isred;
    } Node;
    Node *head;
    Node *_search(ElementT key, Node *root)
    {
        if (key == root->data)
            return root;
        if (key < root->data)
        {
            if (root->lchild)
                return _search(key, root->lchild) else return nullptr;
        }
        else
        {
            if (root->rchild)
                return _search(key, root->rchild) else return nullptr;
        }
    }
};

template <typename ElementT>
class RedBlackTree
{
  public:
  private:
    BinSearchTree<ElementT> tree;
};