// cudd.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "cudd.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
using namespace std;

DdManager * mgr;
DdNode ***nodes;
int r=0;
int n;
void init() 
{
	mgr = Cudd_Init(n*n,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
	nodes =new DdNode**[n];
	for (int i = 0; i < n; i++)
	{
		nodes[i]=new DdNode*[n];
		for (int j = 0; j < n; j++)
		{
			nodes[i][j]=Cudd_bddIthVar(mgr, i*n+j);
		}
	}
}
void dispose() 
{
	for (int i = 0; i < n; i++)
	{
		delete []nodes[i];
	}
	Cudd_Quit(mgr);
}
DdNode* getVerticalConj(int i,int k,int n)
{
	DdNode* root=Cudd_ReadOne(mgr);
	Cudd_Ref(root);
	for(int j=0;j<n;j++) 
	{
		DdNode* conj;
		if (j!=k)
			conj=Cudd_bddAnd(mgr,Cudd_Not(nodes[j][i]),root);
		else 
			conj=Cudd_bddAnd(mgr,(nodes[j][i]),root);
		Cudd_Ref(conj);
		Cudd_RecursiveDeref(mgr,root);
		root=conj;
	}
	return root;
}
DdNode* getVertical(int i,int n) 
{

	DdNode* root=Cudd_Not(Cudd_ReadOne(mgr));
	Cudd_Ref(root);
	for (int k = 0; k < n; k++)
	{
		DdNode* verticalConj =getVerticalConj(i,k,n);
		DdNode* tmp=Cudd_bddOr(mgr,verticalConj,root);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(mgr,verticalConj);
		Cudd_RecursiveDeref(mgr,root);
		root=tmp;
	}
	return root;
}
DdNode* getGorizontalConj(int i,int k,int n)
{
	DdNode* root=Cudd_ReadOne(mgr);
	Cudd_Ref(root);
	for(int j=0;j<n;j++) 
	{
		DdNode* conj;
		if (j!=k)
			conj=Cudd_bddAnd(mgr,Cudd_Not(nodes[i][j]),root);
		else 
			conj=Cudd_bddAnd(mgr,(nodes[i][j]),root);
		Cudd_Ref(conj);
		Cudd_RecursiveDeref(mgr,root);
		root=conj;
	}
	return root;
}
DdNode* getGorizontal(int i,int n) 
{
	DdNode* root=Cudd_Not(Cudd_ReadOne(mgr));
	Cudd_Ref(root);
	for (int k = 0; k < n; k++)
	{
		DdNode* gorizontalConj =getGorizontalConj(i,k,n);
		Cudd_Ref(gorizontalConj);
		DdNode* tmp=Cudd_bddOr(mgr,gorizontalConj,root);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(mgr,gorizontalConj);
		Cudd_RecursiveDeref(mgr,root);		
		root=tmp;
	}
	return root;
}
DdNode* getDiagonalNWConj1(int i,int k,int n)
{
	int len=n-i;
	DdNode* root=Cudd_ReadOne(mgr);
	Cudd_Ref(root);
	for(int j=0;j<len;j++) 
	{
		DdNode* conj;
		if (j!=k)
			conj=Cudd_bddAnd(mgr,Cudd_Not(nodes[j][j+i]),root);
		else 
			conj=Cudd_bddAnd(mgr,(nodes[j][j+i]),root);
		Cudd_Ref(conj);
		Cudd_RecursiveDeref(mgr,root);
		root=conj;
	}
	return root;
}
DdNode* getDiagonalNWConj2(int i,int k,int n)
{
	int len=n-i;
	DdNode* root=Cudd_ReadOne(mgr);
	Cudd_Ref(root);
	for(int j=0;j<len;j++) 
	{
		DdNode* conj;
		if (j!=k)						//if (i=0) continue
			conj=Cudd_bddAnd(mgr,Cudd_Not(nodes[j+i][j]),root);
		else 
			conj=Cudd_bddAnd(mgr,(nodes[j+i][j]),root);
		Cudd_Ref(conj);
		Cudd_RecursiveDeref(mgr,root);
		root=conj;
	}
	return root;
}
DdNode* getDiagonalNW1(int i,int n) {
	DdNode* root=Cudd_Not(Cudd_ReadOne(mgr));
	Cudd_Ref(root);
	for (int k = -1; k < n; k++)
	{
		DdNode* diagonalNWConj =getDiagonalNWConj1(i,k,n);
		Cudd_Ref(diagonalNWConj);
		DdNode* tmp=Cudd_bddOr(mgr,diagonalNWConj,root);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(mgr,diagonalNWConj);
		Cudd_RecursiveDeref(mgr,root);	
		root=tmp;
	}
	return root;
}
DdNode* getDiagonalNW2(int i,int n) {
	DdNode* root=Cudd_Not(Cudd_ReadOne(mgr));
	Cudd_Ref(root);
	for (int k = -1; k < n; k++)
	{
		DdNode* diagonalNWConj =getDiagonalNWConj2(i,k,n);
		Cudd_Ref(diagonalNWConj);
		DdNode* tmp=Cudd_bddOr(mgr,diagonalNWConj,root);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(mgr,diagonalNWConj);
		Cudd_RecursiveDeref(mgr,root);	
		root=tmp;
	}
	return root;

}
DdNode* getDiagonalNEConj1(int i,int k,int n)
{
	int len=n-i;
	DdNode* root=Cudd_ReadOne(mgr);
	Cudd_Ref(root);
	for(int j=0;j<len;j++) 
	{
		DdNode* conj;
		if (j!=k)
			conj=Cudd_bddAnd(mgr,Cudd_Not(nodes[j][n-1-j-i]),root);
		else 
			conj=Cudd_bddAnd(mgr,(nodes[j][n-1-j-i]),root);
		Cudd_Ref(conj);
		Cudd_RecursiveDeref(mgr,root);
		root=conj;

	}
	return root;
}
DdNode* getDiagonalNEConj2(int i,int k,int n)
{
	int len=n-i;
	DdNode* root=Cudd_ReadOne(mgr);
	Cudd_Ref(root);
	for(int j=0;j<len;j++) 
	{
		DdNode* conj;

		if (j!=k)						//if (i=0) continue  //if j!=k then cudd_not(conj)
			conj=Cudd_bddAnd(mgr,Cudd_Not(nodes[j+i][n-1-j]),root);
		else 
			conj=Cudd_bddAnd(mgr,(nodes[j+i][n-1-j]),root);

		Cudd_Ref(conj);
		Cudd_RecursiveDeref(mgr,root);
		root=conj;
	}
	return root;
}
DdNode* getDiagonalNE1(int i,int n) {
	DdNode* root=Cudd_Not(Cudd_ReadOne(mgr));
	Cudd_Ref(root);
	for (int k = -1; k < n; k++)
	{
		DdNode* diagonalNEConj =getDiagonalNEConj1(i,k,n);
		Cudd_Ref(diagonalNEConj);
		DdNode* tmp=Cudd_bddOr(mgr,diagonalNEConj,root);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(mgr,diagonalNEConj);
		Cudd_RecursiveDeref(mgr,root);	
		root=tmp;
	}
	return root;
}
DdNode* getDiagonalNE2(int i,int n) {
	DdNode* root=Cudd_Not(Cudd_ReadOne(mgr));
	Cudd_Ref(root);
	for (int k = -1; k < n; k++)
	{
		DdNode* diagonalNEConj =getDiagonalNEConj2(i,k,n);
		Cudd_Ref(diagonalNEConj);
		DdNode* tmp=Cudd_bddOr(mgr,diagonalNEConj,root);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(mgr,diagonalNEConj);
		Cudd_RecursiveDeref(mgr,root);	
		root=tmp;
	}
	return root;

}
DdNode* buildBDD(int n) 
{
	DdNode* root=Cudd_ReadOne(mgr);
	Cudd_Ref(root);

	unsigned int start_time =  clock();
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; 
	DdNode** nodesToCon=new DdNode*[n];
	for (int i = 0; i < n; i++)
	{


		DdNode* vertical=getVertical(i,n);
		DdNode* gorizontal=getGorizontal(i,n);

		DdNode* diagonalNE1=getDiagonalNE1(i,n);
		DdNode* diagonalNE2=getDiagonalNE2(i,n);

		DdNode* diagonalNW1=getDiagonalNW1(i,n);
		DdNode* diagonalNW2=getDiagonalNW2(i,n);



		DdNode* lines=Cudd_bddAnd(mgr,gorizontal,vertical);
		Cudd_Ref(lines);
		Cudd_RecursiveDeref(mgr,gorizontal);
		Cudd_RecursiveDeref(mgr,vertical);



		DdNode* diagonalNE=Cudd_bddAnd(mgr,diagonalNE1,diagonalNE2);
		Cudd_Ref(diagonalNE);
		Cudd_RecursiveDeref(mgr,diagonalNE1);
		Cudd_RecursiveDeref(mgr,diagonalNE2);

		DdNode* diagonalNW=Cudd_bddAnd(mgr,diagonalNW1,diagonalNW2);
		Cudd_Ref(diagonalNW);
		Cudd_RecursiveDeref(mgr,diagonalNW1);
		Cudd_RecursiveDeref(mgr,diagonalNW2);

		DdNode* diagonals=Cudd_bddAnd(mgr,diagonalNE,diagonalNW);
		Cudd_Ref(diagonals);
		Cudd_RecursiveDeref(mgr,diagonalNE);
		Cudd_RecursiveDeref(mgr,diagonalNW);



		DdNode* dotRoot=Cudd_bddAnd(mgr,lines,diagonals);
		Cudd_Ref(dotRoot);
		Cudd_RecursiveDeref(mgr,lines);
		Cudd_RecursiveDeref(mgr,diagonals);


		nodesToCon[i]=dotRoot;
		/*DdNode* tmp=Cudd_bddAnd(mgr,dotRoot,root);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(mgr,dotRoot);
		Cudd_RecursiveDeref(mgr,root);
		root=tmp;*/


	}
	int m=n;
	while (m!=1) {

		for (int j = 0; j < (m+1)/2; j++)
		{
			start_time =  clock();
			int a=j;
			int b=m-j-1;
			if (a==b)
				continue;
			DdNode* tmp=Cudd_bddAnd(mgr,nodesToCon[a],nodesToCon[b]);
			Cudd_Ref(tmp);
			Cudd_RecursiveDeref(mgr,nodesToCon[a]);
			Cudd_RecursiveDeref(mgr,nodesToCon[b]);
			nodesToCon[a]=tmp;

			end_time = clock(); // конечное время
			search_time = end_time - start_time; 
		//	cout<<"tmp:"<<search_time<<endl;
		}
		m=(m+1)/2;

	}
	root=nodesToCon[0];
	return root;
}
int main(int argc, char* argv[])
{
	stringstream ss; 

	//ss<<argv[1];
	//ss>>n;
	//cin>>n;
	unsigned int start_time =  clock();

	cin>>n;

	init();
	DdNode* root=buildBDD(n);

	
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; 
//	cout<<"count:"<<Cudd_CountMinterm(mgr,root,n*n)<<endl<<endl;            
	//Cudd_bddPrintCover1(mgr, root, root,n);
//	cout<<"count:"<<Cudd_CountMinterm(mgr,root,n*n)<<endl<<endl; 
	cout<<"time:"<<search_time<<"ms"<<endl;

	FILE* f;
//	ofstream fout("cudd_countOfSolves.txt");//= fopen("cudd_countOfSolves.txt", "w");
//	Cudd_SetStdout(mgr, f);
	//fout<<Cudd_CountPathsToNonZero(root);
	//fout.close();
	////fclose(f)

	//f=fopen ("cudd_solves.txt" , "w");
	//Cudd_SetStdout(mgr,f);
	//Cudd_PrintMinterm(mgr,root);
	//fclose(f);

	//fout.open("cudd_finished.txt");
	//fout << n;
	//fout.close();
//	DdNode* pseudoRoot=Cudd_bddAnd(mgr,Cudd_bddIthVar(mgr,n*n+1),root);
//	DdNode * nodes[1]={pseudoRoot};
////	Cudd_DumpDot(mgr,1,nodes,NULL,NULL,f);
	

	dispose();
	return 0;
}

