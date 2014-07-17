#ifndef AVLNODE_H
#define AVLNODE_H
#include<iostream>
#include<stdio.h>

/** Enumeração para acessar as subarvores através da direção **/
enum Direction {
    LEFT = 0, //0 significa que deve-se procurar pela esquerda
    RIGHT = 1, //1 significa que deve-se procurar pela direita
    EQUAL = 2, //Subarvores iniciam com valores iguais
};

class avlNode {
public:
    /** Constroi um nó a partir de um elemento **/
    avlNode(const int& data) {
        this->element = data;
        this->balFactor = EQUAL;
        this->subTree[LEFT] = NULL;
        this->subTree[RIGHT] = NULL;
    }

    /** Destrutor **/
    ~avlNode() {
        delete this->subTree[LEFT];
        delete this->subTree[RIGHT];
    }

    /** Conteúdo dos nós **/
    int element; //Dado associado ao nó (Pode facilmente ser acrescentado mais dados)
    unsigned short balFactor;//Fator de balanceamento
    avlNode *subTree[2];//Subárvores direita e esquerda
};

#endif // AVLNODE_H
