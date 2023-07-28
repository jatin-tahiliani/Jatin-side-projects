/* Your code here! */

#include "dsets.h"

using namespace std;

void DisjointSets::addelements(int num){
    for(int i=0; i<num; i++){
        sets.push_back(-1);
    }
}	

int DisjointSets::find(int elem){
    if(sets[elem]<0){
        return elem;
    }
    else{
        return find(sets[elem]);
    }
}	

void DisjointSets::setunion(int a, int b){
    while(sets[a]>=0){
        a = sets[a];
    }

    while(sets[b]>=0){
        b = sets[b];
    }

    int size = sets[a] + sets[b];

    if(sets[a] < sets[b]){
        sets[b] = a;
        sets[a] = size;
    }
    else{
        sets[a] = b;
        sets[b] = size;
    }
}

int DisjointSets::size(int elem){
    int root = find(elem);
    int root_size = sets[root]*(-1);
    return root_size;
}	


