#pragma once
#include <algorithm>//ʹ��STL�е�swap()��max()
using namespace std;

template <typename K>
class RedBlack
{
	enum RBColor { RED, BLACK };
    struct RBNode {
		K key;
        int height;
        RBColor color; //Ĭ���½ڵ���ɫΪRED
        RBNode *parent, *left, *right;
        RBNode() : parent(nullptr), left(nullptr), right(nullptr), height(0), color(RED) {}
        RBNode(K key, RBNode *parent = nullptr, RBNode *left = nullptr, RBNode *right = nullptr, int height = 0, RBColor color = RED) : key(key), parent(parent), left(left), right(right), height(height), color(color) {}
        RBNode *uncle(); //���ص�ǰ�ڵ㸸�׵ĸ��׵���һ������
        RBNode *succ();  //���ص�ǰ�ڵ��ֱ�Ӻ��,�����ڷ���null
    };

public:
	struct iterator	{//���������
		RBNode* pNode;
		iterator(RBNode* p = nullptr) :pNode(p) {}
		iterator& operator=(iterator& x) { pNode = x->pNode; return *this; }
		K& operator*() { return pNode->key; }
		K* operator->() { return &pNode->key; }
		iterator& operator++() { iterator old = *this; this->pNode = this->pNode->succ(); return old; }
		iterator& operator++(int) { this->pNode = this->pNode->succ(); return *this; }
		bool operator==(iterator& x) { return pNode == x.pNode; }
		bool operator!=(iterator& x) { return pNode != x.pNode; }
	};

    RedBlack() : m_root(nullptr), m_hot(nullptr), m_size(0) {}
	~RedBlack() { clear(); }

	int size() const { return m_size; }
	void clear() { while (m_size > 0) removeAt(m_root); }

	bool insert(const K &key);		//�����в���elem,����ɹ�����true;��elem�Ѵ���,����false
    bool erase(const K &key);     //������ɾ��elem,ɾ���ɹ�����true;��elem������,����false

	iterator begin() const { RBNode* p = m_root; if (p) while (p->left) p = p->left; return iterator(p); }
	iterator end() const { return iterator(); }
	iterator find(const K& key) const { return iterator(search(key)); }
	bool erase(iterator& it) { if (it.pNode) { erase(it.pNode->key); return true; } return false; }


private:
	int m_size;
	RBNode* m_root;
	mutable RBNode* m_hot;
	RBNode*& search(const K& key) const; //����Ԫ��ʱ,���ظ�Ԫ�صĸ��ڵ�ָ���Ԫ�ص�ָ�������
									//Ԫ�ز�����ʱ,���ظ��ڵ�ָ���nullԪ�ص�ָ�������(�൱�� m_hot->left �� m_hot->right)
									//(��Ϊ��ʱ,����m_root������,m_hotΪnull)
									//ͬʱ����m_hotΪ�ý��ĸ��ڵ�

	RBNode* removeAt(RBNode*& rx);       //����:������ɾ��rxָ��Ľڵ�
												//����:ȡ��rx�Ľڵ�ָ��
												//������:����m_hotΪ�ý��ĸ��ڵ�

	int updateHeight(RBNode* x); //���½ڵ�ĺڸ߶�
								 //��ʽ:�ڸ߶�=max(���ҽڵ�ڸ߶�)+(��ǰΪ�ڽڵ�)
	RBNode* zig(RBNode* x);
	RBNode* zag(RBNode* x);
	void solveRed(RBNode* x);   //�޸�˫��ȱ��
	void solveBlack(RBNode* x); //�޸�˫��ȱ��
};

template <typename K>
typename RedBlack<K>::RBNode *RedBlack<K>::RBNode::succ()
{
    RBNode *s = this;
    if (right) { //�ҵ������������Ľڵ�
        s = right;
        while (s->left)
            s = s->left;
    } else {
        while (s->parent && s->parent->right && s->parent->right == s)
            s = s->parent; //�����������Ϸ��ƶ�
        s = s->parent;     //�����Ϸ��ƶ�
    }
    return s;
}

template <typename K>
typename RedBlack<K>::RBNode *RedBlack<K>::RBNode::uncle()
{
    RBNode *s = this;
    RBNode *p = s->parent;
    if (!p) return nullptr;
    RBNode *g = p->parent;
    if (!g) return nullptr;
    RBNode *gl = g->left, *gr = g->right;
    return (p == gl) ? gr : gl;
}

#define stature(x) ((!(x)) ? (0) : ((x)->height))

template <typename K>
int RedBlack<K>::updateHeight(RBNode *x)
{
    x->height = max(stature(x->left), stature(x->right)); //�ⲿ�ڵ�=0
    return (x->color == BLACK ? ++x->height : x->height); //�ڽڵ�+1
}

