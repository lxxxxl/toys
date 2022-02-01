#ifndef NODE_H
#define NODE_H
#include "QRect"
#include "package.h"


class Node
{
public:
    Node();
    Node *insert(Package *package);

    void setRect(QRect rect){m_rect = rect;}
    QRect getRect(){return m_rect;}

private:
    Node *m_child[2];
    QRect m_rect;
    Package *m_package;
};

#endif // NODE_H
