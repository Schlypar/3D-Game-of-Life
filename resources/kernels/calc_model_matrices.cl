float4 productVecAndMat4(float* vec, float* mat) {
    float res[4] = {0};

    for (uint i = 0; i < 4; i++) {
        for (uint j = 0; j < 4; j++) {
            res[i] += mat[i * 4 + j] * vec[i];
        }
    }

    return vload4(0, res);
}

__kernel void calculateModelMatrices(
    __global float* vbo,
    __global uint* vertexForMatricesBorders,
    __global float16* matrices,
    __global float* vertices,
    uint vertexSize) {

    uint modelIndex = get_global_id(0);
    uint verticesStart = vertexForMatricesBorders[modelIndex];
    uint verticesEnd = vertexForMatricesBorders[modelIndex + 1];

    float mat[16];
    vstore16(matrices[modelIndex], 0, mat);

    for (uint i = verticesStart; i < verticesEnd; i++) {
        float vec[4] = {0};
        for (uint j = 0; j < 3; j++) {
            vec[j] = vertices[i * vertexSize + j];
        }

        float4 res = productVecAndMat4(vec, mat);
        vstore4(res, 0, vec);

        for (uint j = 0; j < 3; j++) {
            vbo[i * vertexSize + j] = vec[j];
        }
        for (uint j = 3; j < vertexSize; j++) {
            vbo[i * vertexSize + j] = vertices[i * vertexSize + j];
        }
    }

}