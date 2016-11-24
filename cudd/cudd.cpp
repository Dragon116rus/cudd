// cudd.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "cudd.h"
#include <iostream>
using namespace std;

DdManager * mgr;
DdNode ***nodes;
int n;
void init() 
{
	cin>>n;
	
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
DdNode* getDiagonalNE(int i,int n) {

}
DdNode* buildBDD(int n) 
{
	DdNode* root=Cudd_ReadOne(mgr);
	Cudd_Ref(root);
	for (int i = 0; i < n; i++)
	{
		DdNode* vertical=getVertical(i,n);
		Cudd_Ref(vertical);
		DdNode* gorizontal=getGorizontal(i,n);
		Cudd_Ref(gorizontal);
		/*DdNode* diagonalNE=getDiagonalNE(i,n);
		Cudd_Ref(diagonalNE);
		DdNode* diagonalNW=getDiagonalNW(i,n);
		Cudd_Ref(diagonalNW);*/
		DdNode* lines=Cudd_bddAnd(mgr,gorizontal,vertical);
		Cudd_Ref(lines);
		Cudd_RecursiveDeref(mgr,gorizontal);
		Cudd_RecursiveDeref(mgr,vertical);
		/*DdNode* diagonals=Cudd_bddAnd(mgr,diagonalNE,diagonalNW);
		Cudd_Ref(diagonals);
		Cudd_RecursiveDeref(mgr,diagonalNE);
		Cudd_RecursiveDeref(mgr,diagonalNW);
		DdNode* dotRoot=Cudd_bddAnd(mgr,lines,diagonals);
		Cudd_Ref(dotRoot);
		Cudd_RecursiveDeref(mgr,lines);
		Cudd_RecursiveDeref(mgr,diagonals);*/
		DdNode* tmp=Cudd_bddAnd(mgr,lines,root);//DdNode* tmp=Cudd_bddAnd(mgr,dotRoot,root);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(mgr,lines);//Cudd_RecursiveDeref(mgr,dotRoot);
		Cudd_RecursiveDeref(mgr,root);
		root=tmp;
	}
	return root;
}
int main()
{
	init();

	DdNode* root=buildBDD(n);

	/* Build BDD. */



	/* Inspect it. */
	//  printf("f");
	// Cudd_PrintSummary(mgr, f, 4, 0);
	//Cudd_PrintDebug(mgr,f,1,4);
	Cudd_bddPrintCover(mgr, root, root);
	// char * fform = Cudd_FactoredFormString(mgr, f, inames);
	// printf("%s\n", fform);
	/* Break up camp and go home. */
	// free(fform);
	// Cudd_RecursiveDeref(mgr, f);
	//  int err = Cudd_CheckZeroRef(mgr);
	// Cudd_Quit(mgr);

	dispose();
	return 0;
}

