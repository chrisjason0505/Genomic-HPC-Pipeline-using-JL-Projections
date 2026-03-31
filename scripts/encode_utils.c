#include <Python.h>
#include <stdio.h>

__declspec(dllexport) void encode_snps(const char* ref_alleles, const char* geno_matrix, float* output, int n_snps, int n_samples) {
    for (int i = 0; i < n_snps; i++) {
        char ref = ref_alleles[i]; 
        for (int j = 0; j < n_samples; j++) {
            int gt_idx = (i * n_samples + j) * 2;
            char g1 = geno_matrix[gt_idx];
            char g2 = geno_matrix[gt_idx + 1];
            
            int out_idx = i * n_samples + j;
            
            if (g1 == 'N' && g2 == 'N') {
                output[out_idx] = 1.0f;
            } else {
                int c = 0;
                if (g1 == ref) c++;
                if (g2 == ref) c++;
                
                if (c == 2) {
                    output[out_idx] = 0.0f;
                } else if (c == 1) {
                    output[out_idx] = 1.0f;
                } else {
                    output[out_idx] = 2.0f;
                }
            }
        }
    }
}

// Dummy python init block so setuptools creates the .pyd without error
static PyMethodDef DummyMethods[] = {
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef dummymodule = {
    PyModuleDef_HEAD_INIT,
    "encode_utils_compiled",
    NULL,
    -1,
    DummyMethods
};

PyMODINIT_FUNC PyInit_encode_utils_compiled(void) {
    return PyModule_Create(&dummymodule);
}
