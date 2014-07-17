#ifndef AVLTREE_H
#define AVLTREE_H
#include "avlnode.h"
#include<fstream>
#include<iostream>
#include<stdio.h>

using namespace std;

class avlTree
{
public:
    /** Construtor e Destrutores **/
    avlTree();
    ~avlTree();
    void clear();

    /** Insere o elemento na árvore **/
    void insert(const int& data);

    /** Remove um elemento da árvore **/
    bool remove(const int data);

   /** Busca um elemento na árvore **/
    bool search(int data);

    /** Exibe detalhes da árvore na tela **/
    void print(int data);
    void printNode();

private:
    avlNode *root; //Raiz

    /** Métodos recursivos para Inserção e Remoção **/
    void recursiveInsert(const int& data, avlNode*& node, bool& changedHeight);
    bool recursiveRemove(const int& data, avlNode*& node, bool& changedHeight);

    /** Rotações **/
    void simpleRotation(avlNode *&node, Direction dir);
    void doubleRotation(avlNode*& node, Direction dir);

    /** Balanceamento/Rebalanceamento **/
    void balancement(avlNode* node, Direction dir);
    void rebalanceInsert(avlNode*& node, Direction dir, bool& changedHeight);
    void rebalanceRemove(avlNode*& node, Direction dir, bool& changedHeight);

    /** Recursivo auxiliar de print() **/
    void printNodeRec(avlNode *node);

    /** Este método inverte a direção de todos os acessos(Útil para generalizar as rotações) **/
    inline Direction opposite(Direction dir);
};

#endif // AVLTREE_H
