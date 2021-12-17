//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#ifndef STUDY_COMPILER_UTIL_H
#define STUDY_COMPILER_UTIL_H

#include <functional>

// Inserts the key-value pair into the collection. Dies if key was already
// present.
template <class Collection, class Key, class Value>
void InsertOrDie(Collection* const collection, Key&& key, Value&& value) {
    auto p = collection->insert(std::make_pair(std::forward<Key>(key), std::forward<Value>(value)));
    CHECK(p.second) << "duplicate key: " << key;
}

// Returns true if and only if the given collection contains the given key.
template <class Collection, class Key>
bool containsKey(const Collection& collection, const Key& key) {
    return collection.find(key) != collection.end();
}

// Inserts `value` into `set`. Dies if it was already present.
template <class Set, class Value>
void InsertOrDie(Set* const set, Value&& value) {
    CHECK(set->insert(std::forward<Value>(value)).second) << "duplicate value: " << value;
}

#endif //STUDY_COMPILER_UTIL_H