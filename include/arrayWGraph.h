/*
==========================================================================
Author: BeingGod
License: Apache-2.0 License
Description: 邻接数组实现的加权无向图
==========================================================================
*/

#ifndef arrayWGraph_
#define arrayWGraph_

#include <iostream>
#include <fstream>
#include "graph.h"
#include "myExceptions.h"
#include "arrayListWithIterator.h"
#include "weightedEdge.h"


//**************************************************************************
template <class T>
struct wEdge
{
	int vertex;
	T weight;
	wEdge(int theVertex = 0, T theWeight = 0)
	{
		vertex = theVertex; weight = theWeight;
	}
	operator int() const { return vertex; }
};

template <class T>
ostream& operator<<(ostream& out, const wEdge<T>& x)
{
	out << x.vertex << " " << x.weight << " "; return out;
}
//**************************************************************************


template<class T>
class arrayWGraph :public graph<T>
{// 有权无向图类
protected:
	int n;
	int e;
	arrayList<wEdge<T> > *aList;

public:
	arrayWGraph(int v = 0)
	{
		
		if (v < 0)
			throw illegalParameterValue("number of vertices must be >= 0");
		n = v;
		e = 0;

		aList = new arrayList<wEdge<T> > [n + 1];
	}

	~arrayWGraph() { delete[] aList; }

	int numberOfVertices() const { return n; };

	int numberOfEdges() const { return e; };

	bool existsEdge(int i, int j) const
	{// 检测边是否存在
		if (i < 1 || j < 1 || i > n || j > n
			|| (aList[i].indexOf(wEdge<T>(j)) == -1 &&
				aList[j].indexOf(wEdge<T>(i)) == -1))
			return false;
		else
			return true;
	}

	void insertEdge(edge<T> *theWeightEdge)
	{// 插入一条边
		int v1 = theWeightEdge->vertex1();
		int v2 = theWeightEdge->vertex2();
		if (v1 < 1 || v2 < 1 || v1 > n || v2 > n || v1 == v2)
		{
			ostringstream s;
			s << "(" << v1 << "," << v2
				<< ") is a illegal edge";
			throw illegalParameterValue(s.str());
		}

		int index1 = aList[v1].indexOf(wEdge<T>(v2));
		int index2 = aList[v2].indexOf(wEdge<T>(v1));

		if (index1 == -1 && index2 == -1)
		{// 该边不存在->添加
			aList[v1].insert(0, wEdge<T>(v2, theWeightEdge->weight()));
			aList[v2].insert(0, wEdge<T>(v1, theWeightEdge->weight()));
			e++;
		}

		else
		{// 该边存在->更新权值
			aList[v1].setElement(index1, wEdge<T>(v2, theWeightEdge->weight()));
			aList[v2].setElement(index2, wEdge<T>(v1, theWeightEdge->weight()));
		}

	}

	void eraseEdge(int i, int j)
	{// 删除一条边
		if (i >= 1 && j >= 1 && i <= n && j <= n )
		{
			if (aList[i].indexOf(j) != -1)
			{
				wEdge<T> v1 = aList[i].eraseElement(j);
				wEdge<T> v2 = aList[j].eraseElement(i);
				if (v1 != NULL)
					e--;
			}
		}
	}

	int degree(int theVertex) const
	{// 计算顶点的度
	// 无向图特殊方法
		checkVertex(theVertex);
		return aList[theVertex].size();
	}

	int inDegree(int theVertex) const { throw undefinedMethod("inDegree() undefined"); }
		
	int outDegree(int theVertex) const { throw undefinedMethod("outDegree() undefined"); }

	void checkVertex(int theVertex) const
	{
		if (theVertex < 1 || theVertex > n)
		{
			ostringstream s;
			s << "Illegal  the vertex: " << theVertex;
			throw illegalParameterValue(s.str());
		}
	}

	bool directed() const { return false; }

	bool weighted() const { return true; }

	typedef typename arrayList<wEdge<T> >::iterator vit;

	class myIterator : public vertexIterator<int>
	{// 对arrayList迭代器进行封装
	public:
		myIterator(vit theBegin,vit theEnd)
		{
			it = theBegin;
			end = theEnd;
		}

		~myIterator() {}

		int next()
		{// 返回下一个顶点
			if (it++ != end)
			{
				return it->vertex;
			}
			return 0;
		}

		int next(T& theWeight)
		{// 返回下一个顶点
		// 并将 theWeight 设置为边的权值
			if (it++ != end)
			{
				theWeight = it->weight;
				return it->vertex;
			}
			return 0;
		}

	protected:
		vit it;
		vit end; // 尾后迭代器
	};

	myIterator* iterator(int theVertex)
	{
		checkVertex(theVertex);
		return new myIterator(--aList[theVertex].begin(),aList[theVertex].end());
	}

	void output(ostream& out) const
	{
		out << "v w" << endl;
		for (int i = 1; i <= n; i++)
			out << aList[i] << endl;
	}

	void save(string& file) 
	{// 将图保存到文件
		ofstream out(file);
		if (out.is_open())
		{
			output(out);
			out.close();
		}
	}

	void init(string& file) 
	{// 从文件中初始化图
		ifstream in(file);
		if (in.is_open())
		{
			in >> n;
			arrayList<wEdge<T> >* temp = new arrayList<wEdge<T> >[n+1];
			delete[] aList;
			aList = temp;
			
			while (in)
			{
				int v1, v2;
				T w;
				in >> v1 >> v2 >> w;
				insertEdge(new weightedEdge<T>(v1, v2, w));
			}
			in.close();
		}
	}
};

// overload <<
template <class T>
ostream& operator<<(ostream& out, const arrayWGraph<T>& x)
{
	x.output(out); return out;
}

// 重载 << 将图输入到文件流
template <class T>
ofstream& operator<<(ofstream& out, const arrayWGraph<T>& x)
{
	x.output(out); return out;
}
#endif