

#ifndef DSPOTIFY25SPRING_WET2_H_
#define DSPOTIFY25SPRING_WET2_H_

#include "wet2util.h"
#include "UnionFind.h"
#include "Genre.h"
#include "Song.h"
class DSpotify {

public:
    UnionFind UF;

    // <DO-NOT-MODIFY> {
    DSpotify();

    virtual ~DSpotify();

    StatusType addGenre(int genreId);

    StatusType addSong(int songId, int genreId);

    StatusType mergeGenres(int genreId1, int genreId2, int genreId3);

    output_t<int> getSongGenre(int songId);

    output_t<int> getNumberOfSongsByGenre(int genreId);

    output_t<int> getNumberOfGenreChanges(int songId);
    // } </DO-NOT-MODIFY>
};

#endif // DSPOTIFY25SPRING_WET2_H_