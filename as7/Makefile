BIN = Agent Smoker1 Smoker2 Smoker3
GEN = Smoking_clnt.c Smoking_svc.c Smoking.h 
RPCCOM = rpcgen

all: $(BIN)

Smoker1: Smoker1.o Smoking_clnt.o
	gcc -o $@ Smoker1.o Smoking_clnt.o

Smoker2: Smoker2.o Smoking_clnt.o
	gcc -o $@ Smoker2.o Smoking_clnt.o

Smoker3: Smoker3.o Smoking_clnt.o
	gcc -o $@ Smoker3.o Smoking_clnt.o

Smoker1.o: Smoker1.c Smoking.h
	gcc -g Smoker1.c -c

Smoker2.o: Smoker2.c Smoking.h
	gcc -g Smoker2.c -c

Smoker3.o: Smoker3.c Smoking.h
	gcc -g Smoker3.c -c

Agent: Agent.o Smoking_svc.o
	gcc -o $@ Agent.o Smoking_svc.o

Agent.o: Agent.c Smoking.h
	gcc -g Agent.c -c

$(GEN): Smoking.x
	$(RPCCOM) Smoking.x


