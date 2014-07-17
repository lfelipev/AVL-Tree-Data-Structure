#include <iostream>
#include <string.h>
#include "avlTree.h"

using namespace std;

int main() {
    avlTree* arvore;

    if (arvore == NULL)
    {
        arvore = new avlTree();
    }
    arvore->insert(1);
    arvore->insert(2);
    arvore->insert(3);
    arvore->insert(4);
    arvore->insert(5);
    arvore->insert(6);
    arvore->insert(7);
    arvore->remove(7);
    int a = 4;
    if(arvore->search(a))
    {
        arvore->print(a);
    }
    else
    {
        cout << "O elemento nao se encontra na arvore!" << endl;
    }

    arvore->clear();

    return 0;
}
