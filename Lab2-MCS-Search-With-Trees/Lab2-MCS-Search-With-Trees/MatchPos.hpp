#include <unordered_set> 

struct MatchPos {
    size_t position;
    size_t offset;

    bool operator==(const MatchPos& other) const {
        return position == other.position && offset == other.offset;
    }
};

namespace std {
    template <>
    struct hash<MatchPos> {
        size_t operator()(const MatchPos& mp) const {
            return hash<size_t>()(mp.position) ^ (hash<size_t>()(mp.offset) << 1);
        }
    };
}
