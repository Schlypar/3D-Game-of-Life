float4 productVecAndMat4(float* vec, float* mat) {
    float res[4] = { 0 };

    for (uint i = 0; i < 4; i++) {
        for (uint j = 0; j < 4; j++) {
            res[i] += mat[i + j * 4] * vec[j];
        }
    }

    return vload4(0, res);
}

__kernel void calculateModelMatrices(
        __global float* vbo,
        __global float16* matrices,
        __global float* vertices,
        uint clVertexSize
) {

    uint vertexIndex = get_global_id(0);
    uint modelIndex = (uint) vertices[vertexIndex * clVertexSize + 6];
    uint vboVertexSize = clVertexSize - 1;

    float mat[16];
    vstore16(matrices[modelIndex], 0, mat);

    float vec[4] = { 0, 0, 0, 1 };
    for (uint j = 0; j < 3; j++) {
        vec[j] = vertices[vertexIndex * clVertexSize + j];
    }

    float4 res = productVecAndMat4(vec, mat);
    vstore4(res, 0, vec);

    for (uint j = 0; j < 3; j++) {
        vbo[vertexIndex * vboVertexSize + j] = vec[j];
    }
    for (uint j = 3; j < vboVertexSize; j++) {
        vbo[vertexIndex * vboVertexSize + j] = modelIndex;
    }
}