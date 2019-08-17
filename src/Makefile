
VPATH = ./ ../include

YFLAGS = --debug
CPPFLAGS = -g -I ../include 
SOURCES = main.c obj_reader_func.c obj_reader_flex.c obj_reader_bison.tab.c obj_reader_flex.c

%.ln: %.y
%.c: %.y
.y.c:


obj_reader: $(subst .c,.o,$(SOURCES))
	$(CC) -D DEBUG -g -o $@ $^

obj_reader_bison.tab.h: obj_reader_bison.y
	bison $(YFLAGS) -d $< 

main.o: main.c obj_reader.h
obj_reader_func.o: obj_reader_func.c obj_reader.h
obj_reader_flex.o: obj_reader_flex.c
obj_reader_bison.tab.o: obj_reader_bison.tab.c

obj_reader_flex.c: obj_reader_flex.l
obj_reader_bison.tab.c: obj_reader_bison.y



include $(subst .c,.d,$(SOURCES))

%.d: %.c
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

