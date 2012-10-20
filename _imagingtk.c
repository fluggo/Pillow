/*
 * The Python Imaging Library.
 *
 * tkinter hooks
 *
 * history:
 * 99-07-26 fl	created
 * 99-08-15 fl	moved to its own support module
 *
 * Copyright (c) Secret Labs AB 1999.
 *
 * See the README file for information on usage and redistribution.
 */


#include "Python.h"
#if PY_MAJOR_VERSION >= 3
#define IS_PY3K
#ifndef DL_EXPORT
#   define DL_EXPORT(RTYPE) RTYPE
#endif
#endif
#include "Imaging.h"

#include "tk.h"

/* must link with Tk/tkImaging.c */
extern void TkImaging_Init(Tcl_Interp* interp);

/* copied from _tkinter.c (this isn't as bad as it may seem: for new
   versions, we use _tkinter's interpaddr hook instead, and all older
   versions use this structure layout) */

typedef struct {
    PyObject_HEAD
    Tcl_Interp* interp;
} TkappObject;

static PyObject* 
_tkinit(PyObject* self, PyObject* args)
{
    Tcl_Interp* interp;

    long arg;
    int is_interp;
    if (!PyArg_ParseTuple(args, "li", &arg, &is_interp))
        return NULL;

    if (is_interp)
        interp = (Tcl_Interp*) arg;
    else {
        TkappObject* app;
	/* Do it the hard way.  This will break if the TkappObject
	   layout changes */
        app = (TkappObject*) arg;
        interp = app->interp;
    }

    /* This will bomb if interp is invalid... */
    TkImaging_Init(interp);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef functions[] = {
    /* Tkinter interface stuff */
    {"tkinit", (PyCFunction)_tkinit, 1},
    {NULL, NULL} /* sentinel */
};


/*----------------------------------------------------------------------------*/
struct module_state {
    PyObject *error;
};

#ifdef IS_PY3K
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))

static int traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "_imagingtk",
        NULL,
        sizeof(struct module_state),
        functions,
        NULL,
        traverse,
        clear,
        NULL
};

PyObject *
PyInit__imagingtk(void)
{
    PyObject *m;
    PyObject *d;
    PyObject *v;
    int major, minor, patch;

    m = PyModule_Create(&moduledef);
    d = PyModule_GetDict(m);
    if (m == NULL)
        return NULL;
    struct module_state *st = GETSTATE(m);

    st->error = PyErr_NewException("_imagingtk.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;

DL_EXPORT(void)
init_imagingtk(void)
{
    Py_InitModule("_imagingtk", functions);
}
#endif
