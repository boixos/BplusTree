#include <bits/stdc++.h>
#include <fstream>

using namespace std;
#define lli long int
#define loop(a, b, c) for (int a = b; a < c; a++)
#define eloop(a, b, c) for (int a = b; a <= c; a++)
#define rloop(a, b, c) for (int a = b; a > c; a--)
ofstream outfile("output.txt");
int order = 3;
lli count_ = 0;

class TreeNode{
public:
	lli *rec;
	lli size;
	TreeNode **cptr;
	bool leaf;
	TreeNode()
	{
		rec = new lli[3];
		cptr = new TreeNode *[4];
	}
	friend class bplustree;
};

class bplustree{
public:
	TreeNode *root;
	TreeNode *get_root();
	void insert_helper(TreeNode *, TreeNode *, lli);
	TreeNode *get_parent(TreeNode *, TreeNode *);
	bplustree()
	{
		root = NULL;
	}

	void insert(lli);
	void find(lli);
	void range(lli, lli);
	void count(lli);
};

void bplustree::insert(lli x){

	if (root == NULL){
		root = new TreeNode;
		root->rec[0] = x;
		root->leaf = true; 
		root->size = 1;
		// cout<<"rootinit";
		return;
	}

	else{	
		TreeNode *cnode = root,*pptr;

		while (cnode->leaf != true){
			pptr = cnode;
			loop(i, 0, cnode->size){

				if (x < cnode->rec[i]){
					cnode = cnode->cptr[i];
					break;
				}
				if (i == cnode->size - 1){
					cnode = cnode->cptr[i + 1];
					break;
				}
			}
		}

		if (cnode->size < order){
			int i = 0;
			while (x >= cnode->rec[i] && i < cnode->size){
				i++;
			}
			// cout<<i;
			rloop(j, cnode->size, i){
				cnode->rec[j] = cnode->rec[j - 1];
			}

			cnode->rec[i] = x;
			cnode->size++;

			cnode->cptr[cnode->size] = cnode->cptr[cnode->size - 1];
			cnode->cptr[cnode->size - 1] = NULL;
		}

		else{
			TreeNode *newLeaf = new TreeNode;
			int virtualNode[order + 1];

			loop(i, 0, order){
				virtualNode[i] = cnode->rec[i];
			}
			int i = 0, j;
			while (x >= virtualNode[i] && i < order){
				i++;
			}
			// cout<<i;
			rloop(j, order + 1, i){
				virtualNode[j] = virtualNode[j - 1];
			}

			virtualNode[i] = x;
			newLeaf->leaf = true;

			cnode->size = (order + 1) / 2;
			newLeaf->size = order + 1 - (order + 1) / 2;

			cnode->cptr[cnode->size] = newLeaf;

			newLeaf->cptr[newLeaf->size] = cnode->cptr[order];

			cnode->cptr[order] = NULL;

			loop(i, 0, cnode->size){
				cnode->rec[i] = virtualNode[i];
				// cout<<"hererr";
			}

			j = cnode->size;
			loop(i, 0, cnode->size){
				newLeaf->rec[i] = virtualNode[j];
				j++;
			}

			if (cnode == root){

				TreeNode *temproot = new TreeNode;

				temproot->rec[0] = newLeaf->rec[0];
				temproot->cptr[0] = cnode;
				temproot->cptr[1] = newLeaf;
				temproot->leaf = false;
				temproot->size = 1;
				root = temproot;
			}
			else{

				insert_helper(pptr, newLeaf, newLeaf->rec[0]);
			}
		}
	}
}
void bplustree::insert_helper(TreeNode *cnode, TreeNode *child, lli x){

	if (cnode->size < order){
		int i = 0;

		while (x >= cnode->rec[i] && i < cnode->size){
			i++;
		}
		rloop(j, cnode->size, i){

			cnode->rec[j] = cnode->rec[j - 1];
		}
		rloop(j, cnode->size + 1, i + 1){
			cnode->cptr[j] = cnode->cptr[j - 1];
		}
		cnode->rec[i] = x;
		cnode->size++;
		cnode->cptr[i + 1] = child;
	}

	else{
		TreeNode *newInternal = new TreeNode,*tempptr[order + 2];
		int temprec[order + 1];
		
		loop(i, 0, order){
			temprec[i] = cnode->rec[i];
		}
		loop(i, 0, order + 1){
			tempptr[i] = cnode->cptr[i];
		}
		int i = 0, j;
		while (x >= temprec[i] && i < order){
			i++;
		}
		rloop(j, order + 1, i){
			temprec[j] = temprec[j - 1];
		}
		temprec[i] = x;

		rloop(j, order + 2, i + 1){
			tempptr[j] = tempptr[j - 1];
		}

		tempptr[i + 1] = child;
		newInternal->leaf = false;

		cnode->size = (order + 1) / 2;

		newInternal->size = order - (order + 1) / 2;
		int t = cnode->size+1;
		j = t;
		loop(i,0,newInternal->size){
			newInternal->rec[i] = temprec[j];
			j++;
		}
		j=t;
		loop(i,0,newInternal->size+1){
			newInternal->cptr[i] = tempptr[j];
			j++;
		}

		if (cnode == root){
			TreeNode *temproot = new TreeNode;
			temproot->rec[0] = cnode->rec[cnode->size];
			temproot->cptr[0] = cnode;
			temproot->cptr[1] = newInternal;
			temproot->leaf = false;
			temproot->size = 1;
			root = temproot;
		}

		else{
			insert_helper(get_parent(root, cnode), newInternal, cnode->rec[cnode->size]);
		}
	}
}

TreeNode *bplustree::get_parent(TreeNode *cnode,TreeNode *child){
	TreeNode *pptr;
	if (cnode->leaf != false || (cnode->cptr[0])->leaf !=false){
		return NULL;
	}
	loop(i, 0, cnode->size + 1){
		if (cnode->cptr[i] == child){
			pptr = cnode;
			return pptr;
		}
		else{
			pptr = get_parent(cnode->cptr[i], child);
			if (pptr != NULL)
				return pptr;
		}
	}
	return pptr;
}

void bplustree::find(lli x){
// 	if (root == NULL){
// 		cout << "Tree is empty\n";
// 	}
// 	else{
// 		TreeNode *cnode = root;
// 		while (cnode->leaf == false){
// 			loop(i, 0, cnode->size){
// 				if (x < cnode->rec[i]){
// 					cnode = cnode->cptr[i];
// 					break;
// 				}
// 				if (i == cnode->size - 1){
// 					cnode = cnode->cptr[i + 1];
// 					break;
// 				}
// 			}
// 		}
// 		loop(i, 0, cnode->size){
// 			if (cnode->rec[i] == x){
// 				outfile << "YES\n";
// 				return;
// 			}
// 		}
// 		outfile << "NO\n";
// 	}

	if (root == NULL){
		cout << "Tree is empty\n";
	}
	else{
		TreeNode *cnode = root;
		while (cnode->leaf == false){
			loop(i, 0, cnode->size){
				if (x < cnode->rec[i]){
					cnode = cnode->cptr[i];
					break;
				}
				if (i == cnode->size - 1){
					cnode = cnode->cptr[i + 1];
					break;
				}
			}
		}
		if(cnode == NULL){
			outfile<< "NO\n";
			return;
		}
		loop(i,0,cnode->size){
			if(cnode->rec[i]==x){
				outfile<<"YES\n";
				return;
			}
		}
		outfile<<"NO\n";
	}
}

void bplustree::range(lli x, lli y){
	if (root == NULL){
		cout << "Tree is empty\n";
	}
	else{
		TreeNode *cnode = root;
		while (cnode->leaf == false){
			loop(i, 0, cnode->size){
				if (x < cnode->rec[i]){
					cnode = cnode->cptr[i];
					break;
				}
				if (i == cnode->size - 1){
					cnode = cnode->cptr[i + 1];
					break;
				}
			}
		}
		while (cnode != NULL){
			loop(i, 0, cnode->size){
				if (x <= cnode->rec[i] && cnode->rec[i] <= y)
					// cout<<cnode->rec[i];
					count_++;

				if (cnode->rec[i] > y)
					cnode == NULL;
			}
			cnode = cnode->cptr[cnode->size];
		}
	}
}

void bplustree::count(lli x){
	lli y = x;
	if (root == NULL){
		cout << "Tree is empty\n";
	}
	else{
		TreeNode *cnode = root;
		while (cnode->leaf == false){
			loop(i, 0, cnode->size){
				if (x < cnode->rec[i]){
					// cout<<cnode->rec[i]<<' ';
					cnode = cnode->cptr[i];
					break;
				}
				if (i == cnode->size - 1){
					cnode = cnode->cptr[i + 1];
					break;
				}
			}
		}
		while (cnode != NULL){
			loop(i, 0, cnode->size){
				if (x <= cnode->rec[i] && cnode->rec[i] <= y)
					// cout<<'countinc';
					count_++;

				if (cnode->rec[i] > y)
					cnode == NULL;
			}
			cnode = cnode->cptr[cnode->size];
		}
	}
}

int main(int argc, char *argv[]){
	bplustree nde;
	string ipf = argv[1];
	ifstream infile(ipf);
	// ofstream outfile("output.txt");

	string line, s, s1;
	while (getline(infile, line))
	{
		istringstream iss(line);
		if (!(iss >> s >> s1))
		{
			break;
		}

		else
		{ // cout<<s;
			if (s == "INSERT"){
				// cout<<s<<" ";
				// iss>>s;
				nde.insert(stoi(s1));
			}
			else if (s == "COUNT"){
				// iss>>s;
				nde.count(stoi(s1));
				outfile << count_;
				outfile << '\n';
				count_ = 0;
			}
			else if (s == "RANGE"){
				int x, y;
				// iss>>s;
				x = stoi(s1);
				iss >> s;
				y = stoi(s);
				nde.range(x, y);
				outfile << count_;
				outfile << '\n';
				count_ = 0;
			}
			else if (s == "FIND"){
				// iss>>s;
				nde.find(stoi(s1));
			}
			else
				cout << "ERROR: Invalid query'\n'";
		}
	}
	// nde.display(nde.get_root());
	infile.close();
	outfile.close();
	return 0;
}
