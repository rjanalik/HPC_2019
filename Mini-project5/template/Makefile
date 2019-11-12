filename1=template

pdf:
	pdflatex ${filename1}
	bibtex   ${filename1}||true
	pdflatex ${filename1}
	make clean

read:
	evince ${filename1}.pdf  &

clean:
	rm -f *.out *.log *.bbl *.blg *.aux *.log *.ps *.aux *.out *.dvi *.bbl *.blg
