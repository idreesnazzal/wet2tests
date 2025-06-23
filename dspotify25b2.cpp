#include "dspotify25b2.h"

class Genre;
class Song;
DSpotify::DSpotify() : UF() {}
DSpotify::~DSpotify() {
    int count =0;
    while(count < UF.genres.fullSize) {
        NodeHash<shared_ptr<Genre>>* temp = UF.genres.arr[count];
        while(temp) {
            if(temp->value->root!=nullptr){
                temp->value->root->genre=nullptr;
                temp->value->root=nullptr;
            }
            temp=temp->next;
        }
        count++;}
}

StatusType DSpotify::addGenre(int genreId) {
    if (genreId <= 0) return StatusType::INVALID_INPUT;
    if (UF.genres.find(genreId)) return StatusType::FAILURE;
    try{
        UF.genres.insert(make_shared<Genre>(genreId));
    }
    catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType DSpotify::addSong(int songId, int genreId) {
    if (songId <= 0 || genreId <= 0) return StatusType::INVALID_INPUT;
    if (UF.songs.find(songId)) return StatusType::FAILURE;
    if (!UF.genres.find(genreId) || UF.songs.find(songId)) return StatusType::FAILURE;
    try {
        UF.songs.insert(make_shared<Song>(songId));
        shared_ptr<Genre> genre = UF.genres.find(genreId)->value;
        genre->numOfSongs++;
        shared_ptr<Song> song = UF.songs.find(songId)->value;
        if(!genre->root) {
            genre->root=song;
            song->genre = genre;
        } else {
            song->father = genre->root;
            song->joinCounter = genre->root->numOfChanges;
        }
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType DSpotify::mergeGenres(int genreId1, int genreId2, int genreId3) {
    if (genreId1 <= 0 || genreId2 <= 0 || genreId3 <= 0
        || genreId1 == genreId2 || genreId2 == genreId3 || genreId1 == genreId3)
        return StatusType::INVALID_INPUT;
    if (!UF.genres.find(genreId1) || !UF.genres.find(genreId2) || UF.genres.find(genreId3)) {
        return StatusType::FAILURE;
    }
    try {
        UF.Union(genreId1, genreId2, genreId3);

    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::getSongGenre(int songId) {
    if (songId <= 0) return StatusType::INVALID_INPUT;
    NodeHash<shared_ptr<Song>>* songNode = UF.songs.find(songId);
    if (!songNode) return StatusType::FAILURE;
    try{
        return UF.find(songId);
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> DSpotify::getNumberOfSongsByGenre(int genreId) {
    if (genreId <= 0) return StatusType::INVALID_INPUT;
    if (!UF.genres.find(genreId)) return StatusType::FAILURE;
    try{
        return UF.genres.find(genreId)->value->numOfSongs;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> DSpotify::getNumberOfGenreChanges(int songId) {
    if (songId <= 0) return StatusType::INVALID_INPUT;
    if (!UF.songs.find(songId)) return StatusType::FAILURE;
    try{
        UF.find(songId);
        if(UF.songs.find(songId)->value->father) {
            return UF.songs.find(songId)->value->numOfChanges + (UF.songs.find(songId)->value->father->numOfChanges - UF.songs.find(songId)->value->joinCounter)+1 ;
        }
        else {
            return UF.songs.find(songId)->value->numOfChanges+1;
        }
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}