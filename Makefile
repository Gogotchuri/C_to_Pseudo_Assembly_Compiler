#Makefile for c->pseudo assembly compiler
CC = gcc
CFLAGS = -g -Wall -std=gnu99 -Wpointer-arith
LDFLAGS =
PURIFY = purify
PFLAGS=  -demangle-program=/usr/pubsw/bin/c++filt -linker=/usr/bin/ld -best-effort  

VECTOR_SRCS = vector.c
VECTOR_HDRS = $(VECTOR_SRCS:.c=.h)

HASHSET_SRCS = $(VECTOR_SRCS) hashset.c
HASHSET_HDRS = $(HASHSET_SRCS:.c=.h)

PAIR_SRCS = pair.c
PAIR_HDRS = $(PAIR_SRCS:.c=.h)

NODE_SRCS = node.c
NODE_HDRS = $(NODE_SRCS:.c=.h)

TOK_CHECKER_SRCS = tok_checker.c
TOK_CHECKER_HDRS = $(TOK_CHECKER_SRCS:.c=.h)

TOKENIZER_SRCS = $(PAIR_SRCS) $(HASHSET_SRCS) $(TOK_CHECKER_SRCS) tokenizer.c
TOKENIZER_HDRS = $(TOKENIZER_SRCS:.c=.h)

GRAMMAR_SRCS = $(NODE_SRCS) grammar.c
GRAMMAR_HDRS = $(GRAMMAR_SRCS:.c=.h)

PARSER_SRCS = $(TOKENIZER_SRCS) $(GRAMMAR_SRCS) parser.c
PARSER_HDRS = $(PARSER_SRCS:.c=.h)

GENERATOR_SRCS = $(PARSER_SRCS) generator.c
GENERATOR_HDRS = $(GENERATOR_SRCS:.c=.h)

APPLICATION_SRCS = $(GENERATOR_SRCS) application.c
APPLICATION_OBJS = $(APPLICATION_SRCS:.c=.o)

SRCS = $(APPLICATION_SRCS)
HDRS = $(GENERATOR_HDRS) $(PAIR_HDRS)

EXECUTABLES = application
PURIFY_EXECUTABLES = application-pure

default: $(EXECUTABLES)

pure: $(PURIFY_EXECUTABLES)

application : Makefile.dependencies $(APPLICATION_OBJS)
	$(CC) -o $@ $(APPLICATION_OBJS) $(LDFLAGS)

application-pure : Makefile.dependencies $(APPLICATION_OBJS)
	$(PURIFY) $(PFLAGS) $(CC) -o $@ $(APPLICATION_OBJS) $(LDFLAGS)

#default
Makefile.dependencies:: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -MM $(SRCS) > Makefile.dependencies

-include Makefile.dependencies

clean:
	\rm -fr a.out $(EXECUTABLES) $(PURIFY_EXECUTABLES) *.o core Makefile.dependencies
