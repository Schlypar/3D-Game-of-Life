#pragma once
#include <vector>

namespace GoL {

class IndexBuffer {
private:
    static unsigned int id;
    static int count;
    static unsigned int*    all_indices;
    static std::vector<int> index_offsets;

public:
    typedef int Id;
    IndexBuffer();
    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer(IndexBuffer&& other) = delete;
    ~IndexBuffer();

    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other) = delete;

    static void* GetOffset(Id id);
    static Id Register(const unsigned int* indices, int count);
    static void Init();

    static inline unsigned int GetCount(Id index) {
        if (index_offsets.size() == 0) return 0;
        if (index == index_offsets.size() - 1) {
            return count - index_offsets[index];
        }
        return index_offsets[index + 1] - index_offsets[index];
    }
};

}