template <typename K> 
typename RedBlack<K>::RBNode *RedBlack<K>::zig(RedBlack<K>::RBNode *x) {
    /* assert: (x != nullptr) && (x->left != nullptr) */
    RBNode* lc = x->left, *lcrc = lc->right, *p = x->parent;
	RBNode*& rpx = (p == nullptr ? m_root : (p->left == x ? p->left : p->right));
    x->left = lcrc; 
    if( lcrc ) lcrc->parent = x;
    updateHeight(x); 
    lc->right = x;
    x->parent = lc;
    updateHeight(lc);
    lc->parent = p;
    rpx = lc; 
    return lc;
}

template <typename K> 
typename RedBlack<K>::RBNode *RedBlack<K>::zag(RedBlack<K>::RBNode *x) {
    /* assert: (x != nullptr) && (x->right != nullptr) */
    RBNode* rc = x->right, *rclc = rc->left, *p = x->parent;
	RBNode*& rpx = (p == nullptr ? m_root : (p->left == x ? p->left : p->right));
    x->right = rclc; 
    if( rclc ) rclc->parent = x; 
    updateHeight(x);
    rc->left = x; 
    x->parent = rc; 
    updateHeight(rc);
    rc->parent = p; 
    rpx = rc; 
    return rc;
}

template <typename K>
typename RedBlack<K>::RBNode *&RedBlack<K>::search(const K &rKey) const{
	if (!m_root || (!(m_root->key < rKey) && !(rKey < m_root->key))) { m_hot = nullptr; return const_cast<RBNode *&>(m_root); }	//���������˻����
	m_hot = m_root;
    for (;;){
        RBNode *&rpx = (rKey < m_hot->key) ? m_hot->left : m_hot->right;	//�˴�һ��Ҫ���¶���rp
        if (!rpx || (!(rKey < rpx->key) && !(rpx->key < rKey))) return rpx; //����Ԫ��ʱ,���ظ�Ԫ�صĸ��ڵ�ָ���Ԫ�ص�ָ�������
                                                //����ʧ��ʱ(!rpx),���ظ��ڵ�ָ���Ԫ��(null)��ָ�������(�൱�� m_hot->left �� m_hot->right)
                                                //(��Ϊ��ʱ,����m_root������,m_hotΪnull)
        m_hot = rpx;
    }
}

template <typename K>
bool RedBlack<K>::insert(const K &key)
{
    RedBlack<K>::RBNode *&x = search(key);
    if (x) return false;
    x = new RedBlack<K>::RBNode(key, m_hot, nullptr, nullptr, 0); //(����һ�������ⲿ�ڵ�)
    m_size++;
    solveRed(x);
    return true;
}

template <typename K>
typename RedBlack<K>::RBNode *RedBlack<K>::removeAt(RedBlack<K>::RBNode *&rpx) {  //rxΪ���ñز�Ϊ��
    RBNode *x = rpx, *succ = nullptr, *p = x->parent;
    if (!x->left)       succ = rpx = x->right;//��������,��������(����Ϊnull)ȡ����֮,(rxΪ����,�����������ӽڵ�)
    else if (!x->right) succ = rpx = x->left; //��������,��������ȡ����֮,(rxΪ����,�����������ӽڵ�)
    else {                          
        succ = x->succ();           //��˫����,���к��
        swap(x->key, succ->key);  //����x��x�ĺ��,ת��Ϊ�������������
        x = succ;
        p = succ->parent;
        succ = succ->right; 
        (p == rpx ? p->right : p->left) = succ;  //��Ϊxλ���Ѿ��仯,���ֶ������ӽڵ�
    }
    if (succ) succ->parent = p;
	delete x; m_size--;
    m_hot = p;          //����m_hotΪɾȥ�ڵ�ĸ���
    return succ;
}

template <typename K>
bool RedBlack<K>::erase(const K &key)
{
    RBNode *&rx = search(key);
    if (!rx) return false;
    RBNode *r = removeAt(rx);   //rΪָ��ȡ��xλ�õĽڵ��ָ��,����Ϊnull
    if (m_size == 0) return true; //ɾ���������һ���ڵ�(˵������ֻ�и��ڵ�)
    if (m_hot == nullptr) {     //ɾ�����Ǹ��ڵ�(˵�����н���root��һ�����ӽڵ�),ֱ�ӽ�rȾ�ɺ�ɫ
        m_root->color = BLACK;
        updateHeight(m_root);
        return true;
    }
    if ( (stature(m_hot->left) == stature(m_hot->right)) &&
        (m_hot->height == (m_hot->color == BLACK ? stature(m_hot->left) + 1 : stature(m_hot->left)))) {
        //m_hot�ĺڸ߶�û�з�������,���������ƽ��
        return true;
    }
    if (r && r->color == RED){  //��rΪ��ɫ(��xΪ��ɫ),��Υ�����������,��rȾ�ɺ�ɫ����
        r->color = BLACK;
        r->height++;
        return true;
    }
    /* assert: rһ��Ϊ��ɫ(��Ϊnull), m_hotָ�򸸽ڵ�(�ز�Ϊnull) */
    solveBlack(r);
    return true;
}

