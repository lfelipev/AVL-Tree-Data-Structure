#include "avltree.h"
#include "avlnode.h"

/** ------------------- Construtor e Destrutores ------------------- **/
avlTree::avlTree()
{
    this->root = NULL;
}

avlTree::~avlTree()
{
    this->clear();
}

void avlTree::clear()
{
    delete root;
    this->root = NULL;
}

/** ------------------- Busca ------------------- **/
bool avlTree::search(int data)
{
    avlNode* current = this->root; //Raiz

    while(current != NULL)
    {
        if(data > current->element)
        {
            current = current->subTree[RIGHT]; //O valor está para a direita
        }
        else if(data < current->element) {
            current = current->subTree[LEFT]; //O valor está para a esquerda
        }
        else
        {
            data = current->element; //Retorna o valor pedido quando achado
            return true;
        }
    }
    return false; //Se o valor não se encontra na árvore, retorna falso.
}

/** ------------------- Inserção ------------------- **/
void avlTree::insert(const int &data)
{
    bool changedHeight = false; //Passagem por referência

    this->recursiveInsert(data, this->root, changedHeight); //Chama a função recursiva
}

void avlTree::recursiveInsert(const int& data, avlNode*& node, bool &changedHeight)
{
    if (node == NULL)//Quando o nó atual é nulo
    {
        node = new avlNode(data); //Cria o nó e insere

        changedHeight = true; //Notifca que a altura da árvore mudou
    }

    else if (node->element == data) //Caso o elemento já esteja na árvore
    {
        return;//Retorna o resultado da recursão
    }

    else //Caso o nó atual não ser nulo
    {
        Direction dir = (data > node->element) ? RIGHT : LEFT;//O novo elemento é maior que o atual?
                                                             //Se sim: Direita | Se não: Esquerda

        changedHeight = false;
        recursiveInsert(data, node->subTree[dir], changedHeight); //Chama a recursão

        if (changedHeight)
        {
            //Se a altura mudou, o balanceamento é chamado
            this->rebalanceInsert(node, dir, changedHeight);
        }
    }
}

/** ------------------- Remoção ------------------- **/
bool avlTree::remove(const int data)
{
    bool changedHeight = false;

    return this->recursiveRemove(data, this->root, changedHeight);
}

bool avlTree::recursiveRemove(const int& data, avlNode*& node, bool& changedHeight)
{
    bool success = false;

    if(node == NULL) //Caso o nó não seja encontrado
    {
        changedHeight = false;
        return false;
    }
    else if(data == node->element)  //Caso o nó tenha os 2 filhos
    {
        if(node->subTree[LEFT] != NULL && node->subTree[RIGHT] != NULL) {
            avlNode* substitute = node->subTree[LEFT];

            while(substitute->subTree[RIGHT] != NULL)
            {//Procura o nó mais a direita da subarvore da esquerda
                substitute = substitute->subTree[RIGHT];
            }

            //Trocamos o valor
            node->element = substitute->element;
            success = recursiveRemove(node->element, node->subTree[LEFT], changedHeight);

            if(changedHeight) //A altura mudou
            {
               rebalanceRemove(node, LEFT, changedHeight); //Chama o rebalanceamento
            }
        }

        else //O nó tem apenas 1 filho
        {
            avlNode* temp = node;

            Direction dir = (node->subTree[LEFT] == NULL) ? RIGHT : LEFT;

            node = node->subTree[dir]; //Substitui pelo filho

            //Deleta
            temp->subTree[LEFT] = NULL;
            temp->subTree[RIGHT] = NULL;
            delete temp;

            changedHeight = true; //A altura muda
        }

        return true; //A operação foi concluida
    }

    else //Caso não encontramos o nó
    {
        //Fazemos a pesquisa binária
        Direction dir = (data > node->element) ? RIGHT : LEFT;

        if(node->subTree[dir] != NULL)
        {
            //Chamamos a recursão para remover caso encontrar
            success = this->recursiveRemove(data, node->subTree[dir], changedHeight);
        }
        else
        {
            //O nó não está na árvore
            changedHeight = false;
            return false;
        }

        if(changedHeight)
        {
            //Se a altura mudou, rebalancearemos
            this->rebalanceRemove(node, dir, changedHeight);
        }

        //Retorna se o nó foi ou não encontrado
        return success;
    }
}

/** ------------------- Rotação ------------------- **/
void avlTree::simpleRotation(avlNode*& node, Direction dir)
{
    //      6
    //  4       5  //Precisa de uma rotação para a esquerda (dir = 0) (Opposite = 1)
    int opposite = this->opposite(dir); //Sentido da rotação

    avlNode* child = node->subTree[dir];//aux = 5
    node->subTree[dir] = child->subTree[opposite];//5 muda para 4
    child->subTree[opposite] = node;//4 muda para 6

    node = child;//6 recebe 5
    //   5
   //6     4
}

void avlTree::doubleRotation(avlNode*& node, Direction dir)
{
    int opposite = this->opposite(dir); //Sentido da rotação

    avlNode* child = node->subTree[dir]->subTree[opposite];
    node->subTree[dir]->subTree[opposite] = child->subTree[dir];
    child->subTree[dir] = node->subTree[dir];
    node->subTree[dir] = child;
    child = node->subTree[dir];
    node->subTree[dir] = child->subTree[opposite];
    child->subTree[opposite] = node;

    node = child;
}

