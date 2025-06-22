//
// Created by idreesnazzal on 15/06/2025.
//

#ifndef DSWET2_UNIONFIND_H

#define DSWET2_UNIONFIND_H
using namespace std;
#define STARTSIZE 1000
#include <memory>
#include "Genre.h"
#include "Song.h"
#include <iostream>
#include <ostream>
template<class T>
class NodeHash{
public:
    T value;
    NodeHash<T>* next;
    NodeHash(T value,NodeHash<T>* next);
};

template<class T>
class hashTable{
public:
    int fullSize;
    int currentSize;
    NodeHash<T>* *arr;
    int hash(int key)const;
    hashTable();
    ~hashTable();
    void insert(T value);
    void resize();
    NodeHash<T>* find(int fullSize)const;

};
///////NodeHash implementation
template<class T>
NodeHash<T>::NodeHash(T value,NodeHash<T>* next) {
    this->value=value;
    if(next){
        this->next= next;
    }
    else{
        this->next =nullptr;
    }
}

////////hashTable implementation
template<class T>
hashTable<T>::hashTable() {
    this->fullSize=STARTSIZE;
    this->currentSize=0;
    this->arr=new NodeHash<T>*[STARTSIZE];
    for(int i=0;i<STARTSIZE;i++)
    {
        this->arr[i]= nullptr;
    }
}
template<class T>
hashTable<T>::~hashTable() {
    for (int i = 0; i < fullSize; i++) {
        NodeHash<T>* current = this->arr[i];
        while (current != nullptr) {
            NodeHash<T>* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] this->arr;
}
template<class T>
int hashTable<T>::hash(int key) const {
    return key % this->fullSize;
}
template<class T>
void hashTable<T>::insert(T value) {
    this->currentSize++;
    if(this->currentSize== this->fullSize)
    {
        this->resize();
    }
    NodeHash<T> *toInsert=new NodeHash<T>(value, nullptr);
    if(this->arr[hash(value->id)]== nullptr)
    {
        this->arr[hash(value->id)]=toInsert;
        return;
    }
    toInsert->next=this->arr[hash(value->id)];
    this->arr[hash(value->id)]=toInsert;
}

template<class T>
void hashTable<T>::resize() {
    int oldSize = this->fullSize;
    this->fullSize *= 2;
    NodeHash<T>** newArr = new NodeHash<T>*[this->fullSize];
    for (int i = 0; i < this->fullSize; i++) {
        newArr[i] = nullptr;
    }
    for (int i = 0; i < oldSize; i++) {
        NodeHash<T>* transfer = this->arr[i];
        while (transfer != nullptr) {
            NodeHash<T>* next = transfer->next;
            int newIndex = hash(transfer->value->id);
            transfer->next = newArr[newIndex];
            newArr[newIndex] = transfer;
            transfer = next;
        }
    }
    delete[] this->arr;
    this->arr = newArr;

}
template<class T>
NodeHash<T>* hashTable<T>::find(int id) const{

    NodeHash<T>*tran sfer= this->arr[hash(id)];
    while(transfer!= nullptr)
    {
        if(transfer->value->id==id)
        {
            return transfer;
        }
        transfer= transfer->next;
    }
    return nullptr;

}
class UnionFind{
public:
    hashTable<shared_ptr<Song>> songs;
    hashTable<shared_ptr<Genre>> genres;
    UnionFind()=default;
    ~UnionFind()=default;



    int find(int songId) {
        shared_ptr<Song> song = songs.find(songId)->value;
        shared_ptr<Song> current = song;
        int diff=0;
        while (current->father) {
            diff+=current->father->numOfChanges-current->joinCounter;
            current = current->father;
        }
        shared_ptr<Song> temp = song;
        while(temp->father!=current && temp->father){
            shared_ptr<Song> temp2=temp->father;
            int c = temp2->numOfChanges-temp->joinCounter;
            temp->numOfChanges+=diff;
            temp->joinCounter=current->numOfChanges;
            diff-=c;
            temp->father=current;
            temp=temp2;
        }
        return current->genre->id;
    }

    void Union(int genreId1,int genreId2,int genreId3){
        NodeHash<std::shared_ptr<Genre>>* genre1=this->genres.find(genreId1);
        NodeHash<std::shared_ptr<Genre>>* genre2=this->genres.find(genreId2);
        genres.insert(make_shared<Genre>(genreId3));
        NodeHash<std::shared_ptr<Genre>>* genre3=this->genres.find(genreId3);
        genre3->value->numOfSongs=genre1->value->numOfSongs+genre2->value->numOfSongs;
        if(!genre1->value->numOfSongs && !genre2->value->numOfSongs ) return;
        
        if(genre1->value->numOfSongs ==0 && genre2->value->numOfSongs) {
            genre2->value->root->genre = genre3->value;
            genre3->value->root= genre2->value->root;
            genre2->value->root = nullptr;
            genre3->value->root->numOfChanges++;
        }

        else if(genre1->value->numOfSongs && genre2->value->numOfSongs ==0) {
            genre1->value->root->genre = genre3->value;
            genre3->value->root= genre1->value->root;
            genre1->value->root = nullptr;
            genre3->value->root->numOfChanges++;
        }

        else if(genre1->value->numOfSongs>=genre2->value->numOfSongs){
            genre2->value->root->joinCounter=genre1->value->root->numOfChanges++;
            genre1->value->root->genre=genre3->value;
            genre2->value->root->genre= nullptr;
            genre2->value->root->father=genre1->value->root;
            genre3->value->root=genre1->value->root;
            genre1->value->root= nullptr;
            genre2->value->root= nullptr;

        }
        else{
            genre1->value->root->joinCounter=genre2->value->root->numOfChanges++;
            genre2->value->root->genre=genre3->value;
            genre1->value->root->genre= nullptr;
            genre1->value->root->father=genre2->value->root;
            genre3->value->root=genre2->value->root;
            genre2->value->root= nullptr;
            genre1->value->root= nullptr;
        }
        genre1->value->numOfSongs=0;
        genre2->value->numOfSongs=0;

    }
};



////////hashTable implementation

#endif //DSWET2_UNIONFIND_H
