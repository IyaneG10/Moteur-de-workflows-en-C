
#
# Constantes pour la compilation des programmes
#

export CC = gcc
export LD = gcc
export CLIB = ar cq
export CFLAGS = -g -Wall

#
# Constantes liees au projet
#

DIRS= Threads IPC Socket Workflow_admin Workflow_engine

#
# La cible generale
#

all: $(patsubst %, _dir_%, $(DIRS)) 

exec:
	cd Workflow_engine  && ./main

$(patsubst %,_dir_%,$(DIRS)):
	cd $(patsubst _dir_%,%,$@) && make

#
# La cible de nettoyage
#

clean: $(patsubst %, _clean_%, $(DIRS))

$(patsubst %,_clean_%,$(DIRS)):
	cd $(patsubst _clean_%,%,$@) && make clean



clean:
	@echo
	@echo Debut du nettoyage
	for dir in $(DIRS); do\
		$(MAKE) clean -C $$dir;\
	done
	@echo Fin du nettoyage
	@echo
