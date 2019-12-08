#include <libxml2/libxml/parser.h>
#include <string.h>
#include <stdio.h>



static xmlSAXHandler handler;
static int           elem_courant;

void caracteres (void *user_data, const xmlChar *text, int length)
{
	printf ("caracteres:text:%s:\n", (char *)text);
}

void fin_element (void *user_data, const xmlChar *name)
{
	printf ("fin de l’element:%s:\n", (char *)name);
}

void debut_element (void *user_data, const xmlChar *name, const xmlChar **attrs)
{
	printf ("Debut de l’element:%s:\n", name);
}

int main (int argc, char *argv[])
{
	printf("Bonjour\n");
    handler.startElement = debut_element;
	handler.endElement   = fin_element;
	handler.characters   = caracteres;
	if (xmlSAXUserParseFile (&handler, NULL, "process1.xml") < 0) { perror ("oups parser"); }
	return 0;
}
