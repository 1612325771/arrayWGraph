#include "arrayWGraph.h"
#include "weightedEdge.h"
#include <iostream>

#define OUTPUT_PATH "./output.dat"

using namespace std;

int main(int argc, char *argv[])
{
	string outputPath = OUTPUT_PATH;
	if (argc != 2)
	{
		cout << "Useage: run <dat>" << endl;
		return 0;
	}

	string inputPath = argv[1];

	arrayWGraph<int> g(0);
	g.init(inputPath);

	cout << "The number of vertex = " << g.numberOfVertices() << endl;
	cout << "The number of edge = " << g.numberOfEdges() << endl;
	cout << endl;


	if (g.cycle()) cout << "Is cycled" << endl;
	else cout << "No cycled" << endl;

	if (g.connected()) cout << "Is connected" << endl;
	else cout << "No connected" << endl;

        /*
	vertexIterator<int>* gi = g.iterator(4);
	int vertex,weight;
	while ((vertex = gi->next(weight)) != 0)
		cout << vertex << " " << weight << endl;
	cout << endl;
        */

	g.save(outputPath);

	return 1;
}
