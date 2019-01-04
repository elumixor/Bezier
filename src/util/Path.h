//
// Created by Vladyslav Yazykov on 03/01/2019.
//

#ifndef BEZIER_PATH_H
#define BEZIER_PATH_H

/// Simplified double linked list structure
template<typename T>
struct Path {
    struct Node {
        Node *previous{nullptr}, *next{nullptr};
        const T *point{nullptr};

        Node(Node *previous, const T *point, Node *next = nullptr) : previous{previous}, next{next}, point{point} {}
    };

    Node *root{nullptr};
    Node *end{nullptr};

    void add(const T *point) {
        if (!root) root = end = new Node{root, point};
        else {
            end->next = new Node{end, point};
            end->next->previous = end;
            end = end->next;
        }
    }

    void remove(const )

    virtual ~Path() {
        for (Node *p{root}; p;) {
            Node *next{p->next};
            delete p;
            p = next;
        }
    }
};

#endif //BEZIER_PATH_H
