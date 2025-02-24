#include <Python.h>
#include <iostream>

int main() {
    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('src/')");
    PyObject *pName = PyUnicode_DecodeFSDefault("repl.repl");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, "start");
        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString("World"));
            PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != nullptr) {
                std::cout << PyUnicode_AsUTF8(pValue) << std::endl;
                Py_DECREF(pValue);
            } else {
                PyErr_Print();
                std::cerr << "Call failed" << std::endl;
            }
            Py_DECREF(pFunc);
        }
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load \"hello\"" << std::endl;
    }
    Py_Finalize();
    return 0;
}
