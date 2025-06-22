//
// Created by idreesnazzal on 15/06/2025.
//

#ifndef DSWET2_GENRE_H
#define DSWET2_GENRE_H
#include <memory>
#include "Song.h"
class Genre {
 public:
    int id;
    int numOfSongs;
    shared_ptr<Song> root;
    Genre(int id){
        this->id=id;
        numOfSongs=0;
        root= nullptr;
    }
    ~Genre()=default;
    void clear() {
        root = nullptr;
        numOfSongs = 0;
    }
};


#endif //DSWET2_GENRE_H
