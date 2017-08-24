#include <Python.h>
#include <math.h>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>

static PyObject* steroid_system(PyObject* self, PyObject* args)
{
	const char* command;
	int sts;
	if (!PyArg_ParseTuple(args, "s", &command)) return NULL;
	sts = system(command);
	return PyLong_FromLong(sts);
}

static PyObject* steroid_convert(PyObject* self, PyObject* args)
{
	/*Input: string, old and new base in decimal format*/
	char* input;
	unsigned int old_base, new_base;
	/*Output setup: symbols and buffers for strings*/
	char* alphabet="0123456789ABCDEFGHIJKLMNOPQESTUVWXYZ";
	char buffer[255] = {0};
	char* result;

	/*Parsing Arguments*/
	if (!PyArg_ParseTuple(args, "sII", &input, &old_base, &new_base)) return NULL;
	/*Protective statements*/
	if((old_base <= 1)||(old_base > 36)) return NULL;
	if((new_base <= 1)||(new_base > 36)) return NULL;
	//Assertion
	std::cout << input << " to present by base " << new_base << " defined by base " << old_base << std::endl;

	unsigned int i, k, dec=0, pos=1;
	for(i=0; input[i]; i++); //C++ hack: C-based strlen
	for( ; i; i--)
	{ //Positioning
		for(k=0; alphabet[k] != input[i-1] && k < 36; k++);
		dec += k * pos;
		pos *= old_base;
	}
	k = 0;
	while(dec)
	{ //Conversion
		i = dec % new_base;
		buffer[k++] = alphabet[i];
		dec /= new_base;
	}
	result = new char[k+1]; //Buffering
	for(i = 0; k; i++) result[i] = buffer[--k];
	result[i] = 0;
	PyObject* rstr = PyUnicode_DecodeASCII(result, i, NULL);
	delete[] result; //Memory leak protection
	return rstr; //Leaving string to garbage collector
}

static PyMethodDef SteroidMethods[] =
{ //Static structure to define properties
	//for exported functions
	{"system",  steroid_system, METH_VARARGS, "Executes a shell command."},
	{"convert",  steroid_convert, METH_VARARGS, "Converts a number between notation systems defined with decimal bases"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef steroidmodule =
{ //Module definition structure
	PyModuleDef_HEAD_INIT,
	"steroid",	/* name of module */
	NULL,				/* module documentation, may be NULL */
	-1,					/* size of per-interpreter state of the module,
							or -1 if the module keeps state in global variables. */
   	SteroidMethods
};

PyMODINIT_FUNC PyInit_steroid(void)
{
	return PyModule_Create(&steroidmodule);
}
