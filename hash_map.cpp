//
// Created by ilnar on 16.04.16.
//

#include <assert.h>
#include "hash_map.h"

hash_map::hash_map() {
    data.resize(MAX_N + 1);
    size_ = 0;
}

hash_map::~hash_map() {
}


size_t hash_map::hash(key_type type) {
    unsigned res = 0x811c9dc5;
    for (int i = 0; i < type.size(); i++) {
        res *= 0x01000193;
        res ^= type[i];
    }
    return res % MAX_N;
}

hash_map::hash_map(const hash_map &map) {
    this->MAX_N = map.MAX_N;
    this->data = map.data;
    this->size_ = map.size_;
}

hash_map &hash_map::operator=(const hash_map &map) {
    for (auto e : iters) {
        e->it_map = NULL;
    }
    iters.clear();
    size_ = map.size_;
    data = map.data;
    MAX_N = map.MAX_N;
    return *this;
}

bool hash_map::isEmpty() const {
    return size() == 0;
}

size_t hash_map::size() const {
    return size_;
}

hash_map::iterator hash_map::begin() {
    for (size_t i = 0; i < MAX_N; i++) {
        if (data[i].size() != 0) {
            return iterator(i, data[i].begin(), this);
        }
    }
    return hash_map::iterator(MAX_N, data[MAX_N].begin(), this);
}

hash_map::iterator hash_map::end() {
    return iterator(MAX_N, data[MAX_N].begin(), this);
}

hash_map::iterator hash_map::insert(const value_type &type) {
    if (size() == MAX_N) {
        for (auto it : iters) {
            it->invalidate();
        }
        std::vector<std::list<value_type> > new_data(2 * MAX_N + 1);
        MAX_N *= 2;
        for (int i = 0; i < MAX_N >> 1; i++) {
            for (auto it = data[i].begin(); it != data[i].end(); it++) {
                new_data[hash(it->first)].push_back(*it);
            }
        }
        data = new_data;
    }
    assert(this->find(type.first) == this->end());
    size_t ha = hash(type.first);
    data[ha].push_back(type);
    size_++;
    return iterator(ha, --(data[ha].end()), this);
}

hash_map::iterator hash_map::erase(const key_type &type) {
    iterator it = hash_map::find(type);
    return hash_map::erase(it);
}

hash_map::iterator hash_map::erase(iterator &iterator) {
    assert(iterator.it_map);
    assert(iterator != this->end());
    hash_map::iterator res = iterator;
    res++;
    for (auto it = data[iterator.it_pos].begin(); it != data[iterator.it_pos].end(); it++) {
        if (iterator.it_node->first == it->first) {
            data[iterator.it_pos].erase(it);
            iterator.invalidate();
            return res;
        }
    }
    return res;
}

hash_map::iterator hash_map::find(const key_type &type) {
    size_t ha = hash(type);
    for (std::list<value_type>::iterator it = data[ha].begin(); it != data[ha].end(); it++) {
        if ((*it).first == type) {
            return iterator(ha, it, this);
        }
    }
    return hash_map::iterator(MAX_N, data[MAX_N].begin(), this);
}

hash_map::iterator::iterator() : it_pos(0), it_node(NULL), it_map(NULL) {
}

hash_map::iterator::~iterator() {
    invalidate();
}

hash_map::iterator::iterator(size_t it_pos, std::list<value_type>::const_iterator type, hash_map *map) : it_pos(it_pos),
                                                                                                         it_node(type),
                                                                                                         it_map(map) {
    it_map->iters.insert(this);
}

hash_map::iterator::iterator(const iterator &iterator) : it_pos(iterator.it_pos), it_node(iterator.it_node),
                                                         it_map(iterator.it_map) {
    it_map->iters.insert(this);
}

hash_map::iterator &hash_map::iterator::operator=(const hash_map::iterator &iterator) {
    if (it_map) {
        it_map->iters.erase(this);
    }
    it_pos = iterator.it_pos;
    it_node = iterator.it_node;
    it_map = iterator.it_map;
    it_map->iters.insert(this);
    return *this;
}

hash_map::iterator &hash_map::iterator::operator++() {
    assert(it_map);
    assert(it_pos != it_map->MAX_N);
    it_node++;
    while (it_pos < it_map->MAX_N && it_node == it_map->data[it_pos].end()) {
        it_pos++;
        std::list<value_type> *poc = &it_map->data[it_pos];
        it_node = poc->begin();
    }
    return *this;
}

hash_map::iterator hash_map::iterator::operator++(int) {
    iterator old = *this;
    ++*this;
    return old;
}

hash_map::iterator &hash_map::iterator::operator--() {
    assert(it_map);
    assert(!(it_pos == 0 && it_node == it_map->data[it_pos].begin()));
    if (it_node == it_map->data[it_pos].begin()) {
        it_pos--;
        while (it_pos > 0 && it_map->data[it_pos].size() == 0) {
            it_pos--;
        }
        it_node = it_map->data[it_pos].end();
    }
    it_node--;
    return *this;
}

hash_map::iterator hash_map::iterator::operator--(int) {
    iterator old = *this;
    --*this;
    return old;
}

const mapped_type &hash_map::iterator::operator*() const {
    assert(it_map);
    assert(it_pos != it_map->MAX_N);
    return (*it_node).second;
}

const mapped_type *hash_map::iterator::operator->() const {
    assert(it_map);
    assert(it_pos != it_map->MAX_N);
    return &(*it_node).second;
}

void hash_map::iterator::invalidate() {
    if (this->it_map) {
        this->it_map->iters.erase(this);
        this->it_map = NULL;
    }
}

bool operator==(hash_map::iterator const &a, hash_map::iterator const &b) {
    assert(a.it_map);
    assert(a.it_map == b.it_map);
    return (a.it_node == b.it_node);
}

bool operator!=(hash_map::iterator const &a, hash_map::iterator const &b) {
    return !(a == b);
}