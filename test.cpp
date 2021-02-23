#include<bits/stdc++.h>
#include<fstream>

using namespace std;
#define lli int
#define loop(a,b,c) for(int a=b;a<c;a++)
#define eloop(a,b,c) for(int a=b;a<=c;a++)
#define rloop(a,b,c) for(int a=b;a>c;a--)
int order = 3;
int count_ = 0;
lli a,b;
ofstream outfile("output.txt");

class node{
    public:
        node();
        lli *rec;
        node** ptr;
        bool leaf;
        int cap;
        friend class bplustree;
};
class bplustree{
    public:
        bplustree();
        node* root;
        node* get_root();
        node* initroot(lli);
        node* get_parent(node*, node*);
        void insert_helper(node*,lli,node*);
        void count_helper(node*);
        void range_helper(node*);
        void display(node*);
        //req func
        void insert(lli);
        void find(lli);
        void count(lli);
        void range(lli,lli);

};


node::node(){
    rec = new lli[3];
    ptr = new node*[4];
}

bplustree::bplustree(){
    root = NULL;
}

node* bplustree::initroot(int x){
    	root = new node; 
		root->rec[0] = x; 
		root->leaf = true; 
		root->cap = 1; 
        return root;
}

node* bplustree::get_root(){
    return root;
}
void bplustree::find(lli x){
    if (root == NULL){
        outfile << "NO\n";
    }
    else{
        node *cptr = root;
        while (cptr->leaf == false){
            loop(i,0,cptr->cap){
                if (x < cptr->rec[i]){
                    cptr = cptr->ptr[i];
                    break;
                }
                if (i == cptr->cap - 1){
                    cptr = cptr->ptr[i + 1];
                    break;
                }
            }
        }
        loop(i,0,cptr->cap){
            if (cptr->rec[i] == x)
            {
                outfile << "YES\n";
                return;
            }
        }
        outfile << "NO\n";
    }
}

void bplustree::insert(lli x){
    if(root == NULL){
    	root = new node; 
		root->rec[0] = x; 
		root->leaf = true; 
		root->cap = 1; 
        }
    else{
        node* cptr = root;
        node* pptr;
        while(cptr->leaf == false){
            pptr = cptr;
            loop(i,0,cptr->cap){
                if(x < cptr->rec[i]){
                    cptr = cptr->ptr[i];
                    break;
                }
                else if(i == cptr->cap-1){
                    cptr = cptr->ptr[i+1];
                    break;
                }
            }
        }
        if(cptr->cap < order){
            lli i = 0;
            while(x >= cptr->rec[i] && i < cptr->cap)
                i++;
            // cout<<i;
            rloop(j,cptr->cap,i)
                cptr->rec[j] = cptr->rec[j-1];

            cptr->rec[i] = x;
            cptr->cap+=1;

            cptr->ptr[cptr->cap] = cptr->ptr[cptr->cap - 1]; 
			cptr->ptr[cptr->cap - 1] = NULL;     
        }
        else{
            node* cleaf = new node;
            lli newnode[4],pos=0;

            loop(i,0,3) newnode[i] = cptr->rec[i];

            while(x>=newnode[pos]&& pos<3) pos++;
            // cout<<pos;

            rloop(j,4,pos) newnode[j] = newnode[j-1];

            cleaf->leaf= true;
            newnode[pos] = x;

            cptr->cap = (order+1)/2;
            cleaf->cap =order +1 - (order+1)/2;
            
            cptr->ptr[cptr->cap] = cleaf;

            cleaf->ptr[cleaf->cap] = cptr->ptr[order];
            cptr->ptr[order] = NULL;

            loop(i,0,cptr->cap) cptr->rec[i] = newnode[i];
            for(int i=0,j=cptr->cap;i<cleaf->cap;i++,j++)
                cleaf->rec[i] = newnode[j];

            if(cptr == root){
				node* newRoot = new node; 
 
				newRoot->rec[0] = cleaf->rec[0]; 
				newRoot->ptr[0] = cptr; 
				newRoot->ptr[1] = cleaf; 
				newRoot->leaf = false; 
				newRoot->cap = 1; 
				root = newRoot;                 
            } 
            else insert_helper(cleaf,cleaf->rec[0],pptr);   
        }
    }
}
///////////////////////////###############

