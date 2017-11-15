#include <iostream>
using namespace std;
enum BFactor { LH = -1, EH = 0, RH = 1 };
template <class T>
struct AVLNode {
	T data;
	BFactor b;
	AVLNode<T> *pLeft;
	AVLNode<T> *pRight;
	AVLNode(T &a) : data(a), b(EH), pLeft(NULL), pRight(NULL) {}
	AVLNode(T &a, AVLNode *pL, AVLNode *pR) : data(a), b(EH), pLeft(pL), pRight(pR) {}
};
template <class T>
using AVLTree = AVLNode<T>*;

template <class T>
void rotLeft(AVLTree<T> &pR) {
	if (pR == NULL || pR->pRight == NULL)
		return;
	AVLNode<T> *p = pR->pRight;
	pR->pRight = p->pLeft;
	p->pLeft = pR;
	switch (p->b)
	{
	case EH:
		pR->b = RH;
		p->b = EH;
		break;
	case RH:
		p->b = EH;
		pR->b = EH;
		break;
	}
	pR = p;
}

template <class T>
void rotRight(AVLTree<T> &pR) {
	if (pR == NULL || pR->pLeft == NULL)
		return;
	AVLNode<T> *p = pR->pLeft;
	pR->pLeft = p->pRight;
	p->pRight = pR;
	switch (p->b)
	{
	case LH:
		pR->b = EH;
		p->b = EH;
		break;
	case EH:
		p->b = RH;
		pR->b = LH;
		break;
	}
	pR = p;
}

template <class T>
void rotRightLeft(AVLTree<T> &pR) {
	if (pR == NULL || pR->pLeft == NULL)
		return;
	/*rotLeft(pR->pLeft);
	rotRight(pR);*/
	AVLNode<T> *p1, *p2;

	p1 = pR->pLeft;
	p2 = p1->pRight;
	pR->pLeft = p2->pRight;
	p1->pRight = p2->pLeft;
	p2->pRight = pR;
	p2->pLeft = p1;

	switch (p2->b)
	{
	case LH:
		p1->b = EH;
		pR->b = RH;
		break;
	case EH:
		pR->b = EH;
		p1->b = EH;
		break;
	case RH:
		pR->b = EH;
		p1->b = LH;
		break;
	}
	p2->b = EH;
	pR = p2;
}

template <class T>
void rotLeftRight(AVLTree<T> &pR) {
	if (pR == NULL || pR->pRight == NULL)
		return;
	/*rotRight(pR->pRight);
	rotLeft(pR);*/
	AVLNode<T> *p1, *p2;
	p1 = pR->pRight;
	p2 = p1->pLeft;
	pR->pRight = p2->pLeft;
	p1->pLeft = p2->pRight;
	p2->pLeft = pR;
	p2->pRight = p1;
	switch (p2->b)
	{
	case LH:
		pR->b = EH;
		p1->b = RH;
		break;
	case EH:
		pR->b = EH;
		p1->b = EH;
		break;
	case RH:
		pR->b = LH;
		p1->b = EH;
		break;
	}
	p2->b = EH;
	pR = p2;
}


#pragma region Balance Left And Right

template <class T>
bool balanceleft(AVLNode<T>*&root) {
	if (root->pLeft->b == LH) {//L-L
		rotRight(root);
		root->b = root->pRight->b = EH;
		return false;
	}

	if (root->pLeft->b == EH) {//L-E
		rotRight(root);
		root->b = RH;
		root->pLeft->b = LH;
		return true;
	}
	if (root->pLeft->b == RH) {//L-R
		rotLeftRight(root);
		if (root->b == LH) {
			root->pLeft->b = EH;
			root->pRight->b = RH;
			root->b = EH;
			return false;
		}
		else if (root->b == EH) {
			root->pLeft->b = root->pRight->b = EH;
			root->b = EH;
			return false;
		}
		root->b = root->pRight->b = EH;
		root->pLeft->b = LH;
		return false;
	}
}

