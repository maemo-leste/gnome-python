/* -*- Mode: C; c-basic-offset: 4 -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include <libgnome/gnome-init.h>

void pygnome_register_classes (PyObject *d);
void pygnome_add_constants (PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pygnome_functions[];

void
pygnome_add_defined_constants(PyObject *dict)
{
    PyDict_SetItemString(dict, "PARAM_NONE", Py_None);
    Py_INCREF(Py_None);

    PyDict_SetItemString(dict, "PARAM_POPT_TABLE", PyString_FromString(GNOME_PARAM_POPT_TABLE));
    PyDict_SetItemString(dict, "PARAM_POPT_FLAGS", PyString_FromString(GNOME_PARAM_POPT_FLAGS));
    PyDict_SetItemString(dict, "PARAM_POPT_CONTEXT", PyString_FromString(GNOME_PARAM_POPT_CONTEXT));
    PyDict_SetItemString(dict, "PARAM_CREATE_DIRECTORIES", PyString_FromString(GNOME_PARAM_CREATE_DIRECTORIES));
    PyDict_SetItemString(dict, "PARAM_ENABLE_SOUND", PyString_FromString(GNOME_PARAM_ENABLE_SOUND));
    PyDict_SetItemString(dict, "PARAM_ESPEAKER", PyString_FromString(GNOME_PARAM_ESPEAKER));
    PyDict_SetItemString(dict, "PARAM_APP_ID", PyString_FromString(GNOME_PARAM_APP_ID));
    PyDict_SetItemString(dict, "PARAM_APP_VERSION", PyString_FromString(GNOME_PARAM_APP_VERSION));
    PyDict_SetItemString(dict, "PARAM_GNOME_PREFIX", PyString_FromString(GNOME_PARAM_GNOME_PREFIX));
    PyDict_SetItemString(dict, "PARAM_GNOME_SYSCONFDIR", PyString_FromString(GNOME_PARAM_GNOME_SYSCONFDIR));
    PyDict_SetItemString(dict, "PARAM_GNOME_DATADIR", PyString_FromString(GNOME_PARAM_GNOME_DATADIR));
    PyDict_SetItemString(dict, "PARAM_GNOME_LIBDIR", PyString_FromString(GNOME_PARAM_GNOME_LIBDIR));
    PyDict_SetItemString(dict, "PARAM_APP_PREFIX", PyString_FromString(GNOME_PARAM_APP_PREFIX));
    PyDict_SetItemString(dict, "PARAM_APP_SYSCONFDIR", PyString_FromString(GNOME_PARAM_APP_SYSCONFDIR));
    PyDict_SetItemString(dict, "PARAM_APP_DATADIR", PyString_FromString(GNOME_PARAM_APP_DATADIR));
    PyDict_SetItemString(dict, "PARAM_APP_LIBDIR", PyString_FromString(GNOME_PARAM_APP_LIBDIR));
    PyDict_SetItemString(dict, "PARAM_HUMAN_READABLE_NAME", PyString_FromString(GNOME_PARAM_HUMAN_READABLE_NAME));
    PyDict_SetItemString(dict, "PARAM_GNOME_PATH", PyString_FromString(GNOME_PARAM_GNOME_PATH));

    PyDict_SetItemString(dict, "POPT_ARGFLAG_OR", PyInt_FromLong(POPT_ARGFLAG_OR));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_AND", PyInt_FromLong(POPT_ARGFLAG_AND));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_XOR", PyInt_FromLong(POPT_ARGFLAG_XOR));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_NOT", PyInt_FromLong(POPT_ARGFLAG_NOT));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_NOR", PyInt_FromLong(POPT_ARGFLAG_NOR));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_NAND", PyInt_FromLong(POPT_ARGFLAG_NAND));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_LOGICALOPS", PyInt_FromLong(POPT_ARGFLAG_LOGICALOPS));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_ONEDASH", PyInt_FromLong(POPT_ARGFLAG_ONEDASH));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_DOC_HIDDEN", PyInt_FromLong(POPT_ARGFLAG_DOC_HIDDEN));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_STRIP", PyInt_FromLong(POPT_ARGFLAG_STRIP));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_OPTIONAL", PyInt_FromLong(POPT_ARGFLAG_OPTIONAL));
    PyDict_SetItemString(dict, "POPT_ARGFLAG_SHOW_DEFAULT", PyInt_FromLong(POPT_ARGFLAG_SHOW_DEFAULT));
    PyDict_SetItemString(dict, "POPT_CONTEXT_NO_EXEC", PyInt_FromLong(POPT_CONTEXT_NO_EXEC));
    PyDict_SetItemString(dict, "POPT_CONTEXT_KEEP_FIRST", PyInt_FromLong(POPT_CONTEXT_KEEP_FIRST));
    PyDict_SetItemString(dict, "POPT_CONTEXT_POSIXMEHARDER", PyInt_FromLong(POPT_CONTEXT_POSIXMEHARDER));
    PyDict_SetItemString(dict, "POPT_CONTEXT_ARG_OPTS", PyInt_FromLong(POPT_CONTEXT_ARG_OPTS));
}


static void
check_pygtk_version(void)
{
    PyObject *m, *d, *pygtk_version, *pygtk_required_version;
    
    m = PyImport_AddModule("gobject");
    d = PyModule_GetDict(m);
    pygtk_version = PyDict_GetItemString(d, "pygtk_version");
    pygtk_required_version = Py_BuildValue("(iii)",
                                           PYGTK_REQUIRED_MAJOR_VERSION,
                                           PYGTK_REQUIRED_MINOR_VERSION,
                                           PYGTK_REQUIRED_MICRO_VERSION);
    if (PyObject_Compare(pygtk_version, pygtk_required_version) < 0) {
        g_error("PyGTK %s required, but %s found.",
                PyString_AsString(PyObject_Repr(pygtk_required_version)),
                PyString_AsString(PyObject_Repr(pygtk_version)));
    }
    Py_DECREF(pygtk_required_version);
    return;
}


DL_EXPORT(void)
init_gnome (void)
{
    PyObject *m, *d, *tuple;

    init_pygobject();
    check_pygtk_version();

    m = Py_InitModule("_gnome", pygnome_functions);
    d = PyModule_GetDict(m);

    pygnome_register_classes(d);
    pygnome_add_constants(m, "GNOME_");
    pygnome_add_defined_constants(d);

    /* gnome-python version */
    tuple = Py_BuildValue("(iii)", GNOME_PYTHON_MAJOR_VERSION, GNOME_PYTHON_MINOR_VERSION, 
                          GNOME_PYTHON_MICRO_VERSION);
    PyDict_SetItemString(d, "gnome_python_version", tuple);
    Py_DECREF(tuple);

    if (!gnome_program_module_registered(LIBGNOME_MODULE))
	gnome_program_module_register(LIBGNOME_MODULE);
}
