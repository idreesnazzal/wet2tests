import os
import argparse

from enum import Enum

class StatusType(Enum):
    SUCCESS = 0
    FAILURE = -1
    ALLOCATION_ERROR = -2
    INVALID_INPUT = -3

class GenreManager:
    def __init__(self):
        self.song_count = {}  # genre -> song count

    def add_genre(self, gid):
        if gid <= 0:
            return "INVALID_INPUT"
        if gid in self.song_count:
            return "FAILURE"
        self.song_count[gid] = 0
        return "SUCCESS"

    def merge_genre(self, gid3):
        if gid3 in self.song_count:
            return "FAILURE"
        self.song_count[gid3] = 0
        return "SUCCESS"

    def increment_count(self, gid):
        self.song_count[gid] += 1

    def decrement_count(self, gid):
        if gid in self.song_count:
            self.song_count[gid] -= 1

    def get_song_count(self, gid):
        if gid <= 0:
            return "INVALID_INPUT"
        if gid not in self.song_count:
            return "FAILURE"
        return f"SUCCESS, {self.song_count[gid]}"

class SongManager:
    def __init__(self, genre_manager):
        self.song_to_genre = {}         # sid -> gid
        self.genre_changes = {}         # sid -> number of changes
        self.genre_manager = genre_manager

    def add_song(self, sid, gid):
        if sid <= 0 or gid <= 0:
            return "INVALID_INPUT"
        if sid in self.song_to_genre:
            return "FAILURE"
        if gid not in self.genre_manager.song_count:
            return "FAILURE"

        self.song_to_genre[sid] = gid
        self.genre_changes[sid] = 0
        self.genre_manager.increment_count(gid)
        return "SUCCESS"

    def get_song_genre(self, sid):
        if sid <= 0:
            return "INVALID_INPUT"
        if sid not in self.song_to_genre:
            return "FAILURE"
        return f"SUCCESS, {self.song_to_genre[sid]}"

    def get_genre_changes(self, sid):
        if sid <= 0:
            return "INVALID_INPUT"
        if sid not in self.genre_changes:
            return "FAILURE"
        return f"SUCCESS, {self.genre_changes[sid]}"

    def reassign_songs_to_merged_genre(self, old_gids, new_gid):
        for sid, gid in list(self.song_to_genre.items()):
            if gid in old_gids:
                self.song_to_genre[sid] = new_gid
                self.genre_changes[sid] += 1
                self.genre_manager.increment_count(new_gid)
                self.genre_manager.decrement_count(gid)

class DSpotify:
    def __init__(self):
        self.genre_manager = GenreManager()
        self.song_manager = SongManager(self.genre_manager)

    def add_genre(self, gid):
        return self.genre_manager.add_genre(gid)

    def add_song(self, sid, gid):
        return self.song_manager.add_song(sid, gid)

    def merge_genres(self, gid1, gid2, gid3):
        if gid1 <= 0 or gid2 <= 0 or gid3 <= 0:
            return "INVALID_INPUT"
        if gid1 == gid2 or gid1 == gid3 or gid2 == gid3:
            return "INVALID_INPUT"
        if gid1 not in self.genre_manager.song_count or gid2 not in self.genre_manager.song_count:
            return "FAILURE"

        result = self.genre_manager.merge_genre(gid3)
        if result != "SUCCESS":
            return result

        self.song_manager.reassign_songs_to_merged_genre({gid1, gid2}, gid3)
        return "SUCCESS"

    def get_song_genre(self, sid):
        return self.song_manager.get_song_genre(sid)

    def get_number_of_songs_by_genre(self, gid):
        return self.genre_manager.get_song_count(gid)

    def get_number_of_genre_changes(self, sid):
        return self.song_manager.get_genre_changes(sid)


# CamelCase to internal method name mapping
command_map = {
    "addGenre": "add_genre",
    "addSong": "add_song",
    "mergeGenres": "merge_genres",
    "getSongGenre": "get_song_genre",
    "getNumberOfSongsByGenre": "get_number_of_songs_by_genre",
    "getNumberOfGenreChanges": "get_number_of_genre_changes"
}

def run_test_on_emulator(input_lines):
    emulator = DSpotify()
    results = []
    for line in input_lines:
        parts = line.strip().split()
        if not parts:
            continue
        cmd = parts[0]
        args = parts[1:]

        method_name = command_map.get(cmd)
        if method_name is None:
            results.append(f"{cmd}: INVALID_INPUT")
            continue

        try:
            args = list(map(int, args))
        except:
            results.append(f"{cmd}: INVALID_INPUT")
            continue

        try:
            method = getattr(emulator, method_name)
            res = method(*args)
        except Exception:
            res = "INVALID_INPUT"

        results.append(f"{cmd}: {res}")
    return results

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input_dir", required=True)
    parser.add_argument("--output_dir", required=True)
    args = parser.parse_args()

    os.makedirs(args.output_dir, exist_ok=True)

    for fname in os.listdir(args.input_dir):
        if not fname.endswith(".in"):
            continue
        with open(os.path.join(args.input_dir, fname)) as f:
            lines = f.readlines()
        output = run_test_on_emulator(lines)
        out_fname = fname.replace(".in", ".out")
        with open(os.path.join(args.output_dir, out_fname), "w") as fout:
            fout.write("\n".join(output) + "\n")

if __name__ == "__main__":
    main()