template <class T>
bool balanceright(AVLNode<T>*&root) {
	if (root->pRight->b == RH) {//R-R
		rotLeft(root);
		root->b = root->pLeft->b = EH;
		return false;
	}

	if (root->pRight->b == EH) {//R-E
		rotLeft(root);
		root->b = LH;
		root->pLeft->b = RH;
		return true;
	}
	if (root->pRight->b == LH) {//R-L
		rotRightLeft(root);
		if (root->b == LH) {
			root->b = root->pRight->b = EH;
			root->pLeft->b = LH;
			return false;
		}
		else if (root->b == EH) {
			root->b = root->pLeft->b = root->pRight->b = EH;
			return false;
		}
		root->b = root->pLeft->b = EH;
		root->pRight->b = RH;
		return false;
	}
}

#pragma endregion

#pragma region InsertNode

template <class T>
bool InsertNode(AVLTree<T> &pR, T a) {
	if (pR == NULL) {
		pR = new AVLNode<T>(a);
		return true;
	}
	if (a < pR->data) {
		if (InsertNode(pR->pLeft, a) == false)return false;
		if (pR->b == EH) {
			pR->b = LH;
			return true;
		}
		if (pR->b == RH) {
			pR->b = EH;
			return false;
		}
		return balanceleft(pR);
		if (pR->pLeft->b == LH) {//L-L
			rotRight(pR);
			pR->b = EH;
			pR->pRight->b = EH;
			return false;
		}
		if (pR->pLeft->b = RH) {//L-R
			rotLeftRight(pR);
			pR->pLeft->b = pR->pRight->b = EH;
			return false;
		}
		/*if (root->balance == -1){
		root->balance = root->left->balance = 0;
		return false;
		}
		if (root->balance == 1){
		root->balance = root->right->balance = 0;
		root->left->balance = -1;
		return false;
		}*/
	}
	if (a >= pR->data) {
		if (InsertNode(pR->pRight, a) == false)return false;
		if (pR->b == EH) {
			pR->b = RH;
			return true;
		}
		if (pR->b == LH) {
			pR->b = EH;
			return false;
		}
		return balanceright(pR);
		//if (root->right->balance==1){//R-R
		//	rotleft(root);
		//	root->balance = root->left->balance = 0;
		//	return false;
		//}
		//if (root->right->balance = -1){//R-L
		//	rotRL(root);
		//	root->left->balance = root->right->balance = 0;
		//	return false;
		//}
		/*if (root->balance = 1){
		root->balance = root->right->balance = 0;
		return false;
		}
		if (root->balance = -1){
		root->balance = root->left->balance = 0;
		root->right->balance = 1;
		return false;
		}*/
	}
}

#pragma endregion

#pragma region Remove
template <class T>
bool remove(T a, AVLTree<T> &pR) {
	if (pR == NULL)
		return false;
	if (a < pR->data) {
		if (remove(a, pR->pLeft)) {
			if (pR->b == EH) { pR->b = RH; return false; }
			if (pR->b == LH) { pR->b = EH; return false; }
			else return !balanceright(pR);
		}
		return false;
	}
	if (a > pR->data) {
		if (remove(a, pR->pRight)) {
			if (pR->b == EH) { pR->b = LH; return false; }
			if (pR->b == RH) { pR->b = EH; return false; }
			else return !balanceleft(pR);
		}
		return false;
	}
	if (pR->pLeft == NULL && pR->pRight == NULL) {
		delete pR;
		pR = NULL;
		return true;
	}
	if (pR->pLeft == NULL) {
		AVLNode<T> *p = pR;
		pR = pR->pRight;
		delete p;
		return true;
	}
	if (pR->pRight == NULL) {
		AVLNode<T> *p = pR;
		pR = pR->pLeft;
		delete p;
		return true;
	}
	AVLNode<T> *p = pR->pRight;
	while (p->pLeft)
		p = p->pLeft;
	pR->data = p->data;
	if (remove(p->data, pR->pRight)) {
		if (pR->b == EH) { pR->b = LH; return false; }
		if (pR->b == RH) { pR->b = EH; return false; }
		else return !balanceleft(pR);
	}
	return false;
}

#pragma endregion






int main() {
	int dataRoot = 4;
	//AVLNode<int> *pR = new AVLNode<int>(dataRoot); // this line  similar line below
	AVLTree<int> root = new AVLNode<int>(dataRoot);
	InsertNode(root, 8);
	InsertNode(root, 19);
	InsertNode(root, 21);
	InsertNode(root, 1);
	InsertNode(root, 6);
	remove(8, root);
	cout << "done";
	return 0;
}
