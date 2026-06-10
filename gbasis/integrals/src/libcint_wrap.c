

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>
#include <string.h>

/* Forward declarations for libcint functions */
extern int int1e_ovlp_sph(double *out, int *dims, int *shls, int *atm, int natm,
                           int *bas, int nbas, double *env, void *opt, double *cache);
extern int int1e_kin_sph(double *out, int *dims, int *shls, int *atm, int natm,
                          int *bas, int nbas, double *env, void *opt, double *cache);
extern int int1e_nuc_sph(double *out, int *dims, int *shls, int *atm, int natm,
                          int *bas, int nbas, double *env, void *opt, double *cache);

/* Macro to generate wrapper functions */
#define DEFINE_INTEGRAL_INT1e(func_name, libcint_func)                        \
static PyObject *                                                              \
func_name(PyObject *self, PyObject *args)                                      \
{                                                                              \
    PyObject *out_obj, *dims_obj, *shls_obj, *atm_obj, *bas_obj, *env_obj;    \
    int natm, nbas;                                                            \
    if (!PyArg_ParseTuple(args, "OOOOiOiO",                                   \
                          &out_obj, &dims_obj, &shls_obj,                      \
                          &atm_obj, &natm, &bas_obj, &nbas, &env_obj))         \
        return NULL;                                                           \
    double *out   = (double *)PyLong_AsVoidPtr(out_obj);                       \
    int    *dims  = (int *)PyLong_AsVoidPtr(dims_obj);                         \
    int    *shls  = (int *)PyLong_AsVoidPtr(shls_obj);                         \
    int    *atm   = (int *)PyLong_AsVoidPtr(atm_obj);                          \
    int    *bas   = (int *)PyLong_AsVoidPtr(bas_obj);                          \
    double *env   = (double *)PyLong_AsVoidPtr(env_obj);                       \
    int result = libcint_func(out, dims, shls, atm, natm,                      \
                              bas, nbas, env, NULL, NULL);                     \
    return PyLong_FromLong(result);                                            \
}

DEFINE_INTEGRAL_INT1e(overlap_sph,  int1e_ovlp_sph)
DEFINE_INTEGRAL_INT1e(kinetic_sph,  int1e_kin_sph)
DEFINE_INTEGRAL_INT1e(nuclear_sph,  int1e_nuc_sph)

static PyMethodDef LibcintMethods[] = {
    {"overlap_sph",  overlap_sph,  METH_VARARGS, "Overlap integral (spherical)"},
    {"kinetic_sph",  kinetic_sph,  METH_VARARGS, "Kinetic energy integral (spherical)"},
    {"nuclear_sph",  nuclear_sph,  METH_VARARGS, "Nuclear attraction integral (spherical)"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef libcintmodule = {
    PyModuleDef_HEAD_INIT,
    "libcint_bindings",
    NULL,
    -1,
    LibcintMethods
};

PyMODINIT_FUNC
PyInit_libcint_bindings(void)
{
    return PyModule_Create(&libcintmodule);
}