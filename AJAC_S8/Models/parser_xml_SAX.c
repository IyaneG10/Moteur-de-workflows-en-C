#include <libxml/parser.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// compilre avec gcc -Wall parser_xml_SAX.c -I/usr/include/libxml2/ -lxml2
//---> xml2-config --libs
//---> xml2-config --flags



static xmlSAXHandler handler;
//static int           elem_courant;
bool debut=false;
bool fin=false;

void caracteres (void *user_data, const xmlChar *text, int length)
{
	
    if(debut==true && fin==true)
    {
        printf ("caracteres:text:%s:\n", (char *)text);
    }
    
}

void fin_element (void *user_data, const xmlChar *name)
{
	

    if(strcmp((const char*)name,"tns:activity")==0)
    {fin=true;
        printf ("fin de l’element:%s:\n", (char *)name);
    }
}

void debut_element (void *user_data, const xmlChar *name, const xmlChar **attrs)
{

    
    if(strcmp((const char*)name,"tns:activity")==0)
        
    {debut=true;
        printf ("Debut de l’element:%s:\n", (char *)name);
    }

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








/*
#include <libxml/parser.h>

static xmlSAXHandler my_handler {
    ...
};

struct ParserState {
    RetVal return_val;
    StatesEnum state;
    ...
};

RetVal
parse_xml_file(const char *filename) {
    struct ParserState my_state;
    
    if (xmlSAXUserParseFile(&my_handler, &my_state, filename) < 0) {
        free_ret_val(my_state.return_val);
        return NULL;
    } else
        return my_state.return_val;
}
*/