/** ------------------- Balanceamento ------------------- **/
void avlTree::balancement(avlNode* node, Direction dir)
{
    int opposite = this->opposite(dir);

    int bal = node->subTree[dir]->subTree[opposite]->balFactor;

    if(bal == dir)
    { //Se o balFactor está pesado no mesmo lado
        node->balFactor = EQUAL;
        node->subTree[dir]->balFactor = opposite;
    }

    else if(bal == opposite)
    { //Se o balFactor está pesado no lado oposto
        node->balFactor = dir;
        node->subTree[dir]->balFactor = EQUAL;
    }

    else
    { //O fator de balanceamento está igual
        node->balFactor = node->subTree[dir]->balFactor = EQUAL;
    }

    node->subTree[dir]->subTree[opposite]->balFactor = EQUAL;
}

void avlTree::rebalanceInsert(avlNode*& node, Direction dir, bool &changedHeight)
{
    int opposite = this->opposite(dir); //O elemento foi inserido na esquerda ou na direita

    //Se o fator de balanceamento é igual a direção do local de inserção
    if(node->balFactor == dir)
    {
        if(node->subTree[dir]->balFactor == dir) //Se o fator de balanceamento do nó que receberá o novo elemento for igual à direção
        {                                        //em que ouve a inserção, no caso, o nó foi inserido na maior subárvore
                                                 //Caso 1: Fator de balanceamento no elemento = direção
            node->subTree[dir]->balFactor = 2; //Insere o fator de balanceamento no novo nó
            node->balFactor = EQUAL;    //atualiza o fator de balanceamento

            //Precisa-se de rotação simples
            simpleRotation(node, dir);
        }
        else
        {   //Se o filho está equilibrado ou pendendo para o lado oposto
            balancement(node, dir);
            doubleRotation(node, dir);
        }

        changedHeight = false;
    }

    //Se o fator de balanceamento é igual ao local oposto
    else if(node->balFactor == opposite) //Se o fator de balanceamento é o oposto, indica que o nó foi inserido na
    {                                   //Menor subarvore, e o fator de balanceamento não muda.
        node->balFactor = 2;
        changedHeight = false; //A altura não mudou.
    }

    else //Caso o nó ja estivesse equilibrado e o equilíbrio deslocou
    {
        node->balFactor = dir;
    }
}

void avlTree::rebalanceRemove(avlNode *&node, Direction dir, bool &changedHeight)
{
    Direction opposite = this->opposite(dir);

    if(node->balFactor == dir) //Caso o elemento a ser removido está na subarvore com maior altura
    {
        node->balFactor = EQUAL;//O nó agora está balanceado
    }

    else if(node->balFactor == opposite) //Caso oposto
    {
        if(node->subTree[opposite]->balFactor == opposite)  //Se o filho do outro lado está mais pesado
        {
            node->subTree[opposite]->balFactor = EQUAL;
            node->balFactor = EQUAL;
            simpleRotation(node, opposite);
        }
        else if(node->subTree[opposite]->balFactor == EQUAL) //Se o filho do outro lado esta equilibrado
        {
            node->subTree[opposite]->balFactor = dir;
            simpleRotation(node, opposite);
        }
        else
        {
            balancement(node, opposite);
            doubleRotation(node, opposite);
        }
        changedHeight = false;
    }
    else
    {
        node->balFactor = opposite;
        changedHeight = false;
    }
}


/** ------------------- Funções Auxiliares ------------------- **/
inline Direction avlTree::opposite(Direction dir)
{
        return (dir == RIGHT) ? LEFT : RIGHT;
}

void avlTree::print(int data)
{
    avlNode* current = this->root; //Raiz

    while(current != NULL)
    {
        if(data > current->element)
        {
            current = current->subTree[RIGHT]; //O valor está para a direita
        }
        else if(data < current->element) {
            current = current->subTree[LEFT]; //O valor está para a esquerda
        }
        else
        {
            data = current->element; //Retorna o valor pedido quando achado
            cout << "Element: " << data << endl;
            cout << "- Left :" << current->subTree[LEFT]->element << endl;
            cout << "- Right :" << current->subTree[RIGHT]->element << endl;
            break;
        }
    }
}

void avlTree::printNode()
{
    if(this->root != 0)
    {
        this->printNodeRec(this->root);
    }

    else
    {
        cout << "Árvore Vazia!" << endl;
    }
}

void avlTree::printNodeRec(avlNode *node)
{
    if (node != 0)
    {
        this->printNodeRec(node->subTree[LEFT]);
        cout << "Node element: " << node->element << endl;
        cout << "Balancement Factor: " << node->balFactor << endl;
        if(node->subTree[LEFT] != NULL)
        {
            cout << "- Left: " << node->subTree[LEFT]->element << endl;
        }
        if(node->subTree[RIGHT] != NULL)
        {
            cout << "- Right: " << node->subTree[RIGHT]->element << endl;
        }

        cout << endl;
        this->printNodeRec(node->subTree[RIGHT]);
    }
}
