//
// Created by idreesnazzal on 15/06/2025.
//
// Created by idreesnazzal on 15/06/2025.
#pragma once
class Genre;
#include <memory>
#ifndef DSWET2_SONG_H
#define DSWET2_SONG_H
class Song {
    public:
    int id;
    int joinCounter;
    int numOfChanges;
    shared_ptr<Genre> genre;
    shared_ptr<Song> father;
    Song(int id){
        this->id=id;
        genre= nullptr;
        father= nullptr;
        joinCounter=0;
        numOfChanges=0;
    }
    bool IsRoot()const{
        return genre!=nullptr;
    }
    ~Song()=default;



};


#endif //DSWET2_SONG_H
