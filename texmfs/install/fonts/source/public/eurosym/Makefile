RCSFILES=../sty/eurosym.sty \
         fey.mf \
         ../README \
         ../Changes \
         ../doc/testeuro.tex \
         makemfs \
         Makefile
TARDIR=$(HOME)/transfer
VERSION=1.3

SHAPES=bl bo br ml mo mr
FILES=$(addprefix fey, $(addsuffix 10, $(SHAPES)))

MF_FILES=  $(addsuffix .mf, $(FILES))
TFM_FILES= $(addprefix ../tfm/, $(addsuffix .tfm, $(FILES)))

all: mf tfm feymr10.dvi feybr10.dvi ../doc/testeuro.dvi
.PHONY: All

.PRECIOUS: $(TFM_FILES) $(MF_FILES)

%.dvi: %.2602gf
	gftodvi $<

%.2602gf: %.mf fey.mf
	mf $<

feymr10.mf feybr10.mf: makemfs
	./makemfs -mf

../doc/testeuro.dvi: ../doc/testeuro.tex feymr10.mf
	(cd ../doc; latex testeuro.tex)

.PHONY: clean
clean: clean-keep-tfm
	rm -f ../tfm/*.tfm

.PHONY: clean-keep-tfm
clean-keep-tfm:
	rm -f /var/texfonts/tfm/fey*.tfm
	rm -f /var/texfonts/pk/*/fey*.*pk
	rm -f ../doc/fey*.*pk
	rm -f *.log *.aux *.2602gf *.600gf *10.mf *~ *.ps* ../doc/*.ps* ../doc/*.log ../doc/*.aux
	rm -f *.tfm
	cat /dev/null >../doc/testeuro.dvi
	cat /dev/null >feymr10.dvi
	cat /dev/null >feybr10.dvi


.PHONY: mf
mf:
	./makemfs -mf

%.mf: mf

%.tfm: %.mf
	mf '\mode:=ljfour; mag:=1; nonstopmode; input $<'

../tfm/%: %
	cp $< $@

.PHONY: tfm
tfm: mf
	$(MAKE) $(TFM_FILES)

dummy:

ps: ../doc/testeuro.ps.gz

print: ../doc/testeuro.ps.gz
	lpr $<

../doc/testeuro.ps.gz: ../doc/testeuro.dvi
	rm -f $@
	(cd ../doc; dvips -D600 testeuro.dvi -o testeuro.ps && gzip -f testeuro.ps)

ci: dummy
	for i in $(RCSFILES); do \
	  if [ -w $$i ]; then \
	    ci $$i </dev/null; \
	  fi; \
	  if [ ! -f $$i ]; then \
	    co $$i </dev/null; \
	  fi; \
	done

.PHONY: cleandvi
cleandvi: dummy
	rm -f *.dvi ../doc/*.dvi


