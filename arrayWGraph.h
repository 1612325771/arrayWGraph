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
{
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
	{
		if (i < 1 || j < 1 || i > n || j > n
			|| (aList[i].indexOf(wEdge<T>(j)) == -1 &&
				aList[j].indexOf(wEdge<T>(i)) == -1))
			return false;
		else
			return true;
	}

	void insertEdge(edge<T> *theWeightEdge)
	{
		int v1 = theWeightEdge->vertex1();
		int v2 = theWeightEdge->vertex2();
		if (v1 < 1 || v2 < 1 || v1 > n || v2 > n || v1 == v2)
		{
			ostringstream s;
			s << "(" << v1 << "," << v2
				<< ") ����һ���Ϸ���";
			throw illegalParameterValue(s.str());
		}

		int index1 = aList[v1].indexOf(wEdge<T>(v2));
		int index2 = aList[v2].indexOf(wEdge<T>(v1));

		if (index1 == -1 && index2 == -1)
		{
			aList[v1].insert(0, wEdge<T>(v2, theWeightEdge->weight()));
			aList[v2].insert(0, wEdge<T>(v1, theWeightEdge->weight()));
			e++;
		}

		else
		{
			aList[v1].setElement(index1, wEdge<T>(v2, theWeightEdge->weight()));
			aList[v2].setElement(index2, wEdge<T>(v1, theWeightEdge->weight()));
		}

	}

	void eraseEdge(int i, int j)
	{
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
	{
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
			s << "û�ж��㣺 " << theVertex;
			throw illegalParameterValue(s.str());
		}
	}

	bool directed() const { return false; }

	bool weighted() const { return true; }

	typedef typename arrayList<wEdge<T> >::iterator vit;

	class myIterator : public vertexIterator<int>
	{
	public:
		myIterator(vit theBegin,vit theEnd)
		{
			it = theBegin;
			end = theEnd;
		}

		~myIterator() {}

		int next()
		{
			if (it++ != end)
			{
				return it->vertex;
			}
			return 0;
		}

		int next(T& theWeight)
		{
			if (it++ != end)
			{
				theWeight = it->weight;
				return it->vertex;
			}
			return 0;
		}

	protected:
		vit it;
		vit end;
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
	{
		ofstream out(file);
		if (out.is_open())
		{
			output(out);
			out.close();
		}
	}

	void init(string& file) 
	{
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

template <class T>
ofstream& operator<<(ofstream& out, const arrayWGraph<T>& x)
{
	x.output(out); return out;
}
#endif