void bplustree::insert_helper(node* child,lli x,node* pptr) 
{ 

	if (pptr->cap < order) { 
		int i = 0; 

		while (x >= pptr->rec[i] && i < pptr->cap) { 
			i++; 
		} 
		rloop(j,pptr->cap,i) pptr->rec[j]= pptr->rec[j - 1]; 
		rloop(j,pptr->cap+1,i+1) pptr->ptr[j] = pptr->ptr[j - 1]; 
		pptr->rec[i] = x; 
		pptr->cap++; 
		pptr->ptr[i + 1] = child; 
	} 

	else { 

		node* newInternal = new node; 
		int virtualrec[order + 1]; 
		node* virtualPtr[order + 2]; 

		loop(i,0,order) virtualrec[i] = pptr->rec[i];  
        loop(i,0,order+1) virtualPtr[i] = pptr->ptr[i];

		int pos = 0, j; 

		while (x >= virtualrec[pos] && pos < order) pos++; 
        rloop(j,4,pos) virtualrec[j] = virtualrec[j - 1]; 

		virtualrec[pos] = x; 
        rloop(j,5,pos+1) virtualPtr[j]= virtualPtr[j - 1]; 

		virtualPtr[pos + 1] = child; 
		newInternal->leaf = false; 

		pptr->cap = (order + 1) / 2; 

		newInternal->cap = order - (order + 1) / 2; 

		for (pos = 0, j = pptr->cap + 1; pos < newInternal->cap; 
			pos++, j++) 
			newInternal->rec[pos] = virtualrec[j]; 
 
		for (pos = 0, j = pptr->cap + 1; pos < newInternal->cap + 1; 
			pos++, j++) { 

			newInternal->ptr[pos] = virtualPtr[j]; 
		} 

		if (pptr == root) { 

			node* newRoot = new node; 
			newRoot->rec[0] = pptr->rec[pptr->cap]; 
 
			newRoot->ptr[0] = pptr; 
			newRoot->ptr[1] = newInternal; 
			newRoot->leaf = false; 
			newRoot->cap = 1; 
			root = newRoot; 
		} 

		else 
			insert_helper(get_parent(root, pptr),pptr->rec[pptr->cap],newInternal); 
		 
	} 
} 

node* bplustree::get_parent(node* cursor, 
						node* child) 
{ 
	node* parent; 

	if (cursor->leaf || (cursor->ptr[0])->leaf) { 
		return NULL; 
	} 
	for (int i = 0; i < cursor->cap + 1; i++) { 

		if (cursor->ptr[i] == child) { 
			parent = cursor; 
			return parent; 
		} 

		else { 
			parent = get_parent(cursor->ptr[i], child); 

			if (parent != NULL) 
				return parent; 
		} 
	} 

	return parent; 
}

void bplustree::count_helper(node *cursor)
{
    if (cursor != NULL)
    {
        if (cursor->leaf == true)
        {
            loop(i, 0, cursor->cap)
            {
                if (cursor->rec[i] == a)
                    count_++;
                //   cout << cursor->rec[i] << " ";
            }
        }

        if (cursor->leaf != true)
        {
            loop(i, 0, cursor->cap + 1)           
                count_helper(cursor->ptr[i]);
            
        }
    }
}

void bplustree::count(lli x){
    a = x;
    count_helper(root);
    // return count_;
}

void bplustree::range_helper(node *cursor){
    if (cursor != NULL){
        if (cursor->leaf == true){
            loop(i, 0, cursor->cap)
                if (cursor->rec[i] >= a && cursor->rec[i] <= b)
                    count_++;
                //   cout << cursor->rec[i] << " ";
            
        }
        if (cursor->leaf != true){
            loop(i, 0, cursor->cap + 1)
                range_helper(cursor->ptr[i]);
            
        }
    }
}

void bplustree::range(lli x,lli y){
    a = x;
    b = y;
    range_helper(root);

}

void bplustree::display(node *cptr){
    if (cptr != NULL)
    {
        if (cptr->leaf == true)
            loop(i, 0, cptr->cap)
                cout << cptr->rec[i] << " ";         

        if (cptr->leaf != true)
            loop(i, 0, cptr->cap + 1)
                display(cptr->ptr[i]);
                
    }
}

// int main() 
// { 
// 	bplustree nde; 

// 	// Create B+ Tree 
// 	nde.insert(1); 
// 	nde.insert(20); 
//     nde.insert(3); 
// 	nde.insert(5); 
// 	nde.insert(7); 
// 	nde.insert(8); 

// 	nde.insert(6); 
// 	nde.insert(16); 
// 	nde.insert(26); 
//     nde.insert(-1000000000);
//     nde.count(6);
// 	// nde.find(6); 
//     // nde.range(5,16);
//     nde.display(nde.get_root());
//     cout<<count_;
// 	return 0; 
// } 

int main(){
    bplustree nde;
   // string ipf = ; //argv[1];
    ifstream infile("input3.txt");
    // ofstream outfile("output.txt");

    string line,s,s1;
    while(getline(infile,line)){
        istringstream iss(line);
        if(!(iss>>s>>s1)){break;}
       
            // cout<<s;
            if (s == "INSERT"){
                cout<<s<<" ";
                // iss>>s;
                nde.insert(stoi(s1));
            }
            if(s=="COUNT"){
                // iss>>s;
                nde.count(stoi(s1));
                outfile<<count_;
                outfile<<'\n';
                count_ = 0;
            }
            if(s=="RANGE"){
                int x,y;
                // iss>>s;
                x = stoi(s1);
                iss>>s; y = stoi(s);
                nde.range(x,y);
                outfile<<count_;
                outfile<<'\n';
                count_=0;
            }
            if(s=="FIND"){
                // iss>>s;
                nde.find(stoi(s1));
            }
            // else cout<<"ERROR: Invalid query'\n'";
        

    }
    nde.display(nde.get_root());
    infile.close();
    outfile.close();
    return 0;
}
