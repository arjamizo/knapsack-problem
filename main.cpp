/*
 * main.cpp
 *
 *  Created on: 13-05-2012
 *      Author: Eta
 */

#include <vector>
#include <stdio.h>
#include <utility>
#include <exception>
#include <memory> //shared_ptr
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>

using namespace std;

class KnapsackProblemSolver {
public:
	vector <int> vols;
	vector <int> vals;
	vector <vector<int> > C;
	void add_job(int a, int w) {
		vols.push_back(a);
		vals.push_back(w);
		C.resize(vols.size()+1);
	}

	void init(vector<int> &vols, vector<int> &vals) {
		this->vols=vols;
		this->vals=vals;
		C.resize(vols.size()+1);
	}

	/**
	 * returns volume of element numerated from 1 to jobs_number
	 */
	int getVolume(size_t j) {
		if (j>=1 && j<=vols.size())
			return vols[j-1];
		throw new exception();
	}
	/**
	 * returns value of element numerated from 1 to jobs_number
	 */
	int getValue(size_t j) {
		if (j>=1 && j<=vals.size())
			return vals[j-1];
		throw new exception();
	}

	operator std::string()
	{
		stringstream ss;
		for (size_t i = 0; i < C.size(); ++i) {
			std::copy(C[i].begin(), C[i].end(), std::ostream_iterator<int>(ss, ", "));
			ss<<"\n";
		}
		return ss.str();
	}

	int c(int j, int A) {
		unsigned int jmax = C.size();
		unsigned int Amax = C[0].size();
		if (j < 0 || j >= jmax || A < 0 || A >= Amax)
			throw new exception();
		if(C[j][A]==-1) {
	        if(j==0 || A==0) {
	            return C[j][A]=0;
	        } else if (getVolume(j)>A) {
	            return C[j][A]=c(j-1, A);
	        } else {
	            return C[j][A]=std::max(c(j-1, A), c(j-1, A-getVolume(j))+getValue(j));
	        }
		}
		return C[j][A];
	}

	int calculateTable(int capacity) {
		for (size_t i = 0; i < C.size(); ++i) {
			C[i].resize(capacity+1);
			std::fill(C[i].begin(), C[i].end(), -1);
		}
		return c(vals.size(), capacity);
	}

	vector<int> backtrackSol() {
		int capPos=C.front().size()-1;
		vector<int> res(C.size()-1, 0);
		int j=C.size()-1;
		while (j!=0 && capPos!=0) {
			std::cout<<"processing: ("<<j<<","<<capPos<<")\n";
			int wasjtaken=(C[j-1][capPos]!=C[j][capPos]);
			if(wasjtaken) {
				res[j-1]=j;
				int moveLeft=getVolume(j);
				capPos-=moveLeft;
			}
			j-=1;
		}
		return res;
	}

};



void exampleDataSlideTPI146(KnapsackProblemSolver& kps) {
	//ftp://sith.ict.pwr.wroc.pl/Informatyka/SDIZO/TPI.pdf
	kps.add_job(1, 9);
	kps.add_job(2, 8);
	kps.add_job(6, 7);
	kps.add_job(7, 10);
	kps.add_job(1, 8);
	kps.add_job(4, 6);
}

int main(int, char *argv[]) {
	KnapsackProblemSolver kps;
	exampleDataSlideTPI146(kps);
	//std::cout<<kps.vals.size()<<","<<kps.vols.size()<<endl;
	std::cout << "best sol=" << kps.calculateTable(12) << endl;
	std::cout << (string)(kps) <<endl;
	vector<int> bestSol=kps.backtrackSol();
	std::remove(bestSol.begin(), bestSol.end(), 0);
	std::copy(bestSol.begin(), bestSol.end(), std::ostream_iterator<int>(std::cout, ", "));
	cout<<endl;
	return 0;
}
