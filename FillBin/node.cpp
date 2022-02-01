#include "node.h"

Node::Node()
{
    m_package = nullptr;
    m_child[0] = nullptr;
    m_child[1] = nullptr;

}

Node * Node::insert(Package *package)
{
    //if we're not a leaf then
    if (m_child[0] != nullptr && m_child[1] != nullptr){
        Node *node = m_child[0]->insert(package);
        if (!node)
            node = m_child[1]->insert(package);
        return node;
    }
    else{
        // there is already package here
        if (m_package != nullptr)
            return nullptr;

        // package does not fit to node
        if (package->width() > m_rect.width() || package->height() > m_rect.height())
            return nullptr;

        // package ideally fits to node
        if (package->width() == m_rect.width() && package->height() == m_rect.height()){
            m_package = package;
            return this;
        }

        // split node to create free and occupied childs

        // check which way to split
        int dw = m_rect.width() - package->width();
        int dh = m_rect.height() - package->height();

        m_child[0] = new Node;
        m_child[1] = new Node;

        int x1, y1, x2, y2;
        m_rect.getCoords(&x1, &y1, &x2, &y2);
 
        if (dw > dh){
            m_child[0]->setRect(QRect(x1, y1, package->width(), m_rect.height()));
            m_child[1]->setRect(QRect(x1+package->width(), y1, m_rect.width()-package->width(), m_rect.height()));
        }
        else{
            m_child[0]->setRect(QRect(x1, y2-package->height(), m_rect.width(), package->height()));
            m_child[1]->setRect(QRect(x1, y1, m_rect.width(), m_rect.height()-package->height()));
        }

        qDebug() << m_child[0]->getRect() << m_child[1]->getRect();

        // insert into first child we created
        return m_child[0]->insert(package);
    }
}
