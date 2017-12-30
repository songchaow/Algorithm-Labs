template <typename ElementT>
class BinSearchTree
{
  public:
    Node *search(ElementT key);
    void insert(ElementT key);

    void delete_(Node *p);

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
    Node *_search(ElementT key, Node *root);
};

template <typename ElementT>
class RedBlackTree
{
  public:
    void insert(ElementT key);

  private:
    BinSearchTree<ElementT> tree;
};