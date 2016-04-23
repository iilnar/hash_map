//
// Created by ilnar on 16.04.16.
//

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <string>
#include <list>
#include <vector>
#include <set>

typedef std::string key_type;
typedef int mapped_type;
typedef std::pair<key_type, mapped_type> value_type;

struct hash_map {
    struct iterator {
    private:
        size_t it_pos;
        std::list<value_type>::const_iterator it_node;
        hash_map *it_map;

        void invalidate();

    public:
        iterator();

        ~iterator();

        iterator(size_t, std::list<value_type>::const_iterator, hash_map *);

        iterator(iterator const &);

        iterator &operator=(iterator const &);

        iterator &operator++();

        iterator operator++(int);

        iterator &operator--();

        iterator operator--(int);

        const mapped_type &operator*() const;

        const mapped_type *operator->() const;

        friend bool operator==(hash_map::iterator const &a, hash_map::iterator const &b);

        friend bool operator!=(hash_map::iterator const &a, hash_map::iterator const &b);

        friend struct hash_map;
    };

    hash_map();

    ~hash_map();

    hash_map(hash_map const &);

    hash_map &operator=(hash_map const &);

    bool isEmpty() const;

    size_t size() const;

    iterator begin();

    iterator end();

    iterator insert(value_type const &);

    iterator erase(key_type const &);

    iterator erase(iterator &);

    iterator find(key_type const &);

private:
    size_t hash(key_type);

    size_t MAX_N = 4;
    std::vector<std::list<value_type> > data;
    size_t size_;
    std::set<iterator *> iters;
};


#endif //HASH_MAP_H
