#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Point Point;
struct Point
{
	int x; // x point
	int y; // y point

};

Point myLocation; //your location, what point we are on.

typedef struct Points Points;
struct Points
{
	Point* sick; // list of locations where sick ppl are.
	unsigned nSick;// number of sick ppl.
	Point* queries; // queries we are supposed to make
	unsigned nQueries; // number of queries we are supposed to make
	unsigned threshold; // theshold between insertion and merge sorts.
};

Points loadPoints()
{
	FILE* in = fopen("assignment3input.txt", "r");
	if (in == NULL)
	{
		fprintf(stderr, "project13: Unable to open assignment3input.txt\n");
		perror("project13");
		exit(1);
	}
	Points p = { NULL, 0, NULL, 0, 0 };
	if (fscanf(in, "%d %d %u %u %u", &myLocation.x, &myLocation.y, &p.nSick, &p.nQueries, &p.threshold) != 5)
	{
		fprintf(stderr, "project13: Unable to read first line of data\n");
		exit(1);
	}
	p.sick = (Point*)malloc(sizeof(Point) * p.nSick);
	for (unsigned i = 0; i < p.nSick; ++i)
	{
		if (fscanf(in, "%d %d", &p.sick[i].x, &p.sick[i].y) != 2)
		{
			fprintf(stderr, "project13: Unable to read sick point %u\n", i);
			exit(1);
		}
		//fprintf(stdout, "sick[%u] = (%d, %d)\n", i, p.sick[i].x, p.sick[i].y);

	}
	p.queries = (Point*)malloc(sizeof(Point) * p.nQueries);
	for (unsigned i = 0; i < p.nQueries; ++i)
	{
		if (fscanf(in, "%d %d", &p.queries[i].x, &p.queries[i].y) != 2)
		{
			fprintf(stderr, "project13: Unable to read queries point %u\n", i);
			exit(1);
		}
		//fprintf(stdout, "queries[%u] = (%d, %d)\n", i, p.queries[i].x, p.queries[i].y);

	}
	return p;
}
//Determining the magnitude of the distance between these numbers. (avoiding the sqrt)
int distance(Point p1, Point p2)
{
	int dx = p1.x - p2.x; //finding out if the xs are the same.
	int dy = p1.y - p2.y; //finding out if the ys are the same.
	return dx * dx + dy * dy;
}
//compare function
int compareTo(void const* p1, void const* p2)
{
	Point const* pp1 = (Point const*)p1;
	Point const* pp2 = (Point const*)p2;
	int p1Distance = distance(myLocation, *pp1);// distance between us and p1.
	int p2Distance = distance(myLocation, *pp2);// distance between us and p2.
	int dDistance = p1Distance - p2Distance; // difference between distances of p1 and p2.
	if (dDistance != 0)
	{
		return dDistance;
	}
	int dx = pp1->x - pp2->x; //finding out if the xs are the same.
	int dy = pp1->y - pp2->y; //finding out if the ys are the same.
	if (dx != 0)
	{
		return dx;
	}
	return dy; 
	// if we return 0 p1 = p2.
	// if we return a - number p1 < p2 if we return a + number p1 > p2.
}

unsigned binarySearch(Point* points, unsigned size, Point query)
{
	unsigned low = 0; // low variable
	unsigned high = size; // high variable
	while (low < high)
	{
		unsigned mid = (high + low) / 2;
		int comparison = compareTo(&query, &points[mid]);
		//fprintf(stderr, "binary search: low = %u, high = %u, mid = %u, comapison= %d\n", low, high, mid, comparison);
		if (comparison < 0)
		{
			high = mid;
			continue;
		}
		if (comparison > 0)
		{
			low = mid + 1;
			continue;
		}
		return mid;
	}
	return size;
}

void insertionSort(Point* pts, unsigned npts)
{
	for (unsigned i = 0; i < npts; ++i)
	{
		for (unsigned j = i + 1; j < npts; j++)
		{
			if (compareTo(&pts[i], &pts[j]) > 0)
			{
				Point temp = pts[i];
				pts[i] = pts[j];
				pts[j] = temp;
			}
		}
	}
}

void mergeSort(Point* pts, unsigned npts, unsigned threshold)
{
	if (npts < threshold)
	{
		insertionSort(pts, npts);
		return;
	}
	unsigned nlow = npts / 2;
	unsigned nhigh = npts - nlow;
	Point* plow = (Point*)malloc(sizeof(Point) * nlow);
	Point* phigh = (Point*)malloc(sizeof(Point) * nhigh);
	memcpy(plow, &pts[0], sizeof(Point) * nlow);
	memcpy(phigh, &pts[nlow], sizeof(Point) * nhigh);
	mergeSort(plow, nlow, threshold);
	mergeSort(phigh, nhigh, threshold);
	unsigned i = 0, j = 0, k = 0;
	while (j < nlow && k < nhigh)// j is pointing to low k is pointing to high and i is where we merge.
	{
		if (compareTo(&plow[j], &phigh[k]) <= 0)
		{
			pts[i++] = plow[j++];
		}
		else
		{
			pts[i++] = phigh[k++];
		}
	}
	while (j < nlow)
	{
		pts[i++] = plow[j++];
	}
	while (k < nhigh)
	{
		pts[i++] = phigh[k++];
	}
	free(plow);
	free(phigh);
}

void sortSick(Point* pts, unsigned npts, unsigned threshold)
{
	if (npts < threshold)
	{
		insertionSort(pts, npts);
	}
	else
	{
		mergeSort(pts, npts, threshold);
	}
}

int main()
{
	Points ps = loadPoints();
	sortSick(ps.sick, ps.nSick, ps.threshold);
		//qsort(ps.queries, ps.nQueries, sizeof(Point), compareTo);
	/*2 2
	-1 3
		3 1
		- 4 - 2
		- 2 4
		2 4
		- 4 3
		0 - 5
		0 5
		4 - 4
		4 4
		- 6 - 2
		- 4 6
		- 6 6
		2 - 1 not found
		3 1 found at rank 3
		0 - 5 found at rank 8
		0 5 found at rank 9
		- 6 7 not found*/
	//fprintf(stdout, "My location = {%d %d}\n", myLocation.x, myLocation.y);
	//fprintf(stdout, "nSick = %u\n", ps.nSick);
	for (unsigned i = 0; i < ps.nSick; ++i)
	{
		//fprintf(stdout, "sick[%u] = (%d, %d)\n", i, ps.sick[i].x, ps.sick[i].y);
		fprintf(stdout, "%d %d\n", ps.sick[i].x, ps.sick[i].y);
	}
	/*fprintf(stdout, "nQueries = %u\n", ps.nQueries);
	for (unsigned i = 0; i < ps.nQueries; ++i)
	{
		fprintf(stdout, "queries[%u] = (%d, %d)\n", i, ps.queries[i].x, ps.queries[i].y);
	}
	fprintf(stdout, "Threshold = %u\n", ps.threshold);*/

	for (unsigned q = 0; q < ps.nQueries; ++q)
	{
		unsigned s = binarySearch(ps.sick, ps.nSick, ps.queries[q]);
		int found = s != ps.nSick;
		if (found)
		{
			//fprintf(stdout, "Queries[%u] is sick[%u]\n", q, s);
			fprintf(stdout, "%d %d found at rank %u\n", ps.queries[q].x, ps.queries[q].y, s + 1);
		}
		else
		{
			//fprintf(stdout, "Queries[%u] is not sick\n", q);
			fprintf(stdout, "%d %d not found\n", ps.queries[q].x, ps.queries[q].y);

		}
	}
	return 0;
}