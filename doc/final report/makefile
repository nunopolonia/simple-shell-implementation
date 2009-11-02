INPUT=sope
all:
	pdflatex $(INPUT).tex
	pdflatex $(INPUT).tex
	bibtex $(INPUT)
	pdflatex $(INPUT).tex
	pdflatex $(INPUT).tex
	
clean:
	rm -f $(INPUT).aux $(INPUT).dvi $(INPUT).log $(INPUT).pdf $(INPUT).lof $(INPUT).out $(INPUT).toc $(INPUT).bbl $(INPUT).blg
	clear
	
rebuild: clean all
