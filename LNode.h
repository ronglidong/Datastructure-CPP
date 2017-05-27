#ifndef LINKNODE_H_
#define LINKNODE_H_
#include <iostream>
#include <stdexcept>

static const std::string notFound("Not found this element!");
static const std::string AllocateMemoryFailed("Allocate memory failed");
template <typename T> class LinkNode {
public:
	
	LinkNode(): length(0), listSize(0), data(0) { }
	LinkNode(const T &elem);
	LinkNode(const T &elem, unsigned num): length(0), listSize(0), data(0) { for (unsigned i = 0; i != num; ++i) Insert(elem, i); }
	// operator+ 将调用拷贝构造函数 
	LinkNode(const LinkNode<T> &l);
	~LinkNode() { Clear(); }
	void Insert(const T &elem, unsigned pos);
	void Delete(unsigned pos);
	void Clear();
	unsigned GetLength() const { return length; }
	unsigned GetListSize() const { return listSize; }
	void PushBack(const T &elem);
	T &Back() const { return Locate(length - 1); }
	T &operator[](unsigned pos) const { return Locate(pos)->data; }
	// 类外定义的友元不能使用私有成员
	LinkNode<T> &operator=(const LinkNode<T> &l);
	friend LinkNode<T> operator+(const LinkNode<T> &a, const LinkNode<T> &b)
	{
		LinkNode<T> total;
		unsigned la = a.GetLength();
		unsigned lb = la + b.GetLength();

		for (unsigned i = 0; i != la; ++i)
			total.Insert(a.Locate(i)->data, i);
		for (unsigned i = la, j = 0; i != lb; ++i, ++j)
			total.Insert(b.Locate(j)->data, i);
		std::cout << total;
		return total;
	}
	friend std::ostream &operator<<(std::ostream &os, const LinkNode<T> &l)
	{
		for (unsigned i = 0; i != l.length; ++i) {
			PNode temp = l.Locate(i);
			os << i << '/' << l.length << ":pointer:" << temp << "data:" << temp->data << "next:" << temp->next << ' ';
		}
		return os;
	}
	friend std::istream &operator>>(std::istream &is, LinkNode<T> &l) 
	{ 
		ValueType d; 
		is >> d; 
		if(is) 
			l.Insert(d, l.length); 
		return is;  
	}
private:
	typedef struct Node{
		T data;
		struct Node *next;
	}Node, *PNode;
	PNode data;
	unsigned length, listSize;
	PNode Locate(unsigned pos) const;
	void CheckPointer(PNode p) { if (!p) throw std::out_of_range(notFound); }
};

template <typename T>
LinkNode<T>::LinkNode(const T &elem)
{
	Insert(elem, 0);
}
template <typename T>
typename LinkNode<T>::PNode LinkNode<T>::Locate(unsigned pos) const
{
	PNode p = data;
	for (unsigned i = 0; p && i != pos; ++i, p = p->next)
		;
	return p;
}
template <typename T>
LinkNode<T>::LinkNode(const LinkNode<T> &l)
{
	length = listSize = 0;
	for (unsigned i = 0; i != l.length; ++i)
		Insert(l.Locate(i)->data, i);
}
template <typename T>
void LinkNode<T>::Insert(const T &elem, unsigned pos)
{
	PNode newElem = new Node();
	if (!newElem)
		throw std::runtime_error(AllocateMemoryFailed);
	newElem->data = elem;
	if (pos) {
		PNode p = Locate(pos - 1);
		CheckPointer(p);
		newElem->next = p->next;
		p->next = newElem;
	}
	else {
		data = newElem;
		data->next = 0;
	}
	++length;
	listSize += sizeof(Node);
}
template <typename T>
void LinkNode<T>::Delete(unsigned pos)
{
	PNode p;
	if (pos)
		p = Locate(pos - 1);
	data = p->next;
	delete(p);
	--length;
	listSize -= sizeof(Node);
}
template <typename T>
void LinkNode<T>::Clear()
{
	PNode p = data;
	for (unsigned i = 0; p; ++i) {
		PNode temp = p;
		p = p->next;
		delete(temp);
	}
	listSize = length = 0;
}
template <typename T>
void LinkNode<T>::PushBack(const T &elem)
{
	PNode p = new Node();
	if (length)
		Locate(length - 1)->next = p;
	else 
		data = p;
	p->data = elem;
	p->next = 0;
	++length;
	listSize += sizeof(Node);
}
template <typename T>
LinkNode<T> &LinkNode<T>::operator=(const LinkNode<T> &l)
{
	Clear();
	for (unsigned i = 0; i != l.length; ++i)
		Insert(l.Locate(i)->data, i);
	return *this;
}
#endif // LINKNODE_H_

