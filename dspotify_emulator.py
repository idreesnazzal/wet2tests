import os
import argparse
from enum import Enum

class StatusType(Enum):
    SUCCESS = 0
    FAILURE = -1
    ALLOCATION_ERROR = -2
    INVALID_INPUT = -3

class Output:
    def __init__(self, status, value=None):
        self.status = status
        self.value = value

class DSpotify:
    def __init__(self):
        self.genre_parent = {}      # Union-Find
        self.genre_song_count = {}
        self.song_to_genre = {}
        self.song_genre_changes = {}

    def find_genre(self, gid):
        if self.genre_parent[gid] != gid:
            self.genre_parent[gid] = self.find_genre(self.genre_parent[gid])
        return self.genre_parent[gid]

    def add_genre(self, gid):
        if gid <= 0:
            return "INVALID_INPUT"
        if gid in self.genre_parent:
            return "FAILURE"
        self.genre_parent[gid] = gid
        self.genre_song_count[gid] = 0
        return "SUCCESS"

    def add_song(self, sid, gid):
        if sid <= 0 or gid <= 0:
            return "INVALID_INPUT"
        if sid in self.song_to_genre:
            return "FAILURE"
        if gid not in self.genre_parent:
            return "FAILURE"
        root_gid = self.find_genre(gid)
        self.song_to_genre[sid] = root_gid
        self.song_genre_changes[sid] = 1
        self.genre_song_count[root_gid] += 1
        return "SUCCESS"

    def merge_genres(self, gid1, gid2, gid3):
        if gid1 <= 0 or gid2 <= 0 or gid3 <= 0:
            return "INVALID_INPUT"
        if gid1 == gid2 or gid1 == gid3 or gid2 == gid3:
            return "INVALID_INPUT"
        if gid1 not in self.genre_parent or gid2 not in self.genre_parent:
            return "FAILURE"
        if gid3 in self.genre_parent:
            return "FAILURE"

        root1 = self.find_genre(gid1)
        root2 = self.find_genre(gid2)
        self.genre_parent[gid3] = gid3
        self.genre_song_count[gid3] = 0

        for sid in self.song_to_genre:
            genre = self.find_genre(self.song_to_genre[sid])
            if genre in {root1, root2}:
                self.song_genre_changes[sid] += 1
                self.song_to_genre[sid] = gid3
                self.genre_song_count[gid3] += 1

        self.genre_song_count[root1] = 0
        self.genre_song_count[root2] = 0
        self.genre_parent[root1] = gid3
        self.genre_parent[root2] = gid3

        return "SUCCESS"

    def get_song_genre(self, sid):
        if sid <= 0:
            return "INVALID_INPUT"
        if sid not in self.song_to_genre:
            return "FAILURE"
        root = self.find_genre(self.song_to_genre[sid])
        return f"SUCCESS, {root}"

    def get_number_of_songs_by_genre(self, gid):
        if gid <= 0:
            return "INVALID_INPUT"
        if gid not in self.genre_parent:
            return "FAILURE"
        root = self.find_genre(gid)
        return f"SUCCESS, {self.genre_song_count.get(root, 0)}"

    def get_number_of_genre_changes(self, sid):
        if sid <= 0:
            return "INVALID_INPUT"
        if sid not in self.song_genre_changes:
            return "FAILURE"
        return f"SUCCESS, {self.song_genre_changes[sid]}"

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