template <typename K>
void RedBlack<K>::solveRed(RedBlack<K>::RBNode *x)
{
    /* 1.���뷢�����ڸ��ڵ�
     *  ֱ��Ⱦ�ɺ�ɫ */
    if (x->parent == nullptr) {/*x == m_root*/ 
        x->color = BLACK;
        x->height++;
        return;
    }
    /* 2.����ڵ�ĸ��ڵ�Ϊ��ɫ,��Υ����������� */
    RBNode *p = x->parent;
    if (p->color == BLACK) 
		return;
    /* 3.����ڵ�ĸ��ڵ�Ϊ��ɫ
     *   ������ڵ��uncle�ڵ���ɫ */
    RBNode *g = p->parent;
    RBNode *u = x->uncle();

    if (!u || u->color == BLACK) {/* 3.1 ����ڵ��uncle�ڵ���ɫΪ��(��Ϊ�ⲿ�ڵ�),���е��� */
        if (g->left == p) {	//�����ȵ�����ɫ�ٵ������˽ṹ,����߶ȸ��»������
            if (p->left == x) {
                p->color = BLACK;
                g->color = RED;
                zig(g);
            } else { 
                x->color = BLACK;
                p->color = g->color = RED;
                zag(p);
                zig(g);
            }
        } else {
            if (p->right == x) { 
                p->color = BLACK;
                g->color = RED;
                zag(g);
            } else { 
                x->color = BLACK;
                p->color = g->color = RED;
                zig(p);
                zag(g);
            }
        }
    } else {/* 3.2 ����ڵ��uncle�ڵ���ɫΪ�� */
		p->color = BLACK; p->height++;
		u->color = BLACK; u->height++;
        g->color = RED;
        solveRed(g);
    }
}

template <typename K> void RedBlack<K>::solveBlack(RBNode *r){
    /* assert: rһ��Ϊ��ɫ(��Ϊnull), m_hotָ�򸸽ڵ�(�ز�Ϊnull) */
    RBNode *p = (r ? r->parent : m_hot);    //1.2.2 ����ݹ����ʱ����ʹ��m_hot
    if (!p) return; //ɾȥ���Ǹ��ڵ�,����ֻʣxһ���ڵ�,����Ҫ����
    RBNode *s = (p->left == r) ? p->right : p->left;    //sibiling
    if (s->color == BLACK) { // 1.��ɾȥ��x���ֵܽڵ�(�ض�����)Ϊ�ڽڵ�
        RBNode *t = nullptr;    //Ѱ��s�ĺ캢��
        if (s->left && s->left->color == RED) t = s->left;
        if (s->right && s->right->color == RED) t = s->right;
        if (t) {    /* 1.1 ��ɾȥ��x���ֵܽڵ��к캢��*/
            RBColor pOld = p->color;
            RBNode *mid = nullptr;
            if (p->left == s) { //p,s,t����3+4�ع�,���ҽڵ���,�м�ڵ㱣��ԭ��p����ɫ
                if (s->left == t) {
                    zig(p);
                    mid = s;
                } else {
                    zag(s);
                    zig(p);
                    mid = t;
                }
            } else {
                if (s->right == t) {
                    zag(p);
                    mid = s;
                } else {
                    zig(s);
                    zag(p);
                    mid = t;
                }
            }
            if (mid->left) mid->left->color = BLACK;
            if (mid->right) mid->right->color = BLACK;
            mid->color = pOld;
        } else {    /* 1.2 ��ɾȥ��x���ֵܽڵ�û�к캢��*/
            s->color = RED; //s��ɺ�ɫ,��x��ɾ�������
            s->height--;
            if (p && p->color == RED) p->color = BLACK; //1.2.1 pΪ��ɫ->p��ɺ�ɫ,�޸����
            else {  //1.2.2 pΪ��ɫ->�ڸ߶�������ϴ���
                p->height--;
                solveBlack(p);
            }
        }
    } else {    /* 2.��ɾȥ��x���ֵܽڵ�Ϊ��ڵ�,����s��p��λ�ú���ɫ */
        if (p->left == s) zig(p); else zag(p);
        s->color = BLACK;
        p->color = RED; 
        solveBlack(r); //�ٴε���solveBlack,��ʱsibiling�ڵ��Ϊ��ɫ
    }
}
