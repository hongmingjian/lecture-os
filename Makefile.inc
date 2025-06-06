.SUFFIXES: .fig .tex .pdf
.fig.pdf:
	fig2dev -L pdf -p xx $< $@
.tex.pdf:
	xelatex -shell-escape $<
	xelatex -shell-escape $<

all: slide handout release

slide: $(LECTURE).pdf
handout: $(LECTURE)_handout.pdf

$(LECTURE)_handout.tex: $(LECTURE).tex
	cp -f $< $@
	sed -e 's@\\documentclass\[@&handout,@' \
	    -e 's@{FullScreen}@{UseNone}@' \
	    -i.bak $@

$(LECTURE).pdf: $(LECTURE).tex $(EXTRADEPENDS)
$(LECTURE)_handout.pdf: $(LECTURE)_handout.tex $(EXTRADEPENDS)

release: $(LECTURE).pdf $(LECTURE)_handout.pdf
	@pdftk $(LECTURE).pdf output $(LECTURE)_slide.pdf owner_pw "$(shell pwgen -1s 12)" encrypt_128bit allow
	@$(RM) -f $(LECTURE).pdf
	@pdftk $(LECTURE)_handout.pdf output $(LECTURE)_print.pdf owner_pw "$(shell pwgen -1s 12)" encrypt_128bit allow Printing ModifyAnnotations
	@$(RM) -f $(LECTURE)_handout.pdf

clean:
	-$(RM) *.pdf *.aux *.log *.out *_handout.* *.bak *.nav *.snm *.toc *.vrb